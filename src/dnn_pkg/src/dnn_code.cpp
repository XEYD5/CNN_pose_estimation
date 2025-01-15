// Includes
#include <opencv2/dnn.hpp>
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <rclcpp/rclcpp.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/msg/image.hpp>
#include "geometry_msgs/msg/point.hpp"

// Consts
const bool green_square = false;
const bool show_image = false;
const int scale = 2;
const double scale_1 = 0.5;
const char* classNames[] = {"background", "aeroplane", "bicycle", "bird", "boat", "bottle", "bus",
                            "car", "cat", "chair", "cow", "diningtable", "dog", "horse", "motorbike",
                            "person", "pottedplant", "sheep", "sofa", "train", "tvmonitor"};

// Namespaces
using namespace cv;
using namespace cv::dnn;
using namespace rs2;

// Node
class DnnNode : public rclcpp::Node
{
    Net net_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr cropped_image_publisher_;
    //rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr xy_publisher_;

    public:
    DnnNode() : Node("dnn_node"), align_to_(RS2_STREAM_COLOR)
    {
        // Load the MobileNetSSD model
        net_ = readNetFromCaffe("/home/jorge/capture_ws/src/dnn_pkg/src/MobileNetSSD_deploy.prototxt",
                                "/home/jorge/capture_ws/src/dnn_pkg/src/MobileNetSSD_deploy.caffemodel");

        // Create the publisher
        cropped_image_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("cropped_image_publisher_", 10);
        cropped_image_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("cropped_image_publisher_", 10);
        //xy_publisher_ = this->create_publisher<geometry_msgs::msg::Point>("xy_publisher_", 10);

        // Subscribe to color and depth image topics
        color_sub_ = create_subscription<sensor_msgs::msg::Image>(
            "camera/color/image_raw", 10, std::bind(&DnnNode::colorCallback, this, std::placeholders::_1));
        //depth_sub_ = create_subscription<sensor_msgs::msg::Image>(
        //    "camera/depth/image_raw", 10, std::bind(&DnnNode::depthCallback, this, std::placeholders::_1));

        // Set the roi's control variable
        roi_initialized_ = false;
    }
    
    private:
        pipeline pipe_;
        rs2::align align_to_;
        pipeline_profile profile_;
        bool roi_initialized_;
        Rect roi_;
        rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr color_sub_;
        rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr depth_sub_;

        void colorCallback(const sensor_msgs::msg::Image::SharedPtr msg)
        {
            // Convert sensor_msgs::msg::Image to cv::Mat
            cv_bridge::CvImagePtr cv_ptr;
            try
            {
                cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
            }
            catch (cv_bridge::Exception &e)
            {
                RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
                return;
            }
            // If the ROI is not initialized, set it to the full image
            if (!roi_initialized_)
            {
                roi_ = Rect(0, 0, cv_ptr->image.cols, cv_ptr->image.rows);
                roi_initialized_ = true;
            }
            // Process the color image and display
            processImage(cv_ptr->image);
        }

        void processImage(Mat &color_mat)
        {
            //Check
            bool check_obj = false;
            // Preprocess input for the neural network
            Mat inputBlob = blobFromImage(color_mat, 0.007843, Size(300, 300), 127.5, false);
            net_.setInput(inputBlob, "data");
            Mat detection = net_.forward("detection_out");
            // Process the detection results
            Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
            float confidenceThreshold = 0.8f;
            for (int i = 0; i < detectionMat.rows; i++)
            {
                float confidence = detectionMat.at<float>(i, 2);

                if (confidence > confidenceThreshold)
                {
                    size_t objectClass = static_cast<size_t>(detectionMat.at<float>(i, 1));

                    // Check if it is the object wanted
                    if (strcmp(classNames[objectClass], "person") == 0 || strcmp(classNames[objectClass], "aeroplane") == 0)
                    {
                        check_obj = true;
                        int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * color_mat.cols);
                        int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * color_mat.rows);
                        int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * color_mat.cols);
                        int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * color_mat.rows);
                        Rect object(xLeftBottom, yLeftBottom, xRightTop - xLeftBottom, yRightTop - yLeftBottom);
                        roi_ = object;
                        // Calculate mean depth inside the detection region
                        // Scalar m = mean(depth_mat(object)); // Uncomment if you want to use depth information
                        std::ostringstream ss;
                        ss << classNames[objectClass] << " ";
                        // ss << std::fixed << std::setprecision(2) << m[0] << " meters away"; // Uncomment if you want to use depth information
                        String conf(ss.str());                       
                        if(green_square){
                            // Draw the detection rectangle and label
                            rectangle(color_mat, object, Scalar(0, 255, 0), 3);
                            int baseLine = 0;
                            Size labelSize = getTextSize(ss.str(), FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
                            auto center = (object.br() + object.tl()) * 0.5;
                            center.x = center.x - labelSize.width / 2;
                            rectangle(
                                color_mat,
                                Rect(
                                    Point(center.x, center.y - labelSize.height),
                                    Size(labelSize.width, labelSize.height + baseLine)
                                ),
                                Scalar(255, 255, 255),
                                FILLED
                            );
                            putText(color_mat, ss.str(), center, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
                        }
                    }
                    if(check_obj){
                        publishCroppedImage(color_mat, roi_);
                        check_obj = false;
                    }
                }
            }
        }
        
        void publishCroppedImage(const cv::Mat& img_o, const Rect& obj)
        {
            //Create a smaller roi
            cv::Rect roi_01(std::max(obj.x / scale, 0), std::max(obj.y / scale, 0), std::min(obj.width, img_o.cols - obj.x), std::min(obj.height, img_o.rows - obj.y));
            roi_01.height /= scale;
            roi_01.width /= scale;
            
            // Create a smaller image
            cv::Mat img_rs;
            cv::resize(img_o, img_rs, cv::Size(), scale_1, scale_1);

            try
            {
                // Create a mask
                cv::Mat mask;

                // Create background and foreground models
                cv::Mat bgdModel, fgdModel;

                // Perform grabCut
                cv::grabCut(img_rs, mask, roi_01, bgdModel, fgdModel, 3, cv::GC_INIT_WITH_RECT);

                // Draw foreground
                mask.setTo(0, mask == 2);
                mask.setTo(1, mask == 3);

                // Convert to grayscale
                cv::Mat img_gray;
                cv::cvtColor(img_rs, img_gray, cv::COLOR_BGR2GRAY);

                // Multiply to remove the background
                cv::Mat img_s_bgd = cv::Mat::zeros(roi_01.size(), CV_8UC1);
                cv::multiply(img_gray, mask, img_s_bgd);

                if(show_image){
                    imshow("Display Image", img_s_bgd);
                    waitKey(1);
                }

                // Send the image
                cv_bridge::CvImagePtr cv_ptr;
                sensor_msgs::msg::Image::SharedPtr msg =
                    cv_bridge::CvImage(std_msgs::msg::Header(), "mono8", img_s_bgd(roi_01))
                        .toImageMsg();
                cropped_image_publisher_->publish(*msg.get());
            }
                /*
                std::cout << "Image Information:" << std::endl;
                std::cout << "Rows: " << img_gray.rows << std::endl;
                std::cout << "Cols: " << img_gray.cols << std::endl;
                std::cout << "Channels: " << img_gray.channels() << std::endl;
                std::cout << "Type: " << img_gray.type() << std::endl;  // Print data type
                std::cout << "Depth: " << img_gray.depth() << std::endl;  // Print depth (8U, 16S, etc.)
                std::cout << "Element Size: " << img_gray.elemSize() << " bytes" << std::endl;
                std::cout << "Total Size: " << img_gray.total() * img_gray.elemSize() << " bytes" << std::endl;
                //cv::imshow("Display Image", img_s_bgd);
                //RCLCPP_INFO(this->get_logger(), "");
                */

            catch (const cv::Exception &ex)
            {
                // Handle the exception (log or rethrow, depending on your needs)
                //RCLCPP_ERROR(this->get_logger(), "Exception during image processing: %s", ex.what());
                return;
            }
        }

};

int main(int argc, char *argv[])
{
    // Initialize the ROS 2 system
    rclcpp::init(argc, argv);

    // Try to run the ROS 2 node
    try
    {
        // Create and run the DnnNode, which is a custom node defined elsewhere in the code
        rclcpp::spin(std::make_shared<DnnNode>());
    }
    // Catch exceptions related to RealSense (rs2::error)
    catch (const rs2::error & e)
    {
        // Print an error message with details about the RealSense error
        std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
        // Return a failure code
        return EXIT_FAILURE;
    }
    // Catch general exceptions (std::exception)
    catch (const std::exception& e)
    {
        // Print an error message with details about the exception
        std::cerr << e.what() << std::endl;
        // Return a failure code
        return EXIT_FAILURE;
    }

    // Shutdown the ROS 2 system
    rclcpp::shutdown();

    // Return a success code
    return 0;
}
// Includes


#include <opencv2/opencv.hpp>
#include <rclcpp/rclcpp.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/msg/image.hpp>
#include "geometry_msgs/msg/point.hpp"

// Consts
const bool show_image = true;
const bool show_info = false;
const int scale = 2;

// Namespaces
using namespace cv;

// Node
class Alg1Node : public rclcpp::Node
{
    public:
        Alg1Node() : Node("alg1_node")
        {
            // Create subscriptions
            subscription_ = create_subscription<sensor_msgs::msg::Image>(
                "cropped_image_publisher_", 10, std::bind(&Alg1Node::colorCallback, this, std::placeholders::_1));
        }

    private:
        rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;

        void colorCallback(const sensor_msgs::msg::Image::SharedPtr msg)
        {
            cv_bridge::CvImagePtr cv_ptr;
            try
            {
                //frameId = msg->header.frame_id;
                cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8);
            }
            catch (cv_bridge::Exception &e)
            {
                RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
                return;
            }
            // Process the color image and display
            alg1(cv_ptr->image);
        }

        void alg2(Mat &img){
            // Apply Sobel filter
            cv::Mat sobelX, sobelY;
            cv::Sobel(img, sobelX, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
            cv::Sobel(img, sobelY, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);

            // Convert back to CV_8U
            cv::Mat absSobelX, absSobelY;
            cv::convertScaleAbs(sobelX, absSobelX);
            cv::convertScaleAbs(sobelY, absSobelY);

            // Combine the results
            cv::Mat result;
            cv::addWeighted(absSobelX, 0.5, absSobelY, 0.5, 0, result);
            showOverBlack(result);
        }

        void alg1(Mat &img_rs)
        {
            // Resize the image back to normal
            cv::Mat img;
            cv::resize(img_rs, img, cv::Size(), scale, scale);

            // Apply Sobel filter
            cv::Mat sobelX, sobelY;
            cv::Sobel(img, sobelX, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
            cv::Sobel(img, sobelY, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);

            // Convert back to CV_8U
            cv::Mat absSobelX, absSobelY;
            cv::convertScaleAbs(sobelX, absSobelX);
            cv::convertScaleAbs(sobelY, absSobelY);

            // Combine the results
            cv::Mat result;
            cv::addWeighted(absSobelX, 0.5, absSobelY, 0.5, 0, result);

            cv::Mat binaryImage;
            cv::threshold(result, binaryImage, 128, 255, cv::THRESH_BINARY);

            // Perform morphological operations (dilation followed by erosion)
            int morphSize = 3;
            cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2 * morphSize + 1, 2 * morphSize + 1));
            cv::Mat morphResult;
            cv::morphologyEx(binaryImage, morphResult, cv::MORPH_CLOSE, element);
            
            // Show the image in a black background
            showOverBlack(morphResult);
        }

        void showOverBlack(Mat &img)
        {   
            // Get the roi
            cv::Rect roi(0, 0, img.cols, img.rows);

            // Create a black image of size 720x1280
            cv::Mat blackImage(720, 1280, CV_8UC1, Scalar(0));

            if(show_info)
            {
                std::cout << "ROI Information:" << std::endl;
                std::cout << "X: " << roi.x << std::endl;
                std::cout << "Y: " << roi.y << std::endl;
                std::cout << "Width: " << roi.width << std::endl;
                std::cout << "Height: " << roi.height << std::endl;
                std::cout << std::endl;
                std::cout << "Image Information:" << std::endl;
                std::cout << "Rows: " << img.rows << std::endl;
                std::cout << "Cols: " << img.cols << std::endl;
                std::cout << "Channels: " << img.channels() << std::endl;
                std::cout << "Type: " << img.type() << std::endl;  // Print data type
                std::cout << "Depth: " << img.depth() << std::endl;  // Print depth (8U, 16S, etc.)
                std::cout << "Element Size: " << img.elemSize() << " bytes" << std::endl;
                std::cout << "Total Size: " << img.total() * img.elemSize() << " bytes" << std::endl;
            }

            // Copy the image into the black background
            img.copyTo(blackImage(roi));

            // Display the black image with the cropped region
            if (show_image)
            {
                imshow("Display Image", blackImage);
                waitKey(1);
            }
        }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Alg1Node>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

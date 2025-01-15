"""
Node: send_frames_node

Subscriptions:
    - It doesn't subscribe, just OS access

Publishers:
    - camera/color/image_raw2

> This node mimics realsense image publisher. The goal is to
use packages that use realsense package but, instead of using
images captured by the camera, use images from a folder. This
is because I wanted to capture and label images separatly.

"""

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <rclcpp/rclcpp.hpp>
#include <iostream>
#include <dirent.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

////////////////////////////////////////////////////////////////////////////////////////////////////
// AUX
////////////////////////////////////////////////////////////////////////////////////////////////////

// Counter
int count = 0;

// Path to dataset_raw folder
std::string sourceFolder_ = "/home/jorge/Desktop/dataset_raw/";

// Get sorted files
std::vector<std::string> listFiles(const std::string& folder) {
    std::vector<std::string> files;
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(folder.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name[0] != '.') { // Skip hidden files
                files.push_back(std::string(ent->d_name));
            }
        }
        closedir(dir);
    }
    // Sort the filenames
    std::sort(files.begin(), files.end());
    return files;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// NODE
////////////////////////////////////////////////////////////////////////////////////////////////////

class SendFramesNode : public rclcpp::Node {

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // PUB
    ////////////////////////////////////////////////////////////////////////////////////////////////

    public:
        SendFramesNode() : Node("send_frames_node") {
            publisher_ = this->create_publisher<sensor_msgs::msg::Image>(
                "camera/color/image_raw2",
                100
            );
            timer_ = this->create_wall_timer(
                std::chrono::milliseconds(10000),
                std::bind(
                    &SendFramesNode::publishImage,
                    this
                )
            );
        }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // CALLBACK
    ////////////////////////////////////////////////////////////////////////////////////////////////

    private:
        void publishImage() {
            std::vector<std::string> files = listFiles(sourceFolder_);
            if (count < files.size()) {
                cv::Mat image = cv::imread(sourceFolder_ + "/" + files[count]);
                cv_bridge::CvImage cv_image;
                cv_image.image = image;
                cv_image.encoding = sensor_msgs::image_encodings::BGR8;
                sensor_msgs::msg::Image::SharedPtr msg = cv_image.toImageMsg();
                rclcpp::Time timestamp(count);
                msg->header.stamp = timestamp;
                msg->header.frame_id = "img_" + std::to_string(count);

                // Publish the image message
                publisher_->publish(*msg);
                std::cout << "Published image: " << files[count++] << std::endl;
                std::cout << count << std::endl;

                // For testing purposes
                //if (count == 1){
                //    exit(0);
                //}
            }
            
            // Finish node
            else {
                std::cout << "Finished publishing all images." << std::endl;
                timer_->cancel();
                exit(0);
            }
        }
    // Pub&Sub Vars
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SendFramesNode>());
    rclcpp::shutdown();
    return 0;
}

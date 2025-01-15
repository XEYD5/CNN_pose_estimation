"""
Node: get_frames_node

Subscriptions:
    - /camera/color/image_raw

Publishers:
    - Doesn't publish, just saves raw images in a folder

> This node saves raw images in the desired folder. Use it
together with the realsense (or other hardware just change
the topic this node subscribes) to save frames. It works with
100ms delay but can be changed.

> img_0001.png
> img_0002.png

"""

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"
#include <opencv2/opencv.hpp>
#include "rclcpp/rclcpp.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <memory>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
// AUX
////////////////////////////////////////////////////////////////////////////////////////////////////

// path to dataset_raw folder. Has to end with '/img_'
std::string path = "/home/jorge/Desktop/dataset_raw/img_";

// Counter
std::string count = "0000";

// Sleep time (recomended: 100ms, 1 img / each 100 ms)
int sleep = 100;

// Function to increment the image counter string from 0001 to 9999
std::string incrementString(const std::string &input) {
    int number = std::stoi(input);
    number = (number % 9999) + 1;
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << number;
    return oss.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// NODE
////////////////////////////////////////////////////////////////////////////////////////////////////

class GetFramesNode : public rclcpp::Node {

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // SUB & SAVE IMAGE
    ////////////////////////////////////////////////////////////////////////////////////////////////

    public:
        GetFramesNode() : Node("get_frames_node") {
            subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
                "/camera/color/image_raw",
                10,
                [this](const sensor_msgs::msg::Image::SharedPtr msg) {
                    cv_bridge::CvImagePtr cv_image_ptr;
                    try {
                        cv_image_ptr = cv_bridge::toCvCopy(
                            msg,
                            sensor_msgs::image_encodings::BGR8
                        );
                    } catch (cv_bridge::Exception& e) {
                        RCLCPP_ERROR(
                            this->get_logger(),
                            "cv_bridge exception: %s",
                            e.what()
                        );
                        return;
                    }
                    count = incrementString(count);
                    std::string path_total = path + count + ".png";
                    cv::imwrite(path_total, cv_image_ptr->image);
                    std::cout << count << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
                }
            );
        }

    private:
        // Pub&Sub Vars
        rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<GetFramesNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

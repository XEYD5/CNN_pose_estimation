"""
Node: get_aruco_node

Subscriptions:
    - /aruco_markers

Publishers:
    - img_labels

> This node reads aruco_marker's messages and formulates a
label that identifies the image read by aruco_marker with
number, yaw category and yaw average value (average between
multiple aruco markers detected). It can be changed with ease
to include different parameters on the label. Define here how
the images should be labeled for the dataset.

> img_1234_L_-99.999
> img_1234_R_99.999
> img_1234_F_0.001
> img_1234_N

"""

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ros2_aruco_interfaces/msg/aruco_markers.hpp"
#include "geometry_msgs/msg/pose_array.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "std_msgs/msg/string.hpp"
#include <eigen3/Eigen/Geometry>
#include <eigen3/Eigen/Dense>
#include "rclcpp/rclcpp.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <memory>
#include <string>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////
// AUX
////////////////////////////////////////////////////////////////////////////////////////////////////

// Counter
std::string count = "0000";

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

class GetArucoNode : public rclcpp::Node {

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // PUB & SUB
    ////////////////////////////////////////////////////////////////////////////////////////////////

    public:
        GetArucoNode() : Node("get_aruco_node") {
            // Subscribes to the ArucoMarkers for its output
            subscription_ = this->create_subscription<ros2_aruco_interfaces::msg::ArucoMarkers>(
                "/aruco_markers",
                100,
                std::bind(
                    &GetArucoNode::getArucoCallback,
                    this,
                    std::placeholders::_1
                )
            );
            // After processing the data, publishes the img_label on "img_labels"
            publisher_ = this->create_publisher<std_msgs::msg::String>(
                "img_labels",
                100
            );
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // CALLBACK
    ////////////////////////////////////////////////////////////////////////////////////////////////

    private:
        void getArucoCallback(const ros2_aruco_interfaces::msg::ArucoMarkers::SharedPtr msg) {
            count = incrementString(count);

            // Variables
            auto class_yaw = "N";
            double yaw_rad = 0;
            double yaw_deg = 0;
            int n_yaws = 0;
            double yaw_average = 0;
            std::ostringstream oss;
            std::string str;

            // Images that aruco didn't detect
            if (msg->detected == false) {
                class_yaw = "N";
                oss << count        << "_"
                    << class_yaw    << "_"
                    << yaw_average;
            }

            // Images that aruco detected
            else {
                // Fetch the yaws
                for (auto pose : msg->poses) {
                    n_yaws += 1;
                    Eigen::Quaterniond q(pose.orientation.w, pose.orientation.x, pose.orientation.y, pose.orientation.z);
                    Eigen::Matrix3d rotationMatrix = q.toRotationMatrix();
                    yaw_rad = std::atan2(rotationMatrix(1, 0), rotationMatrix(0, 0));
                    yaw_deg = yaw_rad * (180.0 / M_PI);
                    yaw_average = (yaw_average + yaw_deg)/n_yaws;
                    // Some feedback
                    std::cout << rotationMatrix << std::endl;
                    std::cout << yaw_rad << std::endl;
                    std::cout << yaw_deg << std::endl;
                    std::cout << yaw_average << std::endl;
                }
                // 15º 'Forward'
                if (yaw_average > -172.5 && yaw_average < -7.5){
                    class_yaw = "L";
                }
                else if (yaw_average < 172.5 && yaw_average > 7.5){
                    class_yaw = "R";
                }
                else {
                    class_yaw = "F";
                }
                oss << count        << "_"
                    << class_yaw    << "_"
                    << yaw_average;
            }

            str = oss.str();
            std::cout << str << std::endl;

            // Publish message
            auto message = std_msgs::msg::String();
            message.data = str;
            publisher_->publish(message);
        }

    // Pub&Sub Vars
    rclcpp::Subscription<ros2_aruco_interfaces::msg::ArucoMarkers>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<GetArucoNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
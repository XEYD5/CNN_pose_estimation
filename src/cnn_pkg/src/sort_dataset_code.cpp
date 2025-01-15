"""
Node: sort_dataset_node

Subscriptions:
    - img_labels

Publishers:
    - Doesn't publish
------------------------------------------------------------
> This node reads the messages published in img_labels. It
copies each image of the source folder (dataset_raw) to the
destination folder (dataset_ready) with a new name (the label
it got from img_labels). This node also sorts the images into
the 3 training folders (train, validation and testing) and the
'aruco_failed'  folder  (for  images  aruco  couldn't  make
estimations). Source images are preserved.

"""

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "sensor_msgs/msg/image.hpp"
#include "std_msgs/msg/string.hpp"
#include "cv_bridge/cv_bridge.h"
#include <opencv2/opencv.hpp>
#include "rclcpp/rclcpp.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>
#include <memory>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
// AUX
////////////////////////////////////////////////////////////////////////////////////////////////////

// Counter
int count = 0;

// Path to unlabeled images
std::string src_path = "/home/jorge/Desktop/dataset_raw/img_";

// Path for labeled images
std::string dst_path = "/home/jorge/Desktop/dataset_ready/";

// Dissect 
void parseString(const std::string &input, std::string &img_number, std::string &label, std::string &value) {
    std::stringstream ss(input);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(ss, item, '_')) {
        tokens.push_back(item);
    }
    img_number = tokens[0];
    label = tokens[1];
    value = tokens[2];
}

// Atribute folder by a specific distribution
std::string getFolder(std::string &class_label) {

    // If it is a non-labeled image, send it to a special folder
    if (class_label == "N") {
        std::cout << "aruco_failed" << std::endl;
        return "/home/jorge/Desktop/dataset_ready/aruco_failed/";
    }
    // Distribution
    double training = 0.7;
    double validation = 0.15;
    double testing = 0.15;

    // Random assignment
    std::vector<std::string> s = {"training/", "validation/", "testing/"};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist({training, validation, testing});
    int i = dist(gen);
    std::string str = dst_path + s[i];

    // Training and Validation have R/L/F subfolders
    if (i != 2) {
        str += class_label + "/";
        std::cout << count << " - " << s[i] << class_label << std::endl;
    }
    else {
        std::cout << "testing" << std::endl;
    }
    // Doesnt matter having Testing images separated
    str += "img_";
    // /.../dataset_ready/tra_val_tes/F_L_R/img_
    return str;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// NODE
////////////////////////////////////////////////////////////////////////////////////////////////////

class SortDatasetNode : public rclcpp::Node {

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // SUB
    ////////////////////////////////////////////////////////////////////////////////////////////////

    public:
        SortDatasetNode() : Node("sort_dataset_node") {
            subscription_ = this->create_subscription<std_msgs::msg::String>(
                "img_labels",
                100,
                std::bind(
                    &SortDatasetNode::sortDatasetCallback,
                    this,
                    std::placeholders::_1
                )
            );
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // CALLBACK
    ////////////////////////////////////////////////////////////////////////////////////////////////

    private:
        void sortDatasetCallback(const std_msgs::msg::String::SharedPtr msg)
        {
            count += 1;
            std::string img_number, class_label, value;
            std::string label = msg->data;
            parseString(label, img_number, class_label, value);

            // Some feedback
            std::cout   << "[" 
                        << img_number   << ","
                        << class_label  << ","
                        << value        << "]"
                        << std::endl;

            // Get full_src_path
            std::string full_src_path = src_path + img_number + ".png";
            
            // Get full_dst_path
            std::string full_dst_path = getFolder(class_label) + img_number + "_" + class_label + "_" + value + ".png";

            // Copy Paste Image
            cv::Mat src_image = cv::imread(full_src_path, cv::IMREAD_COLOR);
            cv::imwrite(full_dst_path, src_image);
        }
    // Pub&Sub Vars
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// NODE
////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SortDatasetNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
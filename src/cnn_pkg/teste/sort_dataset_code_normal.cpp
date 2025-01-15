#include <chrono>
#include <memory>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"
#include "std_msgs/msg/string.hpp"
#include <sstream>
#include <iomanip>
#include <random>

std::string src_path = "/home/jorge/Desktop/dataset_raw/img_";
std::string dst_path = "/home/jorge/Desktop/dataset_ready/";
int count = 0;

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

class ImageSorterNode : public rclcpp::Node
{
public:
    ImageSorterNode() : Node("image_sorter_node") {
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "img_labels", 100, std::bind(&ImageSorterNode::labelCallback, this, std::placeholders::_1));
    }

private:
    void labelCallback(const std_msgs::msg::String::SharedPtr msg)
    {
        count += 1;
        //std::cout << count << std::endl;

        // Get variables
        std::string img_number, class_label, value;
        std::string label = msg->data;
        parseString(label, img_number, class_label, value);

        // Check msg received
        std::cout   << "[" 
                    << img_number   << ","
                    << class_label  << ","
                    << value        << "]"
                    << std::endl;

        // Get full_src_path
        std::string full_src_path = src_path + img_number + ".png";
        //std::cout << full_src_path << std::endl;
        
        // Get full_dst_path
        std::string full_dst_path = getFolder(class_label) + img_number + "_" + class_label + "_" + value + ".png";

        // Copy Paste Image
        cv::Mat src_image = cv::imread(full_src_path, cv::IMREAD_COLOR);
        cv::imwrite(full_dst_path, src_image);
        //std::cout << " " << std::endl;
    }
    // Pubs & Subs
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ImageSorterNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
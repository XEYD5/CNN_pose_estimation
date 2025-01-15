#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <std_msgs/msg/int32.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
//#include <tensorflow/core/public/session.h>
//#include <tensorflow/core/platform/env.h>

int main(){
    return 0;
}
/* #include <tensorflow/core/platform/env.h>
#include <tensorflow/core/public/session.h>
#include <tensorflow/core/protobuf/meta_graph.pb.h>
 */
//using namespace tensorflow;
/* 
class ImageClassifierNode : public rclcpp::Node
{
public:
    ImageClassifierNode()
        : Node("image_classifier_node")
    {
        subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
            "image_topic", 10,
            std::bind(&ImageClassifierNode::imageCallback, this, std::placeholders::_1));
        
        publisher_ = this->create_publisher<std_msgs::msg::Int32>("prediction", 10);
        tensorflow::GraphDef graph;

///     tensorflow::SessionOptions options;
///     std::unique_ptr<tensorflow::Session>
///     session(tensorflow::NewSession(options));
///
///     tensorflow::Status s = session->Create(graph);
///     if (!s.ok()) { ... }
///
///     std::vector<tensorflow::Tensor> outputs;
///     s = session->Run({}, {"output:0"}, {"update_state"}, &outputs);
///     if (!s.ok()) { ... }
///
///     auto output_tensor = outputs[0].flat<float>();
///     if (output_tensor(0) > 0.5) { ... }
///
///     session->Close();

        tensorflow::SessionOptions options;
        std::unique_ptr<tensorflow::Session> session(tensorflow::NewSession(options));
        tensorflow::Status status = session->Create(graph);
        if (!status.ok()) {
            RCLCPP_ERROR(this->get_logger(), "Error creating TensorFlow session: %s", status.ToString().c_str());
        }

        status = ReadBinaryProto(Env::Default(), "path/to/saved_model.pb", &graph_def_);
        if (!status.ok()) {
            RCLCPP_ERROR(this->get_logger(), "Error reading graph: %s", status.ToString().c_str());
        }

        status = session_->Create(graph_def_);
        if (!status.ok()) {
            RCLCPP_ERROR(this->get_logger(), "Error adding graph to session: %s", status.ToString().c_str());
        }
    }

private:
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
        cv::Mat cv_image = cv_bridge::toCvCopy(msg, "bgr8")->image;

        cv::resize(cv_image, cv_image, cv::Size(200, 200));
        cv_image.convertTo(cv_image, CV_32FC3);
        cv_image = cv_image / 255.0;

        Tensor input_tensor(DT_FLOAT, TensorShape({1, 200, 200, 3}));
        auto input_tensor_mapped = input_tensor.tensor<float, 4>();

        for (int y = 0; y < 200; ++y) {
            for (int x = 0; x < 200; ++x) {
                for (int c = 0; c < 3; ++c) {
                    input_tensor_mapped(0, y, x, c) = cv_image.at<cv::Vec3f>(y, x)[c];
                }
            }
        }

        std::vector<Tensor> outputs;
        Status status = session_->Run({{"input_tensor_name", input_tensor}},
                                      {"output_tensor_name"}, {}, &outputs);

        if (!status.ok()) {
            RCLCPP_ERROR(this->get_logger(), "Error running model: %s", status.ToString().c_str());
            return;
        }

        auto output = outputs[0].flat<float>();
        int prediction = std::distance(output.data(), std::max_element(output.data(), output.data() + output.size()));

        auto msg_out = std_msgs::msg::Int32();
        msg_out.data = prediction;
        publisher_->publish(msg_out);
    }

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_;
    std::unique_ptr<Session> session_;
    GraphDef graph_def_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ImageClassifierNode>());
    rclcpp::shutdown();
    return 0;
}
 */
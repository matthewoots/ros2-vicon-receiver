#include "vicon_receiver/publisher.hpp"
#include "rclcpp/time.hpp"

Publisher::Publisher(std::string topic_name, rclcpp::Node* node)
{
    // position_publisher_ = node->create_publisher<vicon_receiver::msg::Position>(topic_name, 10);
    position_publisher_ = node->create_publisher<geometry_msgs::msg::PoseStamped>(topic_name, 10);
    is_ready = true;
}

void Publisher::publish(PositionStruct p)
{

    // To convert any chrono clock to rclcpp time 
    // https://answers.ros.org/question/336045/how-to-convert-a-stdchronohigh_resolution_clock-to-rostime/
    const auto tp0 = std::chrono::time_point<std::chrono::steady_clock>{};
    const auto tp1 = std::chrono::steady_clock::now();

    auto tstamp = tp1 - tp0;
    int32_t sec = std::chrono::duration_cast<std::chrono::seconds>(tstamp).count();
    int32_t nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(tstamp).count() % 1000000000UL;

    rclcpp::Time _time(sec, nsec);

    auto msg = std::make_shared<geometry_msgs::msg::PoseStamped>();
    msg->header.stamp = _time;
    msg->header.frame_id = "world";
    // Vicon resolution is in mm
    double resolution = 1 / 1000;
    msg->pose.position.x = p.translation[0] * resolution;
    msg->pose.position.y = p.translation[1] * resolution;
    msg->pose.position.z = p.translation[2] * resolution;
    msg->pose.orientation.x = p.rotation[0];
    msg->pose.orientation.y = p.rotation[1];
    msg->pose.orientation.z = p.rotation[2];
    msg->pose.orientation.w = p.rotation[3];

    // auto msg = std::make_shared<vicon_receiver::msg::Position>();
    // msg->x_trans = p.translation[0];
    // msg->y_trans = p.translation[1];
    // msg->z_trans = p.translation[2];
    // msg->x_rot = p.rotation[0];
    // msg->y_rot = p.rotation[1];
    // msg->z_rot = p.rotation[2];
    // msg->w = p.rotation[3];
    // msg->subject_name = p.subject_name;
    // msg->segment_name = p.segment_name;
    // msg->frame_number = p.frame_number;
    // msg->translation_type = p.translation_type;

    position_publisher_->publish(*msg);
}

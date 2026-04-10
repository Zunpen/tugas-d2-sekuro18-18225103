#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/string.hpp"

class MoveDestroyer : public rclcpp::Node {
public:
    MoveDestroyer() : Node("move_destroyer") {
        sub_power_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/power_destroyer", 10,
            std::bind(&MoveDestroyer::power_callback, this, std::placeholders::_1));

        sub_type_ = this->create_subscription<std_msgs::msg::String>(
            "/move_type", 10,
            std::bind(&MoveDestroyer::type_callback, this, std::placeholders::_1));
    }

private:
    void power_callback(const geometry_msgs::msg::Twist::SharedPtr msg) {
        std::string arah = "Diam";

        if (msg->linear.x > 0) arah = "Maju";
        else if (msg->linear.x < 0) arah = "Mundur";
        else if (msg->linear.y > 0) arah = "Geser Kiri";
        else if (msg->linear.y < 0) arah = "Geser Kanan";
        else if (msg->angular.z > 0) arah = "Putar Kiri";
        else if (msg->angular.z < 0) arah = "Putar Kanan";

        RCLCPP_INFO(this->get_logger(), "Orientasi Gerak: %s", arah.c_str());
    }

    void type_callback(const std_msgs::msg::String::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "Tipe Kendali Aktif: %s", msg->data.c_str());
    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_power_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_type_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MoveDestroyer>());
    rclcpp::shutdown();
    return 0;
}
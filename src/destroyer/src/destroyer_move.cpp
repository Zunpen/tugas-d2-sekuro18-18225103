#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/string.hpp"

class MoveDestroyer : public rclcpp::Node {
public:
    MoveDestroyer() : Node("move_destroyer") {
        sub_power_ = this->create_subscription<geometry_msgs::msg::Twist>("/power_destroyer", 10, std::bind(&MoveDestroyer::power_callback, this, std::placeholders::_1));

        sub_type_ = this->create_subscription<std_msgs::msg::String>("/move_type", 10, std::bind(&MoveDestroyer::type_callback, this, std::placeholders::_1));
    }

private:
    void power_callback(const geometry_msgs::msg::Twist::SharedPtr msg) {
    double vx = msg->linear.x;
    double vy = msg->linear.y;
    double wz = msg->angular.z;

    double FL = vx - vy - wz;
    double FR = vx + vy + wz;
    double BL = vx + vy - wz;
    double BR = vx - vy + wz;

    auto get_direction = [](double val) {
        if (val > 0.01) return "maju";
        else if (val < -0.01) return "mundur";
        else return "diam";
    };

    std::string fl_dir = get_direction(FL);
    std::string fr_dir = get_direction(FR);
    std::string bl_dir = get_direction(BL);
    std::string br_dir = get_direction(BR);

    RCLCPP_INFO(this->get_logger(), "\nFL : %s\nFR : %s\nBL : %s\nBR : %s\n", fl_dir.c_str(), fr_dir.c_str(), bl_dir.c_str(), br_dir.c_str());
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
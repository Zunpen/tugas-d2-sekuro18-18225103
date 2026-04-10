#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>
#include <random>
#include <cstdlib>
#include <ctime>

using namespace std::chrono_literals;

class AutoModeDestroyer : public rclcpp::Node {
public:
    AutoModeDestroyer() : Node("auto_mode_destroyer"),
        rng_(std::random_device{}())
    {
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/autonomous_destroyer", 10);
        timer_ = this->create_wall_timer(500ms, std::bind(&AutoModeDestroyer::publish_msg, this));
    }

private:
    void publish_msg() {
    auto msg = geometry_msgs::msg::Twist();

    std::uniform_real_distribution<double> angle_dist(0.0, 2.0 * M_PI);
    double angle = angle_dist(rng_);

    double speed = 5.0;

    msg.linear.x = speed * std::cos(angle);
    msg.linear.y = speed * std::sin(angle);

    std::uniform_real_distribution<double> angular_dist(-1.0, 1.0);
    msg.angular.z = angular_dist(rng_);

    publisher_->publish(msg);

    RCLCPP_INFO(this->get_logger(),
        "Auto move -> vx: %.2f vy: %.2f wz: %.2f",
        msg.linear.x, msg.linear.y, msg.angular.z);
}

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    std::mt19937 rng_;
};

int main(int argc, char * argv[]) {
    std::srand(std::time(nullptr));
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<AutoModeDestroyer>());
    rclcpp::shutdown();
    return 0;
}
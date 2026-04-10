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

        std::uniform_int_distribution<int> dist(0, 1);
        msg.linear.x = dist(rng_) ? 5.0 : -5.0;
        msg.linear.y = dist(rng_) ? 5.0 : -5.0;
        msg.angular.z = 0.0;

        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Publishing auto movement");
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
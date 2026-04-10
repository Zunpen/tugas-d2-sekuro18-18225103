#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;

class PriorityMux : public rclcpp::Node {
public:
    PriorityMux() : Node("priority_mux_destroyer"), timeout_(rclcpp::Duration::from_seconds(2.0))
    {
        sub_auto_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/autonomous_destroyer", 10,
            std::bind(&PriorityMux::auto_callback, this, _1));

        sub_drive_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/drive_destroyer", 10,
            std::bind(&PriorityMux::drive_callback, this, _1));

        pub_power_ = this->create_publisher<geometry_msgs::msg::Twist>(
            "/power_destroyer", 10);

        pub_type_ = this->create_publisher<std_msgs::msg::String>(
            "/move_type", 10);

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&PriorityMux::update, this));

    }

private:
    void auto_callback(const geometry_msgs::msg::Twist::SharedPtr msg) {
        last_auto_msg_ = *msg;
        auto_received_ = true;
    }

    void drive_callback(const geometry_msgs::msg::Twist::SharedPtr msg) {
        last_drive_msg_ = *msg;
        last_drive_time_ = this->now();
        drive_received_ = true;
    }

    void update() {
        geometry_msgs::msg::Twist output;
        std::string mode = "Idle";

        bool driver_active = drive_received_ &&
            ((this->now() - last_drive_time_) < timeout_);

        if (driver_active) {
            output = last_drive_msg_;
            mode = "Driver";
        }
        else if (auto_received_) {
            output = last_auto_msg_;
            mode = "Autonomous";
        }

        pub_power_->publish(output);

        std_msgs::msg::String msg_type;
        msg_type.data = mode;
        pub_type_->publish(msg_type);
    }

    geometry_msgs::msg::Twist last_auto_msg_;
    geometry_msgs::msg::Twist last_drive_msg_;

    rclcpp::Time last_drive_time_;

    bool auto_received_ = false;
    bool drive_received_ = false;

    rclcpp::Duration timeout_;

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_auto_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_drive_;

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_power_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_type_;

    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PriorityMux>());
    rclcpp::shutdown();
    return 0;
}
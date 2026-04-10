#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <iostream>
#include <termios.h>
#include <unistd.h>

char getch() {
    char buf = 0;
    struct termios old = {};
    if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO; 
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0) perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0) perror ("tcsetattr ~ICANON");
    return buf;
}

class DestroyerDrive : public rclcpp::Node
{
public:
DestroyerDrive() : Node("drive_mode_destroyer")
{
    publisher_drive_ = this->create_publisher<geometry_msgs::msg::Twist>("drive_destroyer", 10);
    RCLCPP_INFO(this->get_logger(), "Panah: Maju/Mundur/Kiri/Kanan (5 m/s)");
    RCLCPP_INFO(this->get_logger(), "R/L  : Rotasi Kanan/Kiri (1 rad/s)");
    RCLCPP_INFO(this->get_logger(), "B: Rem/Berhenti");
}

void run_keyboard_loop() {
    double vx = 0.0;
    double vy = 0.0;
    double wz = 0.0;

    const double LIN = 5.0;
    const double ANG = 1.0;

    while (rclcpp::ok()) {
        char c = getch();
        bool updated = false;

        if (c == '\x1B') { 
            char seq1 = getch();
            char seq2 = getch();
            
            if (seq1 == '[') {
                switch(seq2) {
                    case 'A': 
                        vx += LIN;
                        updated = true;
                        break;
                    case 'B': 
                        vx -= LIN;
                        updated = true;
                        break;
                    case 'C': 
                        vy -= LIN;
                        updated = true;
                        break;
                    case 'D':
                        vy += LIN;
                        updated = true;
                        break;
                }
            }
        } 
        else {
            if (c == 'r' || c == 'R') {
                wz -= ANG;
                updated = true;
            } 
            else if (c == 'l' || c == 'L') {
                wz += ANG;
                updated = true;
            } 
            else if (c == 'b' || c == 'B') {
                vx = 0.0;
                vy = 0.0;
                wz = 0.0;
                updated = true;
            }
            else if (c == '\x03') {
                break;
            }
        }

        vx = std::max(-5.0, std::min(5.0, vx));
        vy = std::max(-5.0, std::min(5.0, vy));
        wz = std::max(-1.0, std::min(1.0, wz));

        double speed = std::sqrt(vx * vx + vy * vy);
        if (speed > 5.0) {
            vx = (vx / speed) * 5.0;
            vy = (vy / speed) * 5.0;
        }

        if (updated) {
            auto msg = geometry_msgs::msg::Twist();
            msg.linear.x = vx;
            msg.linear.y = vy;
            msg.angular.z = wz;

            publisher_drive_->publish(msg);
        }
    }
}

private:
rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_drive_;
};

int main(int argc, char * argv[])
{
rclcpp::init(argc, argv);

auto node = std::make_shared<DestroyerDrive>();
node->run_keyboard_loop();

rclcpp::shutdown();
return 0;
}

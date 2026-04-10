cmake_minimum_required(VERSION 3.8)
project(destroyer)


if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(auto_mode_destroyer src/destroyer_auto.cpp)
add_executable(drive_mode_destroyer src/destroyer_drive.cpp)
add_executable(check_move_destroyer src/destroyer_check.cpp)
add_executable(move_destroyer src/destroyer_move.cpp)

ament_target_dependencies(auto_mode_destroyer rclcpp geometry_msgs)
ament_target_dependencies(drive_mode_destroyer rclcpp geometry_msgs)
ament_target_dependencies(check_move_destroyer rclcpp geometry_msgs std_msgs)
ament_target_dependencies(move_destroyer rclcpp geometry_msgs std_msgs)

install(TARGETS
  auto_mode_destroyer
  drive_mode_destroyer
  check_move_destroyer
  move_destroyer
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
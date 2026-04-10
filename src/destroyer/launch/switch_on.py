from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    auto_node = Node(
        package='destroyer',
        executable='auto_mode_destroyer',
        name='auto_mode_destroyer',
        output='screen'
    )

    check_node = Node(
        package='destroyer',
        executable='check_move_destroyer',
        name='check_move_destroyer',
        output='screen'
    )

    return LaunchDescription([
        auto_node,
        check_node
    ])

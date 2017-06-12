cmake_minimum_required(VERSION 2.8.3)
project(ConvertCarmenToRosbag)

find_package(catkin REQUIRED COMPONENTS
        geometry_msgs
        nav_msgs
        roscpp
        rospy
        sensor_msgs
        std_msgs
        rosbag
        tf
        )

include_directories(include)

include_directories(
        ${catkin_INCLUDE_DIRS}
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

add_executable(ConvertCarmenToRosbag src/${SOURCE_FILES} src/convert.cpp src/main.cpp)
target_link_libraries(ConvertCarmenToRosbag ${catkin_LIBRARIES})

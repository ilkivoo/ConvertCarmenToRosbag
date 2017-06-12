#include "ros/ros.h"
#include "rosbag/bag.h"
#include "tf/tf.h"
#include "tf/transform_listener.h"
#include "nav_msgs/Odometry.h"
#include "sensor_msgs/LaserScan.h"
#include <fstream>

class Convert {
public:
	Convert(const char*, const char*);
	~Convert();
	void converting();
	void fill_odom();
	void fill_laser_msg(std::string laser_type);
private:
	rosbag::Bag bag;
	ros::Time stamp;
	float rate;
	std::ifstream ifs;
	nav_msgs::Odometry odo_msg;
	std::string odom_link,  robot_link, odom_robot_link;
    geometry_msgs::TransformStamped tf_odom_robot_msg;
    sensor_msgs::LaserScan laser_msg;
};

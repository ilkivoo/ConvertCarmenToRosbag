#include "convert.h"
#include <fstream>
Convert::Convert(const char* log_file_input, const char* file_output) {
 	ifs.open(log_file_input);
 	if (!ifs){
    	throw  std::logic_error (" Can't open file ");
    }
 	bag.open(file_output, rosbag::bagmode::Write);
 	stamp = ros::Time::now();
 	ros::param::param<float>("~global_rate", rate, 40);
 	ros::param::param<std::string>("~odom_link", odom_link, "odom");
  	ros::param::param<std::string>("~robot_link", robot_link, "base_link");
    ros::param::param<std::string>("~odom_robot_link", odom_robot_link, "odom_robot_link");
 }


Convert::~Convert() {
	bag.close();
}


void Convert::converting() {
	std::cout << "Start converting " <<std::endl;
	std::string command, del;
	ifs >> command;
	long long line_number = 1, count_command = 0, count_of_successful_command = 0;
	while (!ifs.eof()) {
		if (command == "RLASER" || command == "FLASER") {
			std::cout << "Line "<< line_number <<": ";
			fill_laser_msg("/" + command);
			count_of_successful_command ++;
			std::cout << "The laser message is converted" << std::endl;
		}
		else if (command == "ODOM") {
			std::cout << "Line "<< line_number <<": ";
			count_of_successful_command ++;
			fill_odom();
			std::cout << "Odmetry message is converted" << std::endl;
		}
		else  if (command != "SYNC" && command != "#" && command != "PARAM") {
			std::cout << "Line "<< line_number <<": ";
			getline(ifs, del);
			std::cout << "Unknown command" << " " << command << std::endl;
			
		}
		else {
			getline(ifs, del);
			count_of_successful_command ++;
		}
		stamp = stamp + ros::Duration(1.0 / rate);
		line_number ++;
		count_command ++;
		ifs >> command;
	}
	std::cout << "Conversion is over." << std::endl;
	std::cout << count_of_successful_command << " commands from " << count_command << " were converted." << std::endl;
}
	

void Convert::fill_odom() {
	float x, y, theta, tr, tv, accel, ipc_timestamp, logger_timestamp;
	std::string  ipc_hostname;

	ifs >> x >> y >> theta >> tr >> tv >> accel >> ipc_timestamp >> ipc_hostname >> logger_timestamp;

	if (ipc_timestamp < 0){
    	throw  std::logic_error (" Time is negative ");
    }

    // write odometry message
    odo_msg.header.stamp = ros::Time(ipc_timestamp);
    odo_msg.header.frame_id = "odom";
    odo_msg.child_frame_id = "base_link";
    odo_msg.pose.pose.position.x = x;
    odo_msg.pose.pose.position.y = y;
    odo_msg.pose.pose.position.z = theta;
    odo_msg.pose.pose.orientation = tf::createQuaternionMsgFromYaw(theta);

    // write tf message
    tf_odom_robot_msg.header.stamp = stamp;
    tf_odom_robot_msg.header.frame_id = "odom";
    tf_odom_robot_msg.child_frame_id = "base_link";
    tf_odom_robot_msg.transform.translation.x = x;
    tf_odom_robot_msg.transform.translation.y = y;
    tf_odom_robot_msg.transform.translation.z = theta;
    tf_odom_robot_msg.transform.rotation = odo_msg.pose.pose.orientation;

    tf::tfMessage tf_msg;
    tf_msg.transforms.push_back(tf_odom_robot_msg);
    bag.write("/ODOM", odo_msg.header.stamp, odo_msg);
 	odo_msg.header.seq++;
    bag.write("/tf", odo_msg.header.stamp, tf_msg);      
	tf_odom_robot_msg.header.seq++;
}


void Convert::fill_laser_msg(std::string laser_type) {
	float value, x, y, theta, odom_x, odom_y, odom_theta, ipc_timestamp, logger_timestamp;
	std::string ipc_hostname;
	std::vector<float> range_readings;
	long long num_readings;
	ifs >> num_readings;
	for (long long i = 0; i < num_readings; i++) {
		ifs >> value;
		range_readings.push_back(value);
	} 
	ifs >> x >> y >> theta >> odom_x >> odom_y >> odom_theta >>  ipc_timestamp >> ipc_hostname >> logger_timestamp;

	if (ipc_timestamp < 0){
    	throw  std::logic_error (" Time is negative ");
    }

    // write laser message
	laser_msg.header.frame_id = "base_link";
	laser_msg.angle_min = (-1) * static_cast<float>(M_PI) / 2;
	laser_msg.angle_max = static_cast<float>(M_PI) / 2;
	laser_msg.angle_increment = static_cast<float>(M_PI) / num_readings;
	laser_msg.range_min = 0;
	laser_msg.range_max = (float) 20;
	laser_msg.ranges = range_readings;
	laser_msg.header.stamp = ros::Time(ipc_timestamp);

 // write tf message
	tf_odom_robot_msg.header.stamp = ros::Time(ipc_timestamp) + ros::Duration(0.01);
    tf_odom_robot_msg.header.frame_id = odom_robot_link;
    tf_odom_robot_msg.child_frame_id = robot_link;
    tf_odom_robot_msg.transform.translation.x = x;
    tf_odom_robot_msg.transform.translation.y = y;
    tf_odom_robot_msg.transform.translation.z = theta;
    tf_odom_robot_msg.transform.rotation = tf::createQuaternionMsgFromYaw(theta);

    tf::tfMessage tf_msg;
    tf_msg.transforms.push_back(tf_odom_robot_msg);

	bag.write(laser_type, laser_msg.header.stamp, laser_msg);
	bag.write("/tf", laser_msg.header.stamp,  tf_msg);
	laser_msg.header.seq ++;
}

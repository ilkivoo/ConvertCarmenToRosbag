Dataset converter from http://radish.sourceforge.net/ to rosbag.

Supports Carmen log file
Message format defined: PARAM SYNC ODOM FLASER RLASER

Installation
1) Install the ROS
2) Download files from this repository
3) Go to the folder where you want to work
4) cmake <path to the folder in which lies CMakeLists.txt>
5) make
6) run roscore
7) open a new terminal
8) ./ConvertCarmenToRosbag -i <input file> -o <output file>

Test
In this repository there is the file nsh_level_a.log, which you can use to test. Also you can find the datasets in the RADISH repository.


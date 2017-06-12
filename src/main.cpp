#include <iostream>
#include "convert.h"
#include <cstring>

using namespace std;

int main(int argc, char** argv) {
	if (argc != 5) {
		std::cout << "Please enter the information in the format: -i < input file >  -o < output file >" << std::endl;
		return 0;
	}

	int index_input_file = -1, index_output_file = -1;

	for (int i = 1; i < argc; i++) {
		if ( strcmp( argv[i], "-i" ) == 0 ) {
			index_input_file = i + 1;
		}

		if ( strcmp( argv[i], "-o" ) == 0 ) {
			index_output_file = i + 1;
		}
	}

	if (index_input_file < 0 || index_input_file > 4 || index_output_file < 0 || index_output_file > 4) {
		std::cout << "Please enter the information in the format: -i < input file >  -o < output file >" << std::endl;
		return 0;	
	}

	try {
		ros::init(argc, argv, "ConvertCarmenToRosbag");
		ros::NodeHandle nh;
		Convert convert(argv[index_input_file], argv[index_output_file]);
		convert.converting();
	}
    catch(std::logic_error &e) {
    	std::cout << e.what() << std::endl;
    }
}

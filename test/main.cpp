// main.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>


void usage() {
    // std::cerr << "Usage: " << program_name << " <input_file> " << std::endl;
    std::cerr << "USAGE: memsim [-q] <input_file> { first | best | next | worst}" << std::endl;
    exit(1);
}

// void readin(std::string input_file);
void readin(const char* input_file);


int main(int argc, char* argv[]){

	if(argc < 3){
		usage();
	}

    Memory M(MEMORY_SIZE, algorithm);
    readin(M, argv[1]);

	return 0;
}

void readin(Memory & memory, const char* input_file){
	std::ifstream file(input_file);
	std::string line;

    if(file.is_open()){
    	int num_processes;
    	getline(file, line);
    	num_processes = std::stoi(line);


    	while(getline(file, line)){
    		std::string process_name;
    		int process_size;
    		int arrive_time;
    		int leave_time;
  
   		    std::istringstream iss(line);

			std::vector<std::string> tokens;
			copy(std::istream_iterator<std::string>(iss),
				std::istream_iterator<std::string>(),
				back_inserter(tokens));
			process_name = tokens[0][;
			process_size = std::stoi(tokens[1]);
            Process p(process_name, process_size);
			for (int i = 2; i < tokens.size(); i = i + 2)
			{
				/* code */
				arrive_time = std::stoi(tokens[i]);
				leave_time = std::stoi(tokens[i + 1]);
                p.addTime(arrive_time, leave_time);
			}
            memory.addProcess(p);
			// std::cout << "Process Name: " << process_name << " process_size: " << process_size
			// 	<< " arrive_time: " << arrive_time << " leave_time: " << leave_time << std::endl;
    	}
    	file.close();
    }
    else {
    	std::cerr << "ERROR: Could not open " << input_file << " for reading." << std::endl;
        usage(); 
    }
}




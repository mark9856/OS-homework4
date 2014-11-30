#include <iostream>
#include <fstream>
#include <vector>

#include "memory.h"

#define MEMORY_SIZE 1600

//function prototype
void readIn(Memory& memory, std::ifstream& input);

//global variable
int time_elapse = 0;

int main(int argc, char* argv[]) {
    if (argc != 3 && argc != 4) {
        std::cerr<<"USAGE: memsim [-q] <input-file> { first | best | next | worst }\n";
        return 1;
    }
    
    std::string algorithm;
    std::ifstream input;
    bool q = 0;//quiet-user flag
    int event = 0;//denote if an event happened in the last second
    //event: 0(nothing),1(process exit), 2(defragmentation), 3(outof memory)
    int time = -1;//user input time
    if (argc == 3) {
        input.open(argv[1]);
        algorithm = argv[2];
    } else {
        q = 1;
        input.open(argv[2]);
        algorithm = argv[3];
    }
    
    Memory M(MEMORY_SIZE, algorithm);
    readIn(M, input);
    while (1) {
        if (!q) {//if not quiet-user mode
            if (time == 0) {//user need to type in a time
                M.print_memory(0, time_elapse);
                std::cin >> time;
            } else {
                time_elapse++;
                //proceed one second for all processes
                //in the memory
                event = M.run_one_second();
                if (event != 0) {//if an event happens
                    M.print_memory(event, time_elapse);
                }
                time--;
            }
        } else {//if quiet-user mode
            time_elapse++;
            //proceed one second for all processes
            //in the memory
            event = M.run_one_second();
            if (event != 0) {//if an event happens
                M.print_memory(event, time_elapse);
            }
        }
    }
    
    return 0;
}

void readIn(Memory& memory, std::ifstream& input) {
    
}

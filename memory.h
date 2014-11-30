#include <string>
#include <map>
#include <list>
#include <vector>
#include "process.h"

class Memory {
public:
    Memory(int s);
    //Memory(int s, std::string algo) {}
    void setAlgorithm(std::string algo) {algorithm = algo;}
    void addProcess(Process& p);
    int runOneSecond();
    int first();
    int best();
    int next();
    int worst();
    int noncontig();
    int defrag();
    void printMemory(int event, int time_elapse);
private:
    int size;
    std::string algorithm;
    std::map<float, int> size_map;
    std::map<int, int> addr_map;
    std::vector<Process> processes;
    std::vector<char> table;
};

// process.h
#include <map>
#include <vector>
#include <string>

class Process {
public:
    Process() {}
    Process(char n, int s):name(n),size(s) {}
    bool addTime(int arrive, int leave);
    int getSize() const {return size;}
    char getName() const {return name;}
private:
    char name;
    int size;
    std::map<int, int> time;
};
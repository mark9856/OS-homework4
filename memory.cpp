#include <math.h>
#include <iostream>
#include "memory.h"


Memory::Memory(int s) {
    size = s;
    size_map.insert(std::make_pair(size-80, 80));
    addr_map.insert(std::make_pair(80, size-80));
    for (int i = 0; i < 80; i++) {
        table.push_back('#');
    }
    for (int i = 80; i < size; i++) {
        table.push_back('.');
    }
}

void
Memory::addProcess(Process& p) {
    //add process to the next available address
    std::map<int, int>::iterator itr = addr_map.end();
    itr--;
    
    processes.push_back(p);
    //update addr_map
    
    int old_addr = itr->first;
    int old_size = itr->second;
    int new_addr = itr->first + p.getSize();
    int new_size = itr->second - p.getSize();
    
    addr_map.erase(itr);
    
    itr = addr_map.insert(std::make_pair(new_addr, new_size)).first;
    
    //change memory table according to addr_map
    int base = itr->first-p.getSize();
    int offset = 0;
    for (; offset < p.getSize(); offset++) {
        table[base+offset] = p.getName();
    }
    
    //update size_map
    std::map<float, int>::iterator itr_s = size_map.begin();
    for (; itr_s != size_map.end(); itr_s++) {
        if (floor(itr_s->first) == old_size) {
            break;
        }
    }
    
    //erase old size-addr pair
    while (floor(itr_s->first) == old_size) {
        if (itr_s->second == old_addr) {
            size_map.erase(itr_s);
            break;
        }
        itr_s++;
    }
    
    //insert new size-addr pair
    while(size_map.insert(std::make_pair(float(new_size), new_addr)).second
          != 1) {
        new_size += 1/size;
    }
    itr_s = size_map.begin();
}

int
Memory::runOneSecond() {
    if (algorithm=="first") {return first();}
    else if (algorithm=="best") {return best();}
    else if (algorithm=="next") {return next();}
    else if (algorithm=="worst") {return worst();}
    else if (algorithm=="noncontig") {return noncontig();}
    return 0;
}

int
Memory::first() {
    
}

int
Memory::best() {
    
}

int
Memory::next() {
    
}

int
Memory::worst() {
    
}

int
Memory::noncontig() {
    
}

int
Memory::defrag() {
    
}

void
Memory::printMemory(int event, int time_elapse) {
    if (event == 0) {//nothing happens
        std::cout << "Memory at time "<<time_elapse<<":\n";
        for (int i = 0; i < size; i++) {
            if (i%80==0) std::cout << "\n";
            std::cout << table[i];
        }
    } else if (event == 1) {//process exit
        
    } else if (event == 2) {//defrag
        
    } else if (event == 3) {//out of memory
        
    }
}
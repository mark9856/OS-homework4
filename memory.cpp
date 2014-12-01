#include <math.h>
#include <iostream>
#include "memory.h"


Memory::Memory(int s) {
    size = s;
    size_map.insert(std::make_pair(size-80, 80));
    addr_map.insert(std::make_pair(80, size-80));
    current_process_addr = 79;
    for (int i = 0; i < 80; i++) {
        table.push_back('#');
    }
    for (int i = 80; i < size; i++) {
        table.push_back('.');
    }
}

void Memory::addProcess(Process& p) {
    //add process to the next available address
    // std::map<int, int>::iterator itr = addr_map.end();
    // itr--;
    
    processes.push_back(p);
    //update addr_map
    
    /*
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
    */
}

int Memory::runOneSecond(int time_elapse) {

    // deal with process leave
    for(int i = 0; i < processes.size(); i ++){
        std::map<int, int>::const_iterator process_it;
        std::map<int, int> process_time = processes[i].getTime();        
        for(process_it = process_time.begin(); process_it != process_time.end(); ++ process_it){
            if(process_it -> second == time_elapse){
                // std::cout << "run leave" << std::endl;
                leave(processes[i]);
            }
        }
    }

    // current_process_addr = 0;
    // deal with process arrive
    for(int i = 0; i < processes.size(); i ++){
        std::map<int, int>::const_iterator process_it;
        std::map<int, int> process_time = processes[i].getTime();
        for(process_it = process_time.begin(); process_it != process_time.end(); ++ process_it){
            if(process_it -> first == time_elapse){
                if(algorithm == "first"){
                    first(processes[i]);
                }
                else if(algorithm == "next"){
                    next(processes[i]);
                }
                else if(algorithm == "best"){
                    best(processes[i]);
                }
                else if(algorithm == "worst"){
                    worst(processes[i]);
                }
                else if(algorithm == "noncontig"){
                    noncontig(processes[i]);
                }
            }
        }
    }

    return 0;
}

int Memory::first(Process p) {
    std::map<int, int>::const_iterator addr_it;
    for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
        // found the first empty space bigger than process space
        if(addr_it -> second > p.getSize()){
            // std::cout << "bigger" << std::endl;
            int i = 0;
            for(i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){                // else{
                table[i] = p.getName();
            }
            int old_addr = addr_it -> first;
            int old_size = addr_it -> second;
            int new_addr = i;
            int new_size = (addr_it -> second) - p.getSize();
            addr_map.erase(addr_it -> first);
            addr_map.insert(std::make_pair(new_addr, new_size));
            removeFromSizeMap(old_addr);
            addSizeMap(new_addr, new_size);
            break;
        }
        // found the first empty space equal to process space
        else if(addr_it -> second == p.getSize()){
            std::cout << "equal" << std::endl;
            for(int i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){
                table[i] = p.getName();
            }
            int old_addr = addr_it -> first;
            addr_map.erase(addr_it -> first);
            removeFromSizeMap(old_addr);
            break;
        }
    }
    return 0;
}

int Memory::best(Process p) {
    std::map<float, int>::const_iterator size_it;
    for(size_it = size_map.begin(); size_it != size_map.end(); ++ size_it){
        if(floor(size_it -> first) == p.getSize()){
            for(int i = size_it -> second; i < size_it -> second + p.getSize(); ++ i){
                table[i] = p.getName();
            }
            int old_addr = size_it -> second;
            addr_map.erase(old_addr);
            removeFromSizeMap(old_addr);            
            break;
        }
        else if(floor(size_it -> first) > p.getSize()){
            for(int i = size_it -> second; i < size_it -> second + p.getSize(); ++ i){
                table[i] = p.getName();
            }
            int old_addr = size_it -> second;
            int old_size = floor(size_it -> first);
            int new_addr = old_addr + p.getSize();
            int new_size = old_size - p.getSize();
            addr_map.erase(old_addr);
            addr_map.insert(std::make_pair(new_addr, new_size));
            removeFromSizeMap(old_addr);
            addSizeMap(new_addr, new_size);
            break;
        }
    }
    return 0;
}

int Memory::next(Process p) {
    bool got = false;
    std::map<int, int>::const_iterator addr_it;
    for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
        if(addr_it -> first <= current_process_addr){
            continue;
        }
        else if(addr_it -> first > current_process_addr){
            // found the first empty space bigger than process space
            if(addr_it -> second > p.getSize()){
                // std::cout << "bigger" << std::endl;
                int i = 0;
                for(i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){                // else{
                    table[i] = p.getName();
                }
                int old_addr = addr_it -> first;
                int old_size = addr_it -> second;
                int new_addr = i;
                int new_size = (addr_it -> second) - p.getSize();
                addr_map.erase(addr_it -> first);
                addr_map.insert(std::make_pair(new_addr, new_size));
                removeFromSizeMap(old_addr);
                addSizeMap(new_addr, new_size);
                current_process_addr = old_addr;
                got = true;
                break;
            }
            // found the first empty space equal to process space
            else if(addr_it -> second == p.getSize()){
                std::cout << "equal" << std::endl;
                for(int i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){
                    table[i] = p.getName();
                }
                int old_addr = addr_it -> first;
                addr_map.erase(addr_it -> first);
                removeFromSizeMap(old_addr);
                current_process_addr = old_addr;
                got = true;
                break;
            }
        }
    }
    if(got == false){
        current_process_addr = 79;
        next(p);
    }
    return 0;
}

int Memory::worst(Process p) {
    std::map<float, int>::const_iterator size_it;
    size_it = size_map.end();
    -- size_it;
    if(floor(size_it -> first) > p.getSize()){
        for(int i = size_it -> second; i < size_it -> second + p.getSize(); ++ i){
            table[i] = p.getName();
        }
        int old_addr = size_it -> second;
        int old_size = floor(size_it -> first);
        int new_addr = old_addr + p.getSize();
        int new_size = old_size - p.getSize();
        addr_map.erase(old_addr);
        addr_map.insert(std::make_pair(new_addr, new_size));
        removeFromSizeMap(old_addr);
        addSizeMap(new_addr, new_size);
    }
    else if(floor(size_it -> first) == p.getSize()){
        for(int i = size_it -> second; i < size_it -> second + p.getSize(); ++ i){
            table[i] = p.getName();
        }
        int old_addr = size_it -> second;
        addr_map.erase(old_addr);
        removeFromSizeMap(old_addr);            
    }
    else if(floor(size_it -> first) < p.getSize()){
        return 3;
    }
    return 0;    
}

int Memory::noncontig(Process p) {
    return 0;    
}

int Memory::defrag() {
    return 0;
}
// when the process leave return current address
int Memory::leave(Process p){
    if(algorithm != "noncontig"){
        int current_addr;
        for(int i = 0; i < size; ++ i){
            if(table[i] == p.getName()){
                current_addr = i;
                // std::cout << "current_addr is " << current_addr << std::endl;
                break;
            }
        }
        for(int i = 0; i < size; ++ i){
            if(table[i] == p.getName()){
                table[i] = '.';
            }
        }

        // create new empty space in both maps
        addr_map.insert(std::make_pair(current_addr, p.getSize()));

        float temp_size = p.getSize();
        int new_addr = current_addr;
        while(size_map.insert(std::make_pair(temp_size, new_addr)).second != 1) {
            temp_size += 0.01;
            // std::cout << temp_size << std::endl;
            // std::cout << "Wooo" << std::endl;
        }

        // attach the empty space together
        bool attach = false;
        std::map<int, int>::const_iterator addr_it;
        int space = p.getSize();
        for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
            // attach the empty space before
            // std::cout << "begin" << std::endl;
            if(addr_it -> first + addr_it -> second == current_addr){
                // std::cout << "before" << std::endl;
                int old_addr = addr_it -> first;
                int old_size = addr_it -> second;
                // int new_addr = addr_it -> first - p.getSize();
                int new_addr = addr_it -> first;
                int new_size = addr_it -> second + space;
                // remove the empty space before current space
                addr_map.erase(addr_it -> first);
                // remove current empty space
                addr_map.erase(current_addr);
                removeFromSizeMap(current_addr);
                addr_map.insert(std::make_pair(new_addr, new_size));
                removeFromSizeMap(old_addr);
                addSizeMap(new_addr, new_size);
                current_addr = new_addr;
                space = new_size;
                break;
            }

        }
        // printMap();
        for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
            // attach the empty space behide
            if(addr_it -> first == current_addr + space){
                // std::cout << "behide" << std::endl;
                int old_addr = addr_it -> first;
                int old_size = addr_it -> second;
                int new_addr = addr_it -> first - space;
                int new_size = addr_it -> second + space;
                addr_map.erase(addr_it -> first);
                // remove current empty space
                addr_map.erase(current_addr);
                removeFromSizeMap(current_addr);
                std::cout << "Hell" <<std::endl;
                addr_map.insert(std::make_pair(new_addr, new_size));
                removeFromSizeMap(old_addr);
                addSizeMap(new_addr, new_size);
                break;
            }
        }
        // printMap();
    }
    else{

    }
    
    return 0;
}

void Memory::printMemory(int event, int time_elapse) {
    
    if (event == 0) {//nothing happens
        std::cout << "Memory at time "<<time_elapse<<":\n";
        for (int i = 0; i < size; i++) {
            if (i%80==0) std::cout << "\n";
            std::cout << table[i];
        }
        std::cout << std::endl;

        printMap();
    } else if (event == 1) {//process exit
        
    } else if (event == 2) {//defrag
        
    } else if (event == 3) {//out of memory
        
    }
    
}


// change the size_map based on addr_map change
// delete old pair in size_map, add new pair to size_map
bool Memory::sizeEditMap(int old_addr, int old_size, int new_addr, int new_size){
    // add new pair to addr_map
    // addr_map.insert(std::make_pair(new_addr, new_size));
    removeFromSizeMap(old_addr);
    float temp_size = new_size;
    while(size_map.insert(std::make_pair(temp_size, new_addr)).second != 1) {
        temp_size += 0.01;
    }
    return true;
}

bool Memory::addSizeMap(int new_addr, int new_size){
    float temp_size = new_size;
    while(size_map.insert(std::make_pair(temp_size, new_addr)).second != 1) {
        temp_size += 0.01;
    }
    return true;
}

bool Memory::removeFromSizeMap(int old_addr){
    std::map<float, int>::const_iterator size_it;
    for(size_it = size_map.begin(); size_it != size_map.end(); ++ size_it){
        if(size_it -> second == old_addr){
            size_map.erase(size_it -> first);
            return true;
        }
    }
    return false;
}


void Memory::printMap(){
    std::map<float, int>::const_iterator size_it;
    std::cout << "size_map is " << std::endl;
    for(size_it = size_map.begin(); size_it != size_map.end(); ++ size_it){
        std::cout << size_it -> first << ", " << size_it -> second << std::endl;
    }
    std::cout << std::endl;
    std::map<int, int>::const_iterator addr_it;
    std::cout << "addr_map is " << std::endl;
    for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
        std::cout << addr_it -> first << ", " << addr_it -> second << std::endl;
    }
    std::cout << std::endl;
}

void Memory::checkMap(){
    /*
    std::map<int, int>::const_iterator addr_it;
    std::map<int, int>::const_iterator addr_it2;

    for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
        for(addr_it2 = ++ addr_it; addr_it2 != addr_map.end(); ++ addr_it2){
            if(addr_it -> first + addr_it -> second == addr_it2 -> first){

                // modify the addr_map
                int old_addr1 = addr_it -> first;
                int old_addr2 = addr_it2 -> first;
                int new_addr = addr_it -> first;
                int new_size = addr_it2 -> second + addr_it -> second;
                addr_map.erase(addr_it);
                addr_map.erase(addr_it2);
                addr_map.insert(std::make_pair(new_addr, new_size));

                // modify the size_map
                removeFromSizeMap(old_addr1);
                removeFromSizeMap(old_addr2);
                float temp_size = new_size;
                while(size_map.insert(std::make_pair(temp_size, new_addr)).second != 1) {
                    temp_size += 0.01;
                }
            }
        }
    }
    */

}



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
// add process to the memory
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

// run One second
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

// use first algorithm
int Memory::first(Process p) {
    std::map<int, int>::const_iterator addr_it;
    // go throung addr_map from begining to end
    for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
        // found the first empty space bigger than process space
        if(addr_it -> second > p.getSize()){
            int i = 0;
            // set table for coming process
            for(i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){                // else{
                table[i] = p.getName();
            }
            // record old and new addr, old and new size
            int old_addr = addr_it -> first;
            int old_size = addr_it -> second;
            int new_addr = i;
            int new_size = (addr_it -> second) - p.getSize();
            // erase and insert to addr_map
            addr_map.erase(addr_it -> first);
            addr_map.insert(std::make_pair(new_addr, new_size));
            // erase and insert to size_map
            // erase from size_map
            removeFromSizeMap(old_addr);
            // add new pair to size_map
            addSizeMap(new_addr, new_size);
            // break;
            return 0;
        }
        // found the first empty space equal to process space
        else if(addr_it -> second == p.getSize()){
            // set table for coming process
            for(int i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){
                table[i] = p.getName();
            }
            // record old addr
            int old_addr = addr_it -> first;
            // erase old from addr_map and size_map
            addr_map.erase(addr_it -> first);
            // erase from size_map
            removeFromSizeMap(old_addr);
            // break;
            return 0;
        }
    }
    // return out of memory
    std::cerr << "OUT-OF-MEMORY" << std::endl;
    return 3;
}

int Memory::best(Process p) {
    std::map<float, int>::const_iterator size_it;
    // go throung size_map from small to big
    for(size_it = size_map.begin(); size_it != size_map.end(); ++ size_it){
        // found the first empty space equal to process space
        if(floor(size_it -> first) == p.getSize()){
            // set table for coming process
            for(int i = size_it -> second; i < size_it -> second + p.getSize(); ++ i){
                table[i] = p.getName();
            }
            // record old addr
            int old_addr = size_it -> second;
            // erase old from addr_map and size_map
            addr_map.erase(old_addr);
            // erase from size_map
            removeFromSizeMap(old_addr);            
            // break;
            return 0;
        }
        // found the first empty space bigger than process space
        else if(floor(size_it -> first) > p.getSize()){
            // set table for coming process
            for(int i = size_it -> second; i < size_it -> second + p.getSize(); ++ i){
                table[i] = p.getName();
            }
            // record old and new addr, old and new size
            int old_addr = size_it -> second;
            int old_size = floor(size_it -> first);
            int new_addr = old_addr + p.getSize();
            int new_size = old_size - p.getSize();
            // erase and insert to addr_map
            addr_map.erase(old_addr);
            addr_map.insert(std::make_pair(new_addr, new_size));
            // erase and insert to size_map
            // erase from size_map
            removeFromSizeMap(old_addr);
            // add new pair to size_map
            addSizeMap(new_addr, new_size);
            // break;
            return 0;
        }
    }
    // return out of memory
    std::cerr << "OUT-OF-MEMORY" << std::endl;
    return 3;
}

int Memory::next(Process p) {
    // whether reach the last slot
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
            // set table for coming process
                for(i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){                // else{
                    table[i] = p.getName();
                }
            // record old and new addr, old and new size
                int old_addr = addr_it -> first;
                int old_size = addr_it -> second;
                int new_addr = i;
                int new_size = (addr_it -> second) - p.getSize();
            // erase and insert to addr_map
                addr_map.erase(addr_it -> first);
                addr_map.insert(std::make_pair(new_addr, new_size));
            // erase and insert to size_map
                // erase from size_map
                removeFromSizeMap(old_addr);
                // add new pair to size_map
                addSizeMap(new_addr, new_size);
                current_process_addr = old_addr;
                got = true;
                // break;
                return 0;
            }
            // found the first empty space equal to process space
            else if(addr_it -> second == p.getSize()){
                std::cout << "equal" << std::endl;
            // set table for coming process
                for(int i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){
                    table[i] = p.getName();
                }
            // record old addr
                int old_addr = addr_it -> first;
            // erase old from addr_map and size_map
                addr_map.erase(addr_it -> first);
                // erase from size_map
                removeFromSizeMap(old_addr);
                current_process_addr = old_addr;
                got = true;
                // break;
                return 0;
            }
        }
    }
    // if reach the last slot
    if(got == false){
        current_process_addr = 79;
        // next(p);
        // std::map<int, int>::const_iterator addr_it;
        for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
            if(addr_it -> first <= current_process_addr){
                continue;
            }
            else if(addr_it -> first > current_process_addr){
                // found the first empty space bigger than process space
                if(addr_it -> second > p.getSize()){
                    // std::cout << "bigger" << std::endl;
                    int i = 0;
                // set table for coming process
                    for(i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){                // else{
                        table[i] = p.getName();
                    }
                // record old and new addr, old and new size
                    int old_addr = addr_it -> first;
                    int old_size = addr_it -> second;
                    int new_addr = i;
                    int new_size = (addr_it -> second) - p.getSize();
                // erase and insert to addr_map
                    addr_map.erase(addr_it -> first);
                    addr_map.insert(std::make_pair(new_addr, new_size));
                // erase and insert to size_map
                    // erase from size_map
                    removeFromSizeMap(old_addr);
                    // add new pair to size_map
                    addSizeMap(new_addr, new_size);
                    current_process_addr = old_addr;
                    got = true;
                    // break;
                    return 0;
                }
                // found the first empty space equal to process space
                else if(addr_it -> second == p.getSize()){
                    std::cout << "equal" << std::endl;
                // set table for coming process
                    for(int i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){
                        table[i] = p.getName();
                    }
                // record old addr
                    int old_addr = addr_it -> first;
                // erase old from addr_map and size_map
                    addr_map.erase(addr_it -> first);
                    // erase from size_map
                    removeFromSizeMap(old_addr);
                    current_process_addr = old_addr;
                    got = true;
                    // break;
                    return 0;
                }
            }
        }
    }
    // return out of memory
    std::cerr << "OUT-OF-MEMORY" << std::endl;
    return 3;
}

int Memory::worst(Process p) {
    std::map<float, int>::const_iterator size_it;
    size_it = size_map.end();
    -- size_it;
    if(floor(size_it -> first) > p.getSize()){
            // set table for coming process
        for(int i = size_it -> second; i < size_it -> second + p.getSize(); ++ i){
            table[i] = p.getName();
        }
            // record old and new addr, old and new size
        int old_addr = size_it -> second;
        int old_size = floor(size_it -> first);
        int new_addr = old_addr + p.getSize();
        int new_size = old_size - p.getSize();
            // erase and insert to addr_map
        addr_map.erase(old_addr);
        addr_map.insert(std::make_pair(new_addr, new_size));
            // erase and insert to size_map
        // erase from size_map
        removeFromSizeMap(old_addr);
        // add new pair to size_map
        addSizeMap(new_addr, new_size);
    }
    else if(floor(size_it -> first) == p.getSize()){
            // set table for coming process
        for(int i = size_it -> second; i < size_it -> second + p.getSize(); ++ i){
            table[i] = p.getName();
        }
            // record old addr
        int old_addr = size_it -> second;
            // erase old from addr_map and size_map
        addr_map.erase(old_addr);
        // erase from size_map
        removeFromSizeMap(old_addr);            
    }
    else if(floor(size_it -> first) < p.getSize()){
        // return out of memory
        std::cerr << "OUT-OF-MEMORY" << std::endl;
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
    // if algorithm is not equal to noncontig
    if(algorithm != "noncontig"){
        // record current address
        int current_addr;
        for(int i = 0; i < size; ++ i){
            if(table[i] == p.getName()){
                current_addr = i;
                // std::cout << "current_addr is " << current_addr << std::endl;
                break;
            }
        }
        // release the memory space after the process leave
        for(int i = 0; i < size; ++ i){
            if(table[i] == p.getName()){
                table[i] = '.';
            }
        }

        // create new empty space in both maps
        addr_map.insert(std::make_pair(current_addr, p.getSize()));

        int new_addr = current_addr;

        int new_size = p.getSize();
        // while(size_map.insert(std::make_pair(temp_size, new_addr)).second != 1) {
        //     temp_size += 0.01;
        //     // std::cout << temp_size << std::endl;
        //     // std::cout << "Wooo" << std::endl;
        // }
        // add new pair to size_map
        addSizeMap(new_addr, new_size);


        // attach the empty space together
        bool attach = false;
        std::map<int, int>::const_iterator addr_it;
        int space = p.getSize();
        for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
            // attach the empty space before
            if(addr_it -> first + addr_it -> second == current_addr){
            // record old and new addr, old and new size
                int old_addr = addr_it -> first;
                int old_size = addr_it -> second;
                int new_addr = addr_it -> first;
                int new_size = addr_it -> second + space;
                // remove the empty space before current space
                addr_map.erase(addr_it -> first);
                // remove current empty space
                addr_map.erase(current_addr);
                // erase from size_map
                removeFromSizeMap(current_addr);
                addr_map.insert(std::make_pair(new_addr, new_size));
                // erase from size_map
                removeFromSizeMap(old_addr);
                // add new pair to size_map
                addSizeMap(new_addr, new_size);
                current_addr = new_addr;
                space = new_size;
                break;
            }

        }// print table
        // printMap();
        for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
            // attach the empty space behide
            if(addr_it -> first == current_addr + space){
            // record old and new addr, old and new size
                int old_addr = addr_it -> first;
                int old_size = addr_it -> second;
                int new_addr = addr_it -> first - space;
                int new_size = addr_it -> second + space;
                addr_map.erase(addr_it -> first);
                // remove current empty space
                addr_map.erase(current_addr);
                // erase from size_map
                removeFromSizeMap(current_addr);
                addr_map.insert(std::make_pair(new_addr, new_size));
                // erase from size_map
                removeFromSizeMap(old_addr);
                // add new pair to size_map
                addSizeMap(new_addr, new_size);
                break;
            }
        }// print table
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
// print table
        printMap();
    } else if (event == 1) {//process exit
        
    } else if (event == 2) {//defrag
        
    } else if (event == 3) {//out of memory
        
    }
    
}

// add new pair to size_map
bool Memory::addSizeMap(int new_addr, int new_size){
    float temp_size = new_size;
    while(size_map.insert(std::make_pair(temp_size, new_addr)).second != 1) {
        temp_size += 0.01;
    }
    return true;
}

// erase from size_map
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

// print table
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




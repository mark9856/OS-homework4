int run_one_second(int time_elapse){
	// deal with process arrive
	for(int i = 0; i < processes.size(); i ++){
		std::map<int, int>::const_iterator process_it;
		for(process_it = processes[i].begin(); process_it != processes[i].end(); ++ process_it){
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
			}
		}
	}

	// deal with process leave
	for(int i = 0; i < processes.size(); i ++){
		std::map<int, int>::const_iterator process_it;
		for(process_it = processes[i].begin(); process_it != processes[i].end(); ++ process_it){
			if(process_it -> second == time_elapse){
				leave(processes[i]);
			}
		}
	}
}


bool first(Process p){
	// std::map<float, int> size_map;
	// std::map<int, int> addr_map;
	std::map<int, int>::const_iterator addr_it;
	for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
		// found the first empty space bigger than process space
		if(addr_it -> second > p.getSize()){
			int i = 0;
			for(i = addr_it -> first; i <= addr_it -> first + p.getSize(); ++ i){
				if(table[i] == "."){
					return false;
				}
				else{
					table[i] = p.getName();
				}
			}
			int old_addr = addr_it -> first;
			int old_size = addr_it -> second;
			int new_addr = i + 1;
			int new_size = (addr_it -> second) - p.getSize();
			addr_map.erase(addr_it -> first);
			if(addrEditMap(old_addr, old_size, new_addr, new_size) == 1)
				return true;
			else{
				std::cerr << "first false" << std::endl;
				return false;
			}
		}
		// found 
		else if(addr_it -> second = p.getSize()){
			for(int i = addr_it -> first; i <= addr_it -> first + p.getSize(); ++ i){
				table[i] = p.getName();
			}
			int old_addr = addr_it -> first;
			addr_map.erase(addr_it -> first);
			removeFromSizeMap(old_addr);
		}
	}
	return false;
}

bool next(){
	//
}

bool best(){
	//
}

bool worst(){
	//
}

bool leave(Process p){

}


// change the size_map based on addr_map change
bool addrEditMap(int old_addr, int old_size, int new_addr, int new_size){
	addr_map.insert(std::make_pair(new_addr, new_size));
	std::map<float, int>::const_iterator size_it;
	for(size_it = size_map.begin(); size_it != size_map.end(); ++ size_it){
		if(size_it -> second == old_addr){
			size_map.erase(size_it -> first);
			while(size_map.insert(std::make_pair(float(new_size), new_addr)).second != 1) {
				new_size += 1/size;
		    }
			return true;
		}
	}
	return false;
}

bool removeFromSizeMap(int old_addr){
	std::map<float, int>::const_iterator size_it;
	for(size_it = size_map.begin(); size_it != size_map.end(); ++ size_it){
		if(size_it -> second == old_addr){
			size_map.erase(size_it -> first);
			return true;
		}
	}
	return false;
}




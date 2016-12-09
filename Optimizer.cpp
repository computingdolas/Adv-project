#include"Optimizer.h"

Optimizer::Optimizer(std::string buildname_,std::string filename_): sim(50,0,6,1),buildname(buildname_),generationnum_(0){
	
    sim.fileRead(filename_);
    
    // Custom Build list for buildpossible void ray and colossus
    build_possible_void_ray = {"void_ray","stargate","cybernetics_core","gateway","pylon"};
    build_possible_colossus = {"colossus","robotics_bay","robotics_facility","cybernetics_core","gateway","pylon"};

}

//Creates the 2 lists init_individuals1 and init_individuals2

void Optimizer::createIndividualsPush(){

    if(buildname == "void_ray")
        init_individuals2 = init_individuals1 = build_possible_void_ray;
    else if(buildname == "colossus")
        init_individuals1 = init_individuals2 = build_possible_colossus;

	init_individuals1.push_back("assimilator");
	init_individuals2.push_back("assimilator");
	init_individuals2.push_back("assimilator");

	std::reverse(init_individuals1.begin(),init_individuals1.end());
	std::reverse(init_individuals2.begin(),init_individuals2.end());

}

// Optimizer
void Optimizer::randomizeIndividualsPush(int numlists_, int listlength_){
	
    std::mt19937 gen;
	gen.seed(generationnum_);
    std::cout<<generationnum_<<std::endl;
    std::uniform_int_distribution<>dis(0,3);
	int size_left = listlength_- (int) init_individuals1.size(),to_insert;
	int probe_left = size_left;

	int count = 0,count1=0,count2=0;

	std::vector<std::string> temp_vec;
	auto it1 = init_individuals1.begin();
	auto it2 = init_individuals2.begin();
	while(count < numlists_ / 2){
		count1 = 0;
		probe_left = size_left;
		it1 = init_individuals1.begin();
  		while(count1 < listlength_){
  			to_insert = min(dis(gen),probe_left);
  			for(int i=1;i<=to_insert;i++){
  				temp_vec.push_back("probe");
  			}
            probe_left -= to_insert;
  			count1 += to_insert;
            temp_vec.push_back(*it1);
  			it1++;
            if (it1 == init_individuals1.end()) {
                break;
            }
  			count1++;
  		}
        for (auto i =0; i < probe_left; ++i) {
            temp_vec.push_back("probe");
        }
        

  		individuals.push_back(temp_vec);
  		temp_vec.clear();
  		//std::cout<<"ho raha hai"<<std::endl;
  		count2 = 0;
  		probe_left = size_left-1;
	    it2 = init_individuals2.begin();
  		while(count2 < listlength_){
  			to_insert = min(dis(gen),probe_left);
  			for(int i=1;i<=to_insert;i++){
  				temp_vec.push_back("probe");
  			}

  			probe_left -= to_insert;
  			count2 += to_insert;

  			temp_vec.push_back(*it2);
  			it2++;
            if (it2 == init_individuals2.end()) {
                break;
            }
  			count2++;
  		}
        for (auto i =0; i < probe_left; ++i) {
            temp_vec.push_back("probe");
        }

  		individuals.push_back(temp_vec);
  		temp_vec.clear();
  		count++;
  	
    }
}

int Optimizer::runSimulation(std::vector<std::string> build_list, int time_steps,std::string goal_name){
    bool is_buildlist_valid = sim.isBuildListValid(build_list);
    
    if (is_buildlist_valid) {
        int count = 0;
        
        auto it = build_list.begin();
        bool build_initiate;
        while(sim.global_time < time_steps){
            
            sim.workerAssignment();
            
            if(it != build_list.end()){
                build_initiate = sim.buildNext(*it);
                if(build_initiate){
                    count++;
                    it++;
                }
            }
            sim.processQueues();
            sim.updateResources();
            sim.global_time++;

            if(sim.countUnits(goal_name) > 0){
                break;
            }
        }
        
        if(sim.countUnits(goal_name) > 0){
             return sim.global_time;
        }
        else{
            return -1;
        }
    }
    else{
        return -2;
    }
}

//Reset the sim object. Has the same effect as calling the constructor with (50,0,6,1)
void Optimizer::reset(){
    sim.supply = 10;
    sim.minerals = 50.0;
	sim.vespene = 0;
	sim.energy = 0;
    sim.global_time = 1;

    // all function 
	Unit newUnit("probe",0);
	// loop to push newUnit into vector for num_workers times 
	sim.units.clear();
	for (int i = 0; i < 6; ++i)
	{
		sim.units.push_back(newUnit);
	}

	Building newBuilding("nexus","Protoss",0);
	//loop to push newBuilding into vector for num_main times

	sim.buildings.clear();
	for (int i = 0; i < 1; ++i)
	{
		sim.buildings.push_back(newBuilding);
	}

	// Initialising the map for count_units
	sim.count_units.clear();
	sim.count_units.insert(std::pair<std::string,int>("probe",6));

	// initialise the map for count_buildings
	sim.count_buildings.clear();
	sim.count_buildings.insert(std::pair<std::string,int>("nexus",1));

	// Initialise the map for countbusy workers 
	sim.count_busy_workers["minerals"] = 0;
	sim.count_busy_workers["vespene"] = 0;

	sim.buildings_queue.clear();
	sim.units_queue.clear();
}

void Optimizer::fitBuildlist(){
    
    int newgensize = (int) (sortedindividuals_.size() / 10);
    std::cout<<"The size of the sorted individuals is :="<<sortedindividuals_.size()<<std::endl;
    auto it = sortedindividuals_.begin();
    for (auto i =0; i < newgensize; ++i) {
        fitgeneration.push_back(it->second);
        ++it;
    }
}



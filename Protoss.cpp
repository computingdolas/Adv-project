#include "Protoss.h"

Protoss::Protoss(double minerals, double vespene, int num_workers, int num_main) :Race(minerals,vespene,num_workers,num_main)
{
	
	race_name = "Protoss"; // race name of the particular species
    supply = num_main*10; // Initial supply given

    // Constructing the probe
	Unit newUnit("probe",0);
    
	// loop to push newUnit into units vector for num_workers times and this is given in initial conditions and could have been done by backinserter //
	for (int i = 0; i < num_workers; ++i)
	{
		units.push_back(newUnit);
	}
    
    // Creating the building
	Building newBuilding("nexus","Protoss",0);
	
    //loop to push newBuilding into vector for num_main times
    for (int i = 0; i < num_main; ++i)
	{
		buildings.push_back(newBuilding);
	}

	// Initialising the map for count_units
    count_units.insert(std::pair<std::string,int>("probe",num_workers));

	// initialise the map for count_buildings

	count_buildings.insert(std::pair<std::string,int>("nexus",num_main));
}

void Protoss::fileRead(const std::string filename)// exactly same in terran
{
	// read the input file and store it in Data Structure 
	BuildingData b1;	

	char delim = ',';
	bool isUnit=true,isProtoss=false;// isZerg=false, isTerran=false;
	std::string line,attr,blank("blank");

	if(race_name == "Protoss") isProtoss = true;
	std::ifstream file_;
	file_.open("techtrees.csv");

	int count = 0;
	int line_number=0;
	// Read the entire line
	while(getline(file_, line) && count <=2) { 
		//std::cout<<line<<std::endl;
		line_number++;
		if(line.substr(0,10) =="#buildings")
		{
			isUnit = false;
			count++;
			//std::cout<<"Buildings found at line number "<<line_number<<std::endl;	
		}
		else if(line.substr(0,10) =="##########")
		{
			isUnit = true;
			count++;
			//std::cout<<"Units found at line number "<<line_number<<std::endl;
		}

				
		// The following if condition has been written to ignore the lines in techtress.csv which starts with #
		if(line.front() != '#') {
			// convert the line into stringstream
			std::stringstream s1(line);
			
			// save the string before delim into string attr
			getline(s1, attr, delim);
			trim_left(attr);
			trim_right(attr);
			b1.name = attr;

			getline(s1, attr, delim);
			trim_left(attr);
			trim_right(attr);
			b1.minerals = std::stod(attr);

			getline(s1, attr, delim);
			trim_left(attr);
			trim_right(attr);
			b1.vespene = std::stod(attr);

			getline(s1, attr, delim);
			trim_left(attr);
			trim_right(attr);
			b1.build_time = std::stoi(attr);

			getline(s1, attr, delim);
			trim_left(attr);
			trim_right(attr);
			b1.supply_cost = std::stoi(attr);

			getline(s1, attr, delim);
			trim_left(attr);
			trim_right(attr);
			b1.supply_provided = std::stoi(attr);

			getline(s1, attr, delim);
			trim_left(attr);
			trim_right(attr);
			b1.start_energy= std::stod(attr);

			getline(s1, attr, delim);
			trim_left(attr);
			trim_right(attr);
			b1.max_energy= std::stod(attr);

			getline(s1, attr, delim);
			trim_left(attr);
			trim_right(attr);
			b1.race = attr;

			getline(s1, attr, delim);
			trim_left(attr);
			trim_right(attr);
			b1.produced_by =attr;

			getline(s1, attr, delim);
			if(attr == ""){
				attr = blank;
			}
			trim_left(attr);
			trim_right(attr);	
			b1.dependency = attr;


			// insert the entry into map
			if(isProtoss) {
				if(isUnit)
					database_units.insert(std::pair<std::string,BuildingData>(b1.name, b1));
				else 
					database_buildings.insert(std::pair<std::string,BuildingData>(b1.name, b1));
			}
		}

	}	

	file_.close();

}

// Worker Assignment function ....
void Protoss::workerAssignment()
{
	// Local_Variables
	int free_count = countFreeUnits("probe");
	int in_vespene = count_busy_workers["vespene"];
	int to_assign=0;
	
	// Pointer declaration
	auto it=units.begin();

	if(free_count > 0)
	{
		//Assign to vespene whatever maximum we can assign 
		if(count_buildings["assimilator"]*3 - in_vespene)
		{
			to_assign = count_buildings["assimilator"]*3 - in_vespene;
			while(free_count > 0 && to_assign > 0)
			{
				if((!(it->busy)) && (it->name == "probe"))
				{
					it->busy = true;
					it->busy_with_vespene = true;
					count_busy_workers["vespene"]++;
					free_count--;
					to_assign--;
				}
				it++;
			}

			//If more workers can still be assigned to vespene
			if(to_assign > 0){
				auto it = units.begin();
				while(to_assign > 0 && count_busy_workers["minerals"] > 0){
					if(it->name== "probe" && it->busy_with_mineral){
						it->busy_with_mineral = false;
						it->busy_with_vespene = true;

						count_busy_workers["vespene"]++;
						count_busy_workers["minerals"]--;

						to_assign--;
					}
					it++;
				}
			}
		}

		//Assign to mineral whatever is left after assignment to vespene if there is any 
		if(free_count > 0)
		{
			it = units.begin();
			while(free_count > 0)
			{
				if((!(it->busy)) && (it->name == "probe"))
				{
					it->busy = true;
					it->busy_with_mineral = true;
					count_busy_workers["minerals"]++;
					free_count--;
				}
				it++;
			}
		}
	}
}

// Function build Next... which is given task of building the unit or building 
bool Protoss::buildNext(const std::string next_build_name)
{
	// local_Variables
	std::string produced_by;
	std::string dependency;
	double minerals_req;
	double vespene_req;
	int supply_cost;
	int build_time;
	int supply_provided;
	int start_energy;
	int max_energy;

	//The next thing to build can be a unit or a building
	bool is_unit = isUnit(next_build_name);
	bool is_building = isBuilding(next_build_name);

	//Checking the requirements and properties associated with the next build.

	// Obtaining the required parameters of units
	if(is_unit)
	{
		produced_by = database_units[next_build_name].produced_by;
        dependency = database_units[next_build_name].dependency;
        minerals_req = database_units[next_build_name].minerals;
        vespene_req = database_units[next_build_name].vespene;
        supply_cost = database_units[next_build_name].supply_cost;
        build_time = database_units[next_build_name].build_time;
        supply_provided = database_units[next_build_name].supply_provided;
        start_energy = database_units[next_build_name].start_energy;
        max_energy = database_units[next_build_name].max_energy;
	}
	// else obtaining for building 
	else
	{

		produced_by = database_buildings[next_build_name].produced_by;
        dependency = database_buildings[next_build_name].dependency;
        minerals_req = database_buildings[next_build_name].minerals;
        vespene_req = database_buildings[next_build_name].vespene;
        supply_cost = database_buildings[next_build_name].supply_cost;
        build_time = database_buildings[next_build_name].build_time;
        supply_provided = database_buildings[next_build_name].supply_provided;
        start_energy = database_buildings[next_build_name].start_energy;
        max_energy = database_buildings[next_build_name].max_energy;
	}

	//Can be produced by a building or a unit. Check which is the case.Check if the produced_by building is already exist or not
	bool find_produced_by = findBuilding(produced_by) || findUnit(produced_by);

	//Dependency could be blank. Dependency is always a building in Protoss.Checking if dependency exist or not 
	bool find_dependency = findBuilding(dependency);
	
	//Index of the location of the producer building
	int producer_rank = 0;
	//Initiate the building process
	if(find_produced_by && find_dependency) // first level of checking ..//
	{
		if(minerals >= minerals_req && vespene >= vespene_req && supply >= supply_cost)// second level of checking //
		{
			// Add to unit queue if it is a unit  nut it is produced by building //
			if(is_unit)
			{
                // counting if any building is busy or not //
				if(countFreeBuildings(produced_by) > 0)
                {
					producer_rank = findFreeBuilding(produced_by);
					Unit new_unit(next_build_name,build_time); // it is not necesary here to define object of the unit class explicility , we can also implicilty ..//
	
					units_queue.push_back(std::pair<Unit,int>(new_unit,producer_rank)); // pushing into units_queue
					setBusy(producer_rank,1); // since it is produced by building , building as been set busy //
                    
                    // Substract the resource required from global resource count
                    minerals -= minerals_req;
                    vespene -= vespene_req;
             
				}
				else
                {
					//std::cout<<"Builder busy"<<std::endl;
					return false;
				}
			}
			// Add to building queue if it is building 
			else if(is_building)
			{
				Building new_building(next_build_name,"Protoss",build_time);
				buildings_queue.push_back(new_building);
                // Substract the resource required from global resource count and no one is busy since in protoss building made bu unit does not occupy unit time
                minerals -= minerals_req;
                vespene -= vespene_req;
               
            }

        }
		else
		{
			//std::cout<<"Not enough resources"<<std::endl;
			return false;
		}
	}
	else
	{
		//std::cout<<"Dependency or produced_by not found"<<std::endl;
		return false;
	}
	return true;
}
 
void Protoss::energyUpdate()
{
	energy += rate_energy*countBuildings("nexus");
}

// Function to process queues added by build next function 
void Protoss::processQueues()
{
	//PrintQueueStatus();
	int time_check;

	// Iterating through the buildings queue
	for(auto it= buildings_queue.begin();it!= buildings_queue.end();it++)
	{
        // starting the loop and substracting the time ...//
		time_check = --(it->time_left);
		if(time_check == 0) // checking if it is build //
		{
			buildings.push_back(*it); // Add the current object to vector buildings
            
            //updating the map find_building if the building already existing //
			if(findBuilding(it->name))
			{
				++count_buildings[it->name];
			}
			else // otherwise create anoher entry for the current entry //
			{
				count_buildings[it->name] = 1;
			}
			supply += database_buildings[it->name].supply_provided;
            buildings_queue.erase(it); // when we erase iterator points to the nest position // ideally we should not remove container objects in the loop we are traversing // and that's why substracting it //
			--it; // updating the iterator //
        }
	}
	// iterating through units queues
	for(auto it=units_queue.begin();it!=units_queue.end();++it)
	{
		time_check = --((it->first).time_left);
		
		if(time_check == 0){
			units.push_back(it->first); // add the current object to unit vector

			if(findUnit(it->first.name))
			{
				count_units[it->first.name]++;
			}
			else
			{
				count_units[it->first.name] = 1;
			}

			setFree(it->second,1); // units is produced by building and we are freeing the building once we have finished building the object//
			
			units_queue.erase(it);
			it--; // erasing the updating the iterator // 
		}
		
	}
}

bool Protoss::isBuildListValid(const std::vector<std::string> &build_list){
    
    // local variables
    std::string produced_by;
    std::string dependency;
    bool is_unit;
    bool is_building;
    
    if(!build_list.size()){
        std::cout<<"Build list empty"<<std::endl;
        return false;
    }
    
    bool found_assimilator = false;
    bool is_vespene_required = false;
    
    for (auto it = build_list.begin(); it!= build_list.end(); ++it)
    {
        
        bool found_produced_by = false;
        bool found_dependency = false;
        
        //std::cout<<"The name of the next game unit is"<<*it<<std::endl;
        is_unit = isUnit(*it);
        is_building = isBuilding(*it);
        
        if (is_unit)
        {
            produced_by = database_units[*it].produced_by;
            dependency = database_units[*it].dependency;
            
            if (database_units[*it].vespene > 0) {
                is_vespene_required = true;
            }
            
            
            if (dependency == "blank") {
                found_dependency = true;
            }
            
            
            for (auto it1= buildings.begin(); it1!=buildings.end(); ++it1)
            {
                // check for produced_by
                if (it1->name == (produced_by))
                {
                    found_produced_by = true;
                    //std::cout<<"producer found"<<std::endl;
                }
                // check for dependency
                if (it1->name == (dependency))
                {
                    found_dependency = true;
                    //std::cout<<"depedency found"<<std::endl;
                }
                //std::cout<<"I am here"<<std::endl;
                
                if (is_vespene_required) {
                    if (it1->name == "assimilator"){
                        found_assimilator = true;
                    }
                }
            }
            
            // now we have to check in buildlist
            
            for (auto it2 = build_list.begin(); it2!=it; ++it2) {
                
                // check for produced by
                if ((*it2) == (produced_by)) {
                    
                    found_produced_by = true;
                    //std::cout<<"Producer found"<<std::endl;
                }
                
                if ((*it2) == dependency) {
                    
                    found_dependency = true;
                    //std::cout<<"dependency found "<<std::endl;
                }
                //std::cout<<"I am here 1"<<std::endl;
                if (is_vespene_required) {
                    if (*it2 == "assimilator") {
                        found_assimilator = true;
                    }
                }
                
            }
             if (  (!found_produced_by) || (!found_dependency) ||  ((is_vespene_required) && !(found_assimilator))  ) {
                return false;
            }
            
        }
        else
        {
            produced_by = database_buildings[*it].produced_by;
            dependency = database_buildings[*it].dependency;
            
            if (dependency == "blank") {
                found_dependency = true;
            }
            
            if (database_buildings[*it].vespene > 0) {
                is_vespene_required = true;
            }
            
            
            // now we have to check if produced_by and dependency exits in either the buildlist before this game unit or it is existed in building
            
            // we know that for buildings in protoss , only units are producers and buildings are dependency
            
            for (auto it1= units.begin(); it1!=units.end(); ++it1)
            {
                
                // check for produced_by
                if (it1->name == (produced_by))
                {
                    found_produced_by = true;
                    //std::cout<<"producer found"<<std::endl;
                }
        
            }
            
            for (auto it3 = buildings.begin(); it3!=buildings.end(); ++it3)
            {
                
                // check for dependency
                if(it3->name == (dependency))
                {
                    found_dependency = true;
                    //std::cout<<"depedency found"<<std::endl;
                }
                
                if (is_vespene_required) {
                    if (it3->name == "assimilator") {
                        found_assimilator = true;
                    }
                }
                            
            }
            
            // now we have to check in buildlist
            
            for (auto it2 = build_list.begin(); it2!=it; ++it2) {
                
                // check for produced by
                if ((*it2) == (produced_by)) {
                    
                    found_produced_by = true;
                    //std::cout<<"Producer found in build_list"<<std::endl;
                }
                
                if ((*it2) == dependency) {
                    
                    found_dependency = true;
                    //std::cout<<"dependency found in buildlist "<<std::endl;
                }
                
                if (is_vespene_required) {
                    if (*it2 == "assimilator") {
                        found_assimilator = true;
                    }
                }
            }
 
            if (  (!found_produced_by) || (!found_dependency) ||  ((is_vespene_required) && !(found_assimilator))  ) {
                return false;
            }
            
        }
        
    }
    return true;
}
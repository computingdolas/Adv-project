#include "Terran.h"

// why did we define different fileread in terran and protoss//
void Terran::fileRead(const std::string filename)
{
	// read the input file and store it in Data Structure 
	BuildingData b1;	

	char delim = ',';
    char delim1 = '/';
	bool isUnit=true,isTerran=false;// isZerg=false, isTerran=false;
	std::string line,attr,blank("blank"),prefix;

	if(race_name == "Terran") isTerran = true;
	std::ifstream file_;
	file_.open("techtrees.csv");

	int count = 0;
	int line_number=0;

	//Move the file pointer ahead till it finds Terran
	while(getline(file_, line) && line.substr(0,8) != "# Terran"){ 
		line_number++;
		//std::cout<<line<<std::endl;
	}

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
            std::stringstream s2(attr);
            getline(s2,prefix,delim1);
			trim_left(prefix);
			trim_right(prefix);
			b1.produced_by =prefix;

			getline(s1, attr, delim);
            std::stringstream s3(attr);
            //prefix = attr.substr(0, attr.find('/'));
            getline(s3,prefix,delim1);
			if(prefix == ""){
				prefix = blank;
			}
			trim_left(prefix);
			trim_right(prefix);
			b1.dependency = prefix;
            //std::cout<<prefix<<std::endl;
			// insert the entry into map
			if(isTerran) {
				if(isUnit)
					database_units.insert(std::pair<std::string,BuildingData>(b1.name, b1));
				else 
					database_buildings.insert(std::pair<std::string,BuildingData>(b1.name, b1));
			}
		}

	}	

	file_.close();

}

Terran::Terran(double minerals,double vespene, int num_workers, int num_main):Race(minerals,vespene,num_workers,num_main){
	race_name = "Terran";
	supply = num_main * 11;

	Unit newUnit("scv",0);
	// loop to push newUnit into vector for num_workers times 
	for (int i = 0; i < num_workers; ++i)
	{
		units.push_back(newUnit);
	}

	Building newBuilding("command_center","Terran",0);
	//loop to push newBuilding into vector for num_main times

	for (int i = 0; i < num_main; ++i)
	{
		buildings.push_back(newBuilding);
	}

	// Initialising the map for count_units

	count_units.insert(std::pair<std::string,int>("scv",num_workers));

	// initialise the map for count_buildings

	count_buildings.insert(std::pair<std::string,int>("command_center",num_main));

	morphs_database["command_centre"] = {"orbital_command","planetary_fortress"};
	morphs_database["barracks"] = {"barracks_with_reactor","barracks_with_tech_lab"};
	morphs_database["factory"] = {"factory_with_reactor","factory_with_tech_lab"};
	morphs_database["starport"] = {"starport_with_reactor","starport_with_tech_lab"};
}

void Terran::energyUpdate()
{
	int num_buildings = countBuildings("orbital_command");
	energy += rate_energy*num_buildings;
	energy = (energy>200*num_buildings)?200*num_buildings:energy;
}

// Worker Assignment function ... First assign the workers to vespene and then whatever is left is assigned to mineral
// extraction ..
void Terran::workerAssignment()
{
	// Local_Variables
	int free_count = countFreeUnits("scv");
	int in_vespene = count_busy_workers["vespene"];
	int to_assign=0;
	
	// Pointer declaration
	auto it=units.begin();

	// Check if there is any free worker
	//std::cout<<"Number of assimilators is "<<count_buildings["assimilator"]<<std::endl; 
	//std::cout<<"Number of in_vespene is"<<in_vespene<<std::endl;
	//std::cout<<"Free count is "<<free_count<<std::endl;
	if(free_count > 0)
	{
		//Assign to vespene whatever maximum we can assign 
		if(count_buildings["refinery"]*3 - in_vespene)
		{
			to_assign = count_buildings["refinery"]*3 - in_vespene;
			while(free_count > 0 && to_assign > 0)
			{
				if((!(it->busy)) && (it->name == "scv"))
				{
					it->busy = true;
					it->busy_with_vespene = true;
					count_busy_workers["vespene"]++;
					free_count--;
					to_assign--;
				}
				it++;
			}
		}

		//Assign to mineral whatever is left after assignment to vespene if there is any 
		if(free_count > 0)
		{
			it = units.begin();
			while(free_count > 0)
			{
				if((!(it->busy)) && (it->name == "scv"))
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

bool Terran::buildNext(const std::string next_build_name)
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
    
    
    //std::cout<<"isbuilding is"<<is_building<<std::endl;
    
    //std::cout<<"is_unit variable and is_building are:="<<is_unit<<"  "<<is_building<<std::endl;

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

	//Checking if morphing is needed or building
	bool producer_is_building = isBuilding(produced_by);
	
    //bool producer_is_unit = isUnit(produced_by);
	bool is_morphing = false;
	if(producer_is_building && is_building){
		is_morphing = true;
	}

	bool find_produced_by,find_dependency,resource_check,tech_level;
	int producer_rank = -1;

	//Checking if there are enough resources
	resource_check = (minerals >= minerals_req && vespene >= vespene_req && supply >= supply_cost);
	if(is_morphing)
    {
		find_produced_by = findBuilding(produced_by) || findUnit(produced_by);
		find_dependency = findBuilding(dependency);
		tech_level = find_produced_by && find_dependency;

		if(tech_level && resource_check)
        {
			if(countFreeBuildings(produced_by) > 0)
            {
				producer_rank = findFreeBuilding(produced_by);
				
				//std::cout<<"Pushed into morphs queue, time: "<<next_build_name<<" "<<global_time<<std::endl;
				morphs_queue.push_back(std::tuple<int,int,std::string>(producer_rank,build_time,next_build_name));
                //
				setBusy(producer_rank,1);
			
                minerals -= minerals_req;
                vespene -= vespene_req;
                supply -= supply_cost;
                
                //return true;
            }
		}
		else
        {
			return false;
		}
	}
	else
    {
        find_produced_by = findBuildingUpgrade(produced_by) || findUnit(produced_by);
		find_dependency = findBuildingUpgrade(dependency);
		tech_level = find_produced_by && find_dependency;
        
		if(tech_level && resource_check)
        {
            if(is_unit){
				if(countFreeBuildingsUpgrade(produced_by) > 0)
                {
                    producer_rank = findFreeBuildingUpgrade(produced_by);
					Unit new_unit(next_build_name,build_time);
					//std::cout<<"Pushed into units queue, time: "<<next_build_name<<" "<<global_time<<std::endl;
					units_queue.push_back(std::pair<Unit,int>(new_unit,producer_rank));
					setBusy(producer_rank,1);

					minerals -= minerals_req;
					vespene -= vespene_req;
					supply -= supply_cost;
                    //return true;
				}
				else
                {
					return false;
				}
			}
			else if(is_building)
            {
                producer_rank = findFreeUnit(produced_by);
				if(producer_rank >= 0)
                {
                    
					Building new_building(next_build_name,"Terran",build_time);
					buildings_queue.push_back(std::pair<Building,int>(new_building,producer_rank));
					//std::cout<<"Pushed into buildings queue, time: "<<next_build_name<<" "<<global_time<<std::endl;
					setBusy(producer_rank,0);

					minerals -= minerals_req;
					vespene -= vespene_req;
					supply -= supply_cost;
                    //return true;
                    
				}
				else
                {
					return false;
				}
			}
		}
		else
        {
			return false;
		}
	}
    return true;
}

//Find if a building or any of its upgrades exist
bool Terran::findBuildingUpgrade(std::string name_check){
	if(name_check == "blank"){
		return true;
	}
    if(countBuildingsUpgrade(name_check) > 0)
	{
		return true;
	}
    else
        return false;
}

int Terran::countBuildingsUpgrade(std::string building_name){
	if(count_buildings[building_name] > 0){
		return count_buildings[building_name];
	}
	else if(morphs_database.find(building_name) != morphs_database.end()){
		auto it = morphs_database.find(building_name);
		for(auto it1=it->second.begin();it1 != it->second.end();it1++){
			if(countBuildings(*it1) > 0){ 
				return countBuildings(*it1);
			}
		}
		return 0;
	}
	else{
		return 0;
	}
}

int Terran::countFreeBuildingsUpgrade(std::string building_name){
	int free_count = 0;
	for(auto it=buildings.begin();it != buildings.end(); ++it)
	{
		if(it->name == building_name && !(it->busy)) 
		{
			free_count++;
		}
	}
	if(free_count > 0){
		return free_count;
	}
	else if(morphs_database.find(building_name) != morphs_database.end()){
		auto it1=morphs_database.find(building_name);
		for(auto it2=it1->second.begin();it2 != it1->second.end();it2++){
			if(countFreeBuildings(*it2) > 0){
				return countFreeBuildings(*it2);
			}
		}
		return 0;
	}
	else{
		return 0;
	}	
}

//Should only be called if countFreeBuildingsUpgrade(building_name) > 0
int Terran::findFreeBuildingUpgrade(std::string building_name){
	auto it=buildings.begin();

	bool found = false;
	int producer_rank = 0;
	while(!found && it != buildings.end())
	{
		if(it->busy == false && it->name == building_name)
		{
			found = true;
			return producer_rank;
		}
		else{
			it++;
			producer_rank++;
		}
	}

	if(morphs_database.find(building_name) != morphs_database.end()){
		auto it1 = morphs_database.find(building_name);
		for(auto it2=it1->second.begin();it2 != it1->second.end();it2++){
			if(findFreeBuilding(*it2) >= 0){
				return findFreeBuilding(*it2);
			}
		}
		return -1;
	}
	else{
		return -1;
	}
}

void Terran::processQueues(){
	//PrintQueueStatus();
	int time_check;

	//std::cout<<"Before buildings iteration"<<std::endl;
	// Iterating through the buildings queue
	for(auto it= buildings_queue.begin();it!= buildings_queue.end();it++)
	{
		time_check = --(it->first.time_left);
		if(time_check == 0)
		{
			buildings.push_back(it->first); // Add the current object to vector buildings 

			if(findBuilding(it->first.name))
			{
				++count_buildings[it->first.name];
			}
			else
			{
				//count_buildings.insert(std::pair<std::string,int>(it->name,1));
				count_buildings[it->first.name] = 1;
			}
			supply += database_buildings[it->first.name].supply_provided;
			energy += database_buildings[it->first.name].start_energy;

			setFree(it->second,0);
			buildings_queue.erase(it);
			it--;
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

			setFree(it->second,1);
			
			//std::cout<<"G_t, produced_by"<<global_time<<" "<<(*it).second.name<<std::endl;
			
			units_queue.erase(it);
			it--;
		}
	}

	for(auto it=morphs_queue.begin();it != morphs_queue.end();it++){
		time_check = --(std::get<1>(*it));

		if(time_check == 0){
			auto it1 = buildings.begin();
			int rank = std::get<0>(*it);

			it1 = it1+rank;

			count_buildings[it1->name]--;

			it1->name = std::get<2>(*it);
			if(findBuilding(it1->name))
			{
				++count_buildings[it1->name];
			}
			else
			{
				count_buildings[it1->name] = 1;
			}

			if(it1->name == "starport_with_reactor" || it1->name == "barracks_with_reactor" || it1->name == "factory_with_reactor"){
				it1->max_load = 2;
			}

			setFree(rank,1);
            morphs_queue.erase(it);
            --it;
		}
	}
}

bool Terran::isBuildListValid(const std::vector<std::string> &build_list)
{
    std::vector<std::string> built;
    built.push_back("command_center");
    built.push_back("scv");
    
    bool is_unit,is_building,producer_found,dependency_found;
    std::string producer,dependency;
    for(auto it=build_list.begin();it != build_list.end();it++){
        is_unit = isUnit(*it);
        is_building = isBuilding(*it);
        
        if(is_unit){
            producer = database_units[*it].produced_by;
            dependency = database_units[*it].dependency;
            producer_found = findInsideUpgrade(built,producer);
            dependency_found = findInsideUpgrade(built,dependency);
            
            if(producer_found && dependency_found){
                built.push_back(*it);
            }
            else{
                return false;
            }
        }
        else{
            producer = database_buildings[*it].produced_by;
            dependency = database_buildings[*it].dependency;
            if(isUnit(producer)){
                producer_found = findInsideUpgrade(built,producer);
                dependency_found = findInsideUpgrade(built,dependency);
                if(producer_found && dependency_found){
                    built.push_back(*it);
                }
                else{
                    return false;
                }
            }
            else{
                producer_found = findInside(built,producer);
                dependency_found = findInsideUpgrade(built,dependency);
                if(producer_found && dependency_found){
                    built.push_back(*it);
                }
                else{
                    return false;
                }
            }
        }
    }
    return true;
}

bool Terran::findInside(const std::vector<std::string> &vec, const std::string to_find)
{
    if(to_find == "blank"){
        return true;
    }
    for(auto it=vec.begin();it!=vec.end();it++){
        if(*it == to_find){
            return true;
        }
    }
    return false;
}

bool Terran::findInsideUpgrade(const std::vector<std::string> &vec, const std::string to_find)
{
    if(to_find == "blank"){
        return true;
    }
    for(auto it=vec.begin();it!=vec.end();it++){
        if(*it == to_find){
            return true;
        }
    }
    
    auto it1 = morphs_database.find(to_find);
    if(it1 != morphs_database.end()){
        for(auto it2=it1->second.begin();it2 != it1->second.end();it2++){
            if(findInside(vec,*it2))
                return true;
        }
    }
    return false;
}














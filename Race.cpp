#include "Race.h"

Race::Race(double minerals, double vespene, int num_workers, int num_main)
{
	
	this->minerals = minerals;
	this->vespene = vespene;

	//All other supply updates are done inside either buildNext() or processQueues()
	rate_energy = 0.565;
	rate_minerals = 0.7;
	rate_vespene = 0.35;

	// Initialise the map for countbusy workers 
	count_busy_workers.insert(std::pair<std::string,int>("minerals",0));
	count_busy_workers.insert(std::pair<std::string,int>("vespene",0));
}

//Counts the number of units of the type called name
int Race::countUnits(const std::string name)
{
	return count_units[name];
}

//Counts the number of buildings of the type called name
int Race::countBuildings(const std::string name)
{
	return count_buildings[name];
}

// Count the number of free units , which are not assigned ny tasks 
int Race::countFreeUnits(const std::string name)
{
	int free_count = 0;
	for(auto it=units.begin();it != units.end(); ++it)
	{
		if(it->name == name && !(it->busy))
		{
			free_count++;
		}
	}
	return free_count;
}

// Count the number of free buildings which are not busy 
int Race::countFreeBuildings(const std::string building_name)
{
	//std::cout<<"building name is "<<building_name<<" ...."<<std::endl;
	int free_count = 0;
	for(auto it=buildings.begin();it != buildings.end(); ++it)
	{
		if(it->name == building_name && !(it->busy)) 
		{
			free_count++;
		}
	}
	return free_count;	
}

// Function to update resources
void Race::updateResources()
{
	mineralExtract();
	vespeneExtract();
	energyUpdate();

	//std::cout<<"RESOURCE STATUS...."<<minerals<<" "<<vespene<<" "<<supply<<std::endl;

}

// Mineral Extraction 
void Race::mineralExtract()
{
	minerals += count_busy_workers["minerals"]*rate_minerals;
}

// Vespene Extraction 
void Race::vespeneExtract()
{
	vespene += count_busy_workers["vespene"]*rate_vespene;
}

// This functionality is only for terran because in that units get busy unitll building comes into existence //
void Race::setBusy(const int producer_rank, bool what){ // busy function to engage units with buildings and engage buildings with units //
	if(what){
		auto it = buildings.begin()+producer_rank;
		it->load++;
		if(it->load == it->max_load){
			it->busy = true;
		}
	}
	else{
		auto it = units.begin()+producer_rank;
		it->busy = true;
		it->busy_with_building = true;
	}
}

void Race::setFree(const int producer_rank, bool what){
	if(what){
		auto it = buildings.begin()+producer_rank;
		it->busy = false;
	}
	else{
		auto it = units.begin()+producer_rank;
		it->busy = false;
		it->busy_with_building = false;
	}
}

// This is redundant function //
void Race::printDatabase()
{
	std::cout<<".....Printing the database received from techtress.csv....."<<std::endl;

	std::cout<<"...Printing Buildings..."<<std::endl;

	for(auto it=database_buildings.begin();it != database_buildings.end();it++)
	{
		std::cout<<it->second.name<<std::endl;
	}

	std::cout<<"...Printing Units..."<<std::endl;
	
	for(auto it=database_units.begin();it != database_units.end();it++)
	{
		std::cout<<it->second.name<<std::endl;
	}
	
}

bool Race::findUnit(const std::string name_check)
{
	if(countUnits(name_check) > 0)
	{
		return true;
	}
	else
    {
		return false;
	}
}

bool Race::isBuilding(const std::string name_check)
{
	auto it = database_buildings.find(name_check);
	if(it != database_buildings.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Race::isUnit(const std::string name_check)
{
	auto it = database_units.find(name_check);
	if(it != database_units.end())
	{
		return true;
	}
	else{
		return false;
	}
}

// findbuilding was for dependency //
bool Race::findBuilding(const std::string name_check)
{
	if(name_check == "blank"){
		return true;
	}

	//std::cout<<count_buildings["nexus"]<<std::endl;
	//std::cout<<"Building name is "<<name_check<<" and "<<countBuildings(name_check)<<std::endl;
	if(countBuildings(name_check) > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Race::findFreeBuilding(const std::string building_name) // add const 
{
	auto it=buildings.begin();

	bool found = false;
	int producer_rank = 0;
	while(!found && it != buildings.end())
	{
		if(it->busy == false && it->name == building_name)
		{
			found = true;
		}
		else
        {
			it++;
			producer_rank++; // better to use prefix // we can use find_if ..//
		}
	}
    return producer_rank; // maybe there is a bug ...if iterator reaches end  of the vector and it does not find the desired result  , what will be the producer rank  , according to me there should be another loop to return producer rank depending on the consditions//
}

//Should only be called inside buildNext() I ave problem in this function //
int Race::findFreeUnit(std::string unit_name){
	auto it=units.begin();

	bool found = false;
	int producer_rank = 0;
	while(!found && it != units.end())
	{
		if(it->busy_with_mineral == true && it->name == unit_name){
			found = true;
			count_busy_workers["minerals"]--;
			return producer_rank;
		}
		/*else if(it->busy_with_vespene == true && it->name == unit_name){
			found = true;
			count_busy_workers["vespene"]--;
			return producer_rank;
		}*/
		else{
			it++;
			producer_rank++;
		}
	}
	return -1;	
}

int Race::supplyUsed(){
    int value=0;
    for (auto it = units.begin(); it!=units.end(); ++it) {
        
        for (auto it1 = database_units.begin(); it1!= database_units.end(); ++it1) {
            if (it->name == it1->first) {
                value+=it1->second.supply_cost;
            }
        }
    }
    
    for (auto it = buildings.begin(); it!=buildings.end(); ++it) {
        
        for (auto it1 = database_buildings.begin(); it1!= database_buildings.end(); ++it1) {
            if (it->name == it1->first) {
                value+=it1->second.supply_cost;
            }
        }
    }
    
    return value;

}



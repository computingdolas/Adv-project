//Abstract Base  class from which both Terran and Protoss will be derived
#pragma once

// header files
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <boost/algorithm/string.hpp>
#include "Building.h"
#include "BuildingData.h"
#include "Unit.h"

using namespace std;
using namespace boost::algorithm;

// class definition 

class Race
{

public:
	std::string race_name;
    
	// Parameter declaration and Initial Condition
	double minerals =0.0;
	double vespene =0.0;
	int supply =0;
	double energy=0.0;
	int global_time = 1;
	double rate_energy=0.0,rate_minerals=0.0,rate_vespene = 0.0;

	// Vector Containers to store all the buildings/units that exist currently
	std::vector<Building> buildings;
	std::vector<Unit> units;

	//Maps to store the count of units/buildings. Key is the name of the respective unit/building.
	std::map<std::string,int> count_units;
	std::map<std::string,int> count_buildings;

	//Maps to store the count of probes busy with resources/buildings
	std::map<std::string,int> count_busy_workers;

	//Map containing all the data read from techtrees.csv 
	std::map<std::string,BuildingData> database_units;
	std::map<std::string,BuildingData> database_buildings;

	std::vector<std::pair<Unit,int>> units_queue; // why int ??

	// Constructor for the Race Class
	Race(double minerals,double vespene, int num_workers, int num_main);

    virtual ~Race(){

	}

	int countBuildings(const std::string name); // Function to Calculate the number of the buildings with particular name

	int countUnits(const std::string name); // function to calculate number of units with particular name

	int countFreeUnits(const std::string name); // Function to calculate the number of units for which busy flag is false

	int countFreeBuildings(const std::string name); //Function to calculate the number of buildings for which busy flag is false

	virtual void workerAssignment() = 0; // worker assignment strategy

	virtual bool buildNext(const std::string name) = 0; // buildnext strategy

	virtual void processQueues() = 0; // function to process queues of buildings being made 

	void updateResources();

	virtual void energyUpdate() = 0;

	//Check all units busy with extraction and deduct

	void mineralExtract();

	void vespeneExtract();

	void printDatabase();
	
	//Utility functions
    
	bool findBuilding(std::string name_check);

	bool findUnit(std::string name_check);

	bool isBuilding(const std::string name_check);

	bool isUnit(const std::string name_check);

	int findFreeBuilding(std::string building_name);

	int findFreeUnit(std::string unit_name);

	void setFree(const int producer_rank, bool what);

	void setBusy(const int producer_rank, bool what);
    
    virtual bool isBuildListValid(const std::vector<std::string> &build_list) =0;
    
    int supplyUsed();

};
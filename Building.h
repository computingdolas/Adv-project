#pragma once 
#include <iostream>
#include <string>

class Building
{
public:
	
	// properties  
    std::string name; // name of the building ...
	std::string race_name; // race it belong's to .... //
	
	int load; // current load of the particular building unit ...
	int max_load; // maximum load it can bear ...

	bool busy = false ; // busy flag ..

	//On hold-not known why
	bool limit = false ; // flag to point if ...the unit is busy or not

	int time_left; // time left to ....
	//methods

	Building(const std::string building_name, const std::string race,int time){
		
        time_left = time;
        name = building_name;
		race_name = race;
        busy = 	false ;
		limit = false ;
        load = 0;
		max_load = 1;
	}

	~Building(){} // Destructor ...s
};
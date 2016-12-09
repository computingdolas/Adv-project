#pragma once 
#include <iostream>
#include <string>

// class definition 
class Unit
{

public:
	//properties of the particular unit ....
	std::string name;
	bool busy;
    
    // variables to denote busyness of the particular unit with ....mineral, vespene..//
	bool busy_with_mineral;
	bool busy_with_vespene;
	bool busy_with_building;

	// timeleft for the particular unit to come into world  ....
    int time_left;
    
    // Constructor for Creation of the unit 
    Unit(const std::string unit_name, int time){ // we could have used the references to string for the argument passing
		
        name = unit_name;
		time_left = time;
		busy=false;
		busy_with_building = false;
		busy_with_mineral = false ;
	 	busy_with_vespene = false ;

	}
	~Unit(){} // Destructor

};
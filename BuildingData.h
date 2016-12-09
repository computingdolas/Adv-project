/*This file has been written as part of forward simulation of Star Craft 2 game
The file contains a class Building whose variables are the arrtibutes of csv file to be read in hashmap.cpp

Author : Sagar Dolas
Last Modified : 27th Dec 2015 by Raju Ram
Version : 1.1
*/

#pragma once 

#include <iostream>
#include <string>

class BuildingData
{
public:
	
    // variables..denote the properties..of the particular building system ....
	std::string race;
	std::string name;
    
    //
    double minerals=0.0;
	double vespene=0.0;
	int build_time=0;
	int supply_cost=0;
	int supply_provided=0;
	int start_energy=0.0;
	int max_energy=0.0;
	
	std::string produced_by;
	std::string dependency;

	/* The follwing 2 boolean variables has not been used
	bool produced= false;
	bool busy = false ;  */

	//Constructor ....default ...and that's why variables are value initialised to their particular type 
	BuildingData() = default;
	
};

// Includes

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
#include "Race.h"

using namespace std;
using namespace boost::algorithm;

// class definition 

class Protoss : public Race
{

public:

	// Queue of the buildings to be which are being made //
    std::vector<Building> buildings_queue;
    
	// Constructor
	Protoss(double minerals,double vespene, int num_workers, int num_main);
	
	//Destructor
	~Protoss(){}

    void fileRead(const std::string filename); // Function to read data from the excel file

	virtual void workerAssignment() override ; // worker assignment strategy

	virtual bool buildNext(const std::string name) override ; // buildnext function
    
	virtual void energyUpdate() override ; // Updating the enery //

	virtual void processQueues() override ; // Processing the queues unit and building
    
    virtual bool isBuildListValid(const std::vector<std::string> &build_list) override; // to check for the validity of the particular buildlist 
};

#include"Race.h"

#include<tuple>

class Terran : public Race
{

public:
	std::vector<std::pair<Building,int>> buildings_queue; 

	//Map stores which morphs or add-ons are possible
	std::map<std::string,std::vector<std::string>> morphs_database;

	//Vector of ints storing the ranks of the buildings currently being morphed
	std::vector<std::tuple<int,int,std::string>> morphs_queue;

	// Constructor
	Terran(double minerals,double vespene, int num_workers, int num_main);

	~Terran(){

	}

	void fileRead(const std::string filename);

	virtual void workerAssignment() override;

	virtual bool buildNext(const std::string name) override;

	virtual void energyUpdate() override;

	virtual void processQueues() override;
    
	bool findBuildingUpgrade(std::string building_name);

	int findFreeBuildingUpgrade(std::string building_name);
	
	int countBuildingsUpgrade(std::string building_name);

	int countFreeBuildingsUpgrade(std::string building_name);
    
    bool findInside(const std::vector<std::string> &vec, const std::string to_find);
    
    bool findInsideUpgrade(const std::vector<std::string> &vec, const std::string to_find);
    
    virtual bool isBuildListValid(const std::vector<std::string> &build_list) override;

    
};
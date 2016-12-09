#include"Protoss.h"
#include"Terran.h"
#include<algorithm>
#include<random>
#include <map>

class Optimizer{
public:

	Protoss sim;
    std::vector<std::string> build_possible_void_ray;
    std::vector<std::string> build_possible_colossus;
    
    std::string buildname ;
    int generationnum_;

	std::vector<std::vector<std::string>> individuals;
    std::vector<std::vector<std::string>> fitgeneration;
    std::multimap<int,std::vector<std::string>> sortedindividuals_;
    std::vector<std::string> init_individuals1; //A list with only 1 assimilator
	std::vector<std::string> init_individuals2; //A list with 2 assimilators
    
    // Constructor
	Optimizer(std::string buildname_,std::string filename_);
    
    // Creating the individual push ....//
    void createIndividualsPush();
    
    //
    void randomizeIndividualsPush(int numlists_,int listlength_);
    void reset();
	int runSimulation(std::vector<std::string> build_list, int time_steps,std::string goal_name);
    void fitBuildlist();
    void generationSimulate();

};
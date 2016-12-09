
#include "Optimizer.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <iterator>

void PrintBuildList(std::vector<std::string> buildlist){
    for(auto it=buildlist.begin();it!=buildlist.end();it++){
        std::cout<<*it<<std::endl;
    }
    std::cout<<std::endl;
}

void validTime(const int time , std::vector<int> &timedist){
    if (time> 0) {
        timedist.push_back(time);
    }
}

int main(){
    
    Optimizer pusher("void_ray","techtrees.csv");
    int numlists = 1000;
    int min ;
    int numgeneration = 5;
    int count1 = 0;
    
    std::vector<int> timedistribution_;
    int listlength = 15;
    pusher.createIndividualsPush();
    while (count1 < numgeneration ) {
        
        pusher.randomizeIndividualsPush(numlists,listlength);
        std::cout<<"The size of the individuals is :="<<pusher.individuals.size()<<std::endl;
        // merge previous best candidates from new gen
        std::move(pusher.fitgeneration.begin(), pusher.fitgeneration.end(), std::back_inserter(pusher.individuals));
        pusher.fitgeneration.clear();
        std::cout<<"The size of the individuals is :="<<pusher.individuals.size()<<std::endl;
        int count = 0,ret;
        while(count < 1000){
            pusher.reset();
            ret = pusher.runSimulation(pusher.individuals[count],1000,"void_ray");
            pusher.sortedindividuals_.insert(std::make_pair(ret,pusher.individuals[count]));
            validTime(ret,timedistribution_);
            ++count;
        }
        
        min = *std::min_element(timedistribution_.begin(),timedistribution_.end());
        std::cout<<"The minimun is:="<<min<<std::endl;
        pusher.fitBuildlist();
        if(count1 ==0)
            numlists-= pusher.fitgeneration.size();
        std::cout<<"The size of the fitgeneration is :="<<pusher.fitgeneration.size()<<std::endl;
        ++pusher.generationnum_;
        pusher.individuals.clear();
        pusher.sortedindividuals_.clear();
        ++count1;
    
    }
}
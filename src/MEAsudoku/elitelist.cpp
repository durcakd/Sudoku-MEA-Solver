/*
* Bakalarsky projekt
* Využitie multiagentového evolučného algoritmu v probléme z umelej inteligencie
* FIIT-5212-5799
* subor: Elite_list.cpp
* autor: Dávid Durčák,  ič:5799
* datum: 13.05.2013
*/


#include "MEAsudoku/elitelist.h"
#include <QString>
#include <QDebug>


// Constructor & Destructor
EliteList::EliteList(){
	fitnessSum = 0;
}

EliteList::~EliteList(){
	std::vector< std::pair< int, int*>  >::iterator  it;
	for(it = list.begin(); it != list.end(); ++it)
		delete it->second;
	list.clear();
}
void EliteList:: setParameters(int nparElitelistSize){
	 parElitelistSize = nparElitelistSize;
}


// push state to Elit list, (deleting the worst state, if new is better)
// states are sorted
int EliteList:: pushState(int fitness, int *state){

	if(list.size() < (unsigned) parElitelistSize){
		list.push_back(std::make_pair(fitness, state));
		fitnessSum += fitness;
		std::sort(list.begin(), list.end(), compare());
	}else{

		if(fitness <= list[0].first){
			fitnessSum = fitnessSum - list[0].first + fitness;

			delete list[0].second;
			list[0].first  = fitness;
			list[0].second = state;
			std::sort(list.begin(), list.end(), compare());
		}else{
			delete state ;
		}
	}
	return 0;
}

// get random state
STATE EliteList:: getRandomState() const {
	int generateNumber;
	STATE randomState;

	if(list.empty()){
		qWarning() << "Warning: EliteList:: getRandomState(): list is empty";
		randomState.state = NULL;
		return randomState;
	}
	//generateNumber = rand() % list.size();
	generateNumber = (int) (list.size() * (rand() / (RAND_MAX + 1.0)));

	randomState.fitness = list[generateNumber].first;
	randomState.state = list[generateNumber].second;
	return randomState;
}


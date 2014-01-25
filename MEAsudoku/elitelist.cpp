/*
* Bakalarsky projekt
* Využitie multiagentového evolučného algoritmu v probléme z umelej inteligencie
* FIIT-5212-5799
* subor: Elite_list.cpp
* autor: Dávid Durčák,  ič:5799
* datum: 13.05.2013
*/


#include "elitelist.h"
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
	QString outstr;

	if(list.size() < (unsigned) parElitelistSize){
		list.push_back(std::make_pair(fitness, state));
		fitnessSum += fitness;
		std::sort(list.begin(), list.end(), compare());
	}else{

		if(fitness <= list[0].first){
			//outstr.Format( "elitelist.pustState(), delete state %3d, %p",list[0].first, list[0].second    ); CLogger::Instance()->write(outstr);
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
STATE EliteList:: getRandomState(){
	int generateNumber;
	STATE randomState;
	//CString outstr;

	if(list.empty()){
		qWarning() << "Warning: EliteList:: getRandomState(): list is empty";
		randomState.state = NULL;
		return randomState;
	}
	//generateNumber = rand() % list.size();
	generateNumber = (int) (list.size() * (rand() / (RAND_MAX + 1.0)));

	randomState.fitness = list[generateNumber].first;
	randomState.state = list[generateNumber].second;
	//outstr.Format( "EliteList:: getRandomState() will return %2d.state %3d %p", generateNumber,randomState.fitness, randomState.state   ); CLogger::Instance()->write(outstr);
	return randomState;
}


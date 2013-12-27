/*
* Bakalarsky projekt
* Využitie multiagentového evolučného algoritmu v probléme z umelej inteligencie
* FIIT-5212-5799
* subor: Elite_list.cpp
* autor: Dávid Durčák,  ič:5799
* datum: 13.05.2013
*/


#include "elitelist.h"


// Constructor & Destructor
EliteList::EliteList(){
	fitnessSum = 0;
}
EliteList::EliteList(int nparElitelistSize){
	parElitelistSize = nparElitelistSize;
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
	CString outstr;

	if(list.size() < parElitelistSize){
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
		CLogger::Instance()->write("Warning: EliteList:: getRandomState(): list is empty");
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

// get state using Rulet selection
STATE EliteList:: getRuletState(){
	int generateNumber;
	STATE randomState;
	//CString outstr;


	if(list.empty()){
		CLogger::Instance()->write("Warning: EliteList:: getRandomState(): list is empty");
		randomState.state = NULL;
		return randomState;
	}


	int i = list.size() + 1,
	criterion = 1,
	lowerBound = ((i*(i-1))/2),
	upperBound = 0;

	generateNumber = 1 + (int)(lowerBound * (rand() / (RAND_MAX + 1.0)));

	while(i > 1 && criterion){
		i--;
		upperBound = lowerBound;
		lowerBound += upperBound - i;
		if((lowerBound < generateNumber) && (generateNumber <= upperBound ))
			criterion = 0;
	}

	randomState.fitness = list[i-1].first;
	randomState.state = list[i-1].second;

	return randomState;

}

// get state using tournament selection
STATE EliteList:: getTournamentState(){
	int generateNumber1;
	int generateNumber2;
	STATE randomState;
	//CString outstr;

	if(list.empty()){
		CLogger::Instance()->write("Warning: EliteList:: getRandomState(): list is empty");
		randomState.state = NULL;
		return randomState;
	}
	//generateNumber = rand() % list.size();
	generateNumber1 = (int) (list.size() * (rand() / (RAND_MAX + 1.0)));
	generateNumber2 = (int) (list.size() * (rand() / (RAND_MAX + 1.0)));
	if( list[generateNumber1].first >= list[generateNumber2].first){
		randomState.fitness = list[generateNumber1].first;
		randomState.state = list[generateNumber1].second;
	}else{
		randomState.fitness = list[generateNumber2].first;
		randomState.state = list[generateNumber2].second;
	}
	//outstr.Format( "EliteList:: getRandomState() will return %2d.state %3d %p", generateNumber,randomState.fitness, randomState.state   ); CLogger::Instance()->write(outstr);
	return randomState;
}


// aux methods & getters
std::vector< std::pair< int, int*>  > EliteList:: getList(){
	return list;
}

int EliteList:: getNumberOfEliteStates(){
	return list.size();
}
int EliteList:: getBestFitness(){
	if(list.empty()){
		return -1;
	}
	return list.back().first;
}
int EliteList:: getMeanFitness(){
	if(list.empty()){
		return -1;
	}
	return fitnessSum / list.size();
}
void EliteList:: printEliteList(){
	std::vector< std::pair< int, int*>  >::iterator  it;
	CString outstr;

	if(list.empty()){
		CLogger::Instance()->write("Warning: Elite List is empty");
	}else{
		outstr.Format("Elite List %d (%d) :", list.size(), parElitelistSize );
		CLogger::Instance()->write(outstr);
		for(it = list.begin(); it != list.end(); ++it){
			outstr.Format("  %3d    %p", it->first, it->second   );
			CLogger::Instance()->write(outstr);
		}
	}
}

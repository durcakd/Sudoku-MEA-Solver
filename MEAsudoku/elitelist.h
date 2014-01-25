/*
* Bakalarsky projekt
* Využitie multiagentového evolučného algoritmu v probléme z umelej inteligencie
* FIIT-5212-5799
* subor: Elite_list.h
* autor: Dávid Durčák,  ič:5799
* datum: 13.05.2013
*/

#ifndef ELITELIST_H
#define ELITELIST_H

#include <queue>
#include <algorithm>


// compare methods
struct compare
{
  bool operator()(const std::pair< int, int*>& left, const std::pair< int, int*>& right)
  {
	  return left.first > right.first ;  }
};



typedef struct structState{
	int fitness;
	int *state;
}STATE;

/*
* Class EliteList represent Elite list data structure, which is used by MEA Sudoku algorithm.
  It mainly provide methods to create EL,  push & get state from it .
* autor: David Durcak
*/
class EliteList
{
private:
	int parElitelistSize;
	int fitnessSum;
	//std::vector< std::pair< int, int*>  >  list;
	//std::priority_queue< std::pair< int, int*> , std::vector< std::pair< int, int*> > ,  compare> list;
	std::vector< std::pair< int, int*>  >  list;

public:
	// Constructor & Destructor
	EliteList();
	~EliteList();
	void setParameters(int nparElitelistSize);

	// push state to Elit list, (deleting the worst state, if new is better)
	// states are sorted
	int pushState(int fitness, int *state);
	// get random state
	STATE getRandomState();
	// get state using Rulet selection

};


#endif // ELITELIST_H

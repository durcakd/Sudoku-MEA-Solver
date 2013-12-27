/*
* Bakalarsky projekt
* Využitie multiagentového evolučného algoritmu v probléme z umelej inteligencie
* FIIT-5212-5799
* subor: MEA.h
* autor: Dávid Durčák,  ič:5799
* datum: 13.05.2013
*/

#ifndef MEA_H
#define MEA_H


#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <list>
#include "agentsudoku.h"
#include "elitelist.h"


/*
* Class MEA represent implemetation of MEA Sudoku algorithm.
  Main method is optimaze(), whitch is MEA algorithm, and this methods use other implemented methods .
  Agents are represented by list of AgentSudoku objects
  Elite_list object represent elite list data structure.
* autor: David Durcak
*/
class MEA
{
private:
	EliteList eliteList;   // elite list
	//int *fixedState;
	//std::list< AgentSudoku * >  agents;

	int counterAgents;
	static int counterTrial;


	// Parameters
	double	parMutProbability; ///
	int		parNumAgents,
			parMaxGenrations,
			parMaxTrials,
			parStartLifePoints,
			parBirthStep,
			parMilestoneStep,
			parElitelistSize; //// ??
	char *sudokuFile;


	int *tabuList;
	int *fixedState;
	int *fixedLists;

	bool testMode;

public:
	std::list< AgentSudoku * >  agents;    // list of agents




	// Constructor & Destructor
	MEA();
	~MEA();
	void setParameters(double nmutProbability, int nnumAgents,int nmaxGenrations,int nmaxTrials,
							int	nlifePoints,int	nbirthStep,int nmilestoneStep,
							int nelitelistSize, char *nsudokuFile, bool ntestMode);



	// main MEA optimalization method ( function of MEA)
	int optimize();
	// start initialization, load & prepare data
	int initialization();
	// generate initial ppopulation of agents
	int generateAgents();
	// chceck if solution was found
	int *controlFitness();
	// decrease of lifespan and elimination of agents, that don't have any life points
	// (push bestmilestone state to Elit List)
	int decLifePointsAndEliminate();
	// birth new agent(get state from elit list)
	int birthNewAgent();
	//   local search
	int localSearch(int generation);

	// aux methods

	// aux print agents information
	void printAgents();
	// statistics information about all agents
	void printBestMeanFitAndAgentsSize();
	// testing metod
	void testEliteList();

	static void addCounterTrials();

};


#endif // MEA_H

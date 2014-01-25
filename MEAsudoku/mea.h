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

#include <QStringList>

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
public:

	// Constructor & Destructor

	MEA();
	~MEA();

	void	setParameters( const int* givenData, int nnumAgents,int nmaxGenrations,int nmaxTrials,
						   int	nlifePoints,int	nbirthStep,int nmilestoneStep,
						   int nelitelistSize);

	// main MEA optimalization method ( function of MEA)
	int		optimize();

	static	void	addCounterTrials()		{ counterTrial++; }

	// aux methods
	QStringList		printState( const int *state ) const;


private:
	// start initialization, load & prepare data
	int		initialization();
	// generate initial ppopulation of agents
	int		generateAgents();
	// chceck if solution was found
	const	int		*controlFitness() const;
	// decrease of lifespan and elimination of agents, that don't have any life points
	// (push bestmilestone state to Elit List)
	int		decLifePointsAndEliminate();
	// birth new agent(get state from elit list)
	int		birthNewAgent();
	//   local search
	int		localSearch( int generation ) const;

private:

	EliteList	eliteList;   // elite list
	std::list< AgentSudoku * >  agents;    // list of agents

	static int	counterTrial;
	int			counterAgents;

	int		*tabuList;
	int		*fixedState;
	int		*fixedLists;

	// Parameters
	int		parNumAgents;
	int		parMaxGenrations;
	int		parMaxTrials;
	int		parStartLifePoints;
	int		parBirthStep;
	int		parMilestoneStep;
	int		parElitelistSize; //// ??

	};
#endif // MEA_H

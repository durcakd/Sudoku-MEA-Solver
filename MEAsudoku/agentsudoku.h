/*
* Bakalarsky projekt
* Využitie multiagentového evolučného algoritmu v probléme z umelej inteligencie
* FIIT-5212-5799
* subor: AgentSudoku.h
* autor: Dávid Durčák,  ič:5799
* datum: 13.05.2013
*/

#ifndef AGENTSUDOKU_H
#define AGENTSUDOKU_H


#include <cstdio>
#include "constants.h"


/*
* Class AgentSudoku represent implemetation of Sudoku agent, which is used by MEA Sudoku algorithm.
  Its provide all needed methods(generate new state, mutation, fitness function , local search methods)
  for searching in states space of Sudoku agent.
* autor: David Durcak
*/

class AgentSudoku
	{

public:
	//AgentSudoku();
	AgentSudoku(int newLifePoints, int *fixedState, int *nfixedLists, int nparMaxTrials, int *parTabuList);
	AgentSudoku(int newLifePoints, int *fixedState, int *nfixedLists, int* newCurrentState, int newCurrentFitness, int nparMaxTrials,  int *parTabuList);
	~AgentSudoku();

	static	QStringList	printState( const int *state);

	void	addLifePoints()					{ lifePoints++; }
	int		decLifePoints()					{ lifePoints--;
											  return lifePoints; }

	// CurrentState & fitness
	int		getCurrentFitness() const		{ return currentFitness; }
	int	   *getCurrentState()	const		{ return currentState; }
	void	setCurrentState(int *newState){ currentState = newState; }

	// BestMilestoneState
	int		getBestMilestoneFitness() const			{ return bestMilestoneFitness; }
	int	   *getBestMilestoneState()	  const			{ return bestMilestoneState; }
	void	setBestMilestoneState( int *newState)	{ bestMilestoneState = newState; }
	int		resetMilestoneState();

	// local search (edited hill climing algorithm)
	int		localSearchUseHeuristic();
	// generate new state for new agents
	int		generateNewState();


private:

	// fitness funcion
	static	int		fitnessFunctionAllOverState( const int *actualState, int *fitnessList);

	// mutation + heuristic + fast fitness function (via change)
	int		mutationUseHeurRetFitness( int *newState, const int *newFitnessList) const;

private:
	int	*fixed;
	int	*tabuList;
	int *fixedList;
	int *currentState;
	int *currentFitnessList;
	int *bestMilestoneState;
	int currentFitness;
	int bestMilestoneFitness;
	int lifePoints;

	int *mutState;
	int *mutFitnessList;
	int *localBestMutState;
	int *localBestMutFitnessList;


	// Parameters
	int parMaxTrials;

	};
#endif // AGENTSUDOKU_H

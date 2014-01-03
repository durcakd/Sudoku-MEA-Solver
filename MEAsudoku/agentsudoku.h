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


#define TABU				1
#define MAX_FITNESS			243
#define OPTIMAL_FITNESS		0
#define N					3
#define NN					9
#define	NN_NN				81
//#define MUTATION_PROBABILITY	0.2
#define OPTIMAL_FITNESS		0
#define USED				1
#define NOT_USED			0


/*
* Class AgentSudoku represent implemetation of Sudoku agent, which is used by MEA Sudoku algorithm.
  Its provide all needed methods(generate new state, mutation, fitness function , local search methods)
  for searching in states space of Sudoku agent.
* autor: David Durcak
*/

class AgentSudoku
{
private:
	int *fixed;
	int *tabuList;
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
	int name;

	// Parameters
	int parMaxTrials;

public:
	//AgentSudoku();
	AgentSudoku(int newLifePoints, int *fixedState, int *nfixedLists, int nparMaxTrials, int *parTabuList);
	AgentSudoku(int newLifePoints, int *fixedState, int *nfixedLists, int* newCurrentState, int newCurrentFitness, int nparMaxTrials,  int *parTabuList);
	~AgentSudoku();




	// CurrentState & fitness
	int  getCurrentFitness();
	void setCurrentFitness();
	void setCurrentFitness(int newFitness);

	int *getCurrentState();
	void setCurrentState(int *newState);
	static void printState(int *state);
	static void scprintState(int *state, int *fixed);


	//LifePoints
	void setLifePoints(int points);
	int getLifePoints();
	void addLifePoints();
	int decLifePoints();

	// BestMilestoneState
	int getBestMilestoneFitness();
	void setBestMilestoneFitness(int newFitness);
	int *getBestMilestoneState();
	void setBestMilestoneState(int *newState);
	void copyToBestMilestoneState(int *state);
	int  resetMilestoneState();


	int getName();
	void setName(int newName);

	// most importatnt metods

	// generate new state for new agents
	int generateNewState();
	// fitness funcion
	static int fitnessFunctionAllOverState(int *actualState, int *fitnessList);
	// mutation + heuristic + fast fitness function (via change)
	int	 mutationUseHeurRetFitness(int *newState, int *newFitnessList);
	// local search (edited hill climing algorithm)
	int	localSearchUseHeuristic();
	// 2. version of local search, not use in final version
	int localSearchUseFitList();


	// old methods (dont use some features (are slower, than new))

	/// int mutation(int *newState);
	/// int	 mutationReturnFitness(int *newState, int *newFitnessList);
	/// static int fitnessFunction(int *actualState);
	/// int localSearch();


};




#endif // AGENTSUDOKU_H

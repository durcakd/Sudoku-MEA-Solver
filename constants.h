#ifndef CONSTANTS_H
#define CONSTANTS_H

// sudoku size parameters
enum { N	= 3 ,
	   N2	= N*N ,
	   N4	= N2*N2 ,
	   COLS	= N2 ,
	   ROWS = N2 };

// MEA deafault parameters
enum { POPSIZE		= 80,		// start nums of agents
	   ELITESIZE	= 80,		// max size of elite list
	   LIFESPAN		= 300,		// life points of agent
	   BIRTHPERIOD		= 60,	// calls between birth
	   MILESTONEPERIOD	= 60,	// calls between best milestone reseting
	   LOCALTRIALS		= 2,	// max calls in local search
	   MAXCALLS			= 100000,	// max calls together
	   NUMTESTS			= 100 };	//


typedef struct parameters{
	int popSize;
	int elitSize;
	int lifespan;
	int birthPeriod;
	int milestonePeriod;
	int localTrials;
	int maxCalls;
	} PARAMETERS;


#endif // CONSTANTS_H

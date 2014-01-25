#ifndef CONSTANTS_H
#define CONSTANTS_H

// sudoku size parameters
enum { N	= 3 ,
	   N2	= N*N ,
	   N4	= N2*N2 ,
	   COLS	= N2 ,
	   ROWS = N2,

	   TABU				= 1,
	   MAX_FITNESS		= 243,
	   OPTIMAL_FITNESS	= 0,
	   USED				= 1,
	   NOT_USED			= 0


	 };

// MEA deafault parameters
enum { NUMTESTS			= 100,

	   EPOPSIZE			= 80,	// start nums of agents
	   EELITESIZE		= 80,	// max size of elite list
	   ELIFESPAN		= 300,	// life points of agent
	   EBIRTHPERIOD		= 60,	// calls between birth
	   EMILESTONEPERIOD	= 60,	// calls between best milestone reseting
	   ELOCALTRIALS		= 2,	// max calls in local search
	   EMAXCALLS		= 100000,	// max calls together

	   MPOPSIZE			= 150,
	   MELITESIZE		= 150,
	   MLIFESPAN		= 400,
	   MBIRTHPERIOD		= 60,
	   MMILESTONEPERIOD	= 60,
	   MLOCALTRIALS		= 3,
	   MMAXCALLS		= 500000,

	   HPOPSIZE			= 300,
	   HELITESIZE		= 300,
	   HLIFESPAN		= 500,
	   HBIRTHPERIOD		= 60,
	   HMILESTONEPERIOD	= 60,
	   HLOCALTRIALS		= 5,
	   HMAXCALLS		= 1000000

	 };	//


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

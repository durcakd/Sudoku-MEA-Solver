#ifndef CONSTANTS_H
#define CONSTANTS_H

// sudoku size parameters
enum { N	= 3 ,
	   N2	= N*N ,
	   N4	= N2*N2 ,
	   COLS	= N2 ,
	   ROWS = N2 };

// MEA deafault parameters
enum { NUMAGENTS		= 80,
	   MAXGENERATIONS	= 100000,
	   MAXTRIALS		= 2,
	   LIFEPOINTS		= 300,
	   BIRTHSTEP		= 60,
	   MILESTONESTEP	= 60,
	   ELITELISTSIZE	= 80,
	   NUMTESTS			= 100 };



#endif // CONSTANTS_H

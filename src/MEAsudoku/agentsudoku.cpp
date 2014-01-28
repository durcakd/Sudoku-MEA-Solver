/*
* Bakalarsky projekt
* Využitie multiagentového evolučného algoritmu v probléme z umelej inteligencie
* FIIT-5212-5799
* subor: agentsudoku.cpp
* autor: Dávid Durčák,  ič:5799
* datum: 13.05.2013
*/



#include <cstdlib>
#include <QString>
#include <QDebug>

#include "MEAsudoku/mea.h"
#include "MEAsudoku/agentsudoku.h"
#include "resultemitersigleton.h"


// Constructors & Destructors
AgentSudoku:: AgentSudoku(int newLifePoints, int *fixedState, int *nfixedLists, int nparMaxTrials,  int *parTabuList){
	tabuList			= parTabuList;
	fixed				= fixedState;
	fixedList			= nfixedLists;
	currentState		= new int[N4];
	bestMilestoneState	= new int[N4] ;
	mutState			= new int[N4] ;
	currentFitnessList  = new int[2 * N2];
	mutFitnessList		= new int[2 * N2];
	localBestMutState		= new int[N4] ;
	localBestMutFitnessList = new int[2 * N2];

	currentFitness			= MAX_FITNESS;
	bestMilestoneFitness	= MAX_FITNESS;
	lifePoints				= newLifePoints;
	parMaxTrials			= nparMaxTrials;
}
AgentSudoku:: AgentSudoku(int newLifePoints, int *fixedState, int *nfixedLists, int* newCurrentState, int newCurrentFitness, int nparMaxTrials, int *parTabuList){
	tabuList				= parTabuList;
	fixed					= fixedState;
	fixedList				= nfixedLists;
	bestMilestoneState		= new int[N4] ;
	mutState				= new int[N4] ;
	currentFitnessList		= new int[2 * N2];
	mutFitnessList			= new int[2 * N2];
	localBestMutState		= new int[N4] ;
	localBestMutFitnessList = new int[2 * N2];

	bestMilestoneFitness	= MAX_FITNESS;
	lifePoints				= newLifePoints;
	parMaxTrials			= nparMaxTrials;


	currentFitness			= newCurrentFitness;
	currentState			= new int[N4];

	memcpy(currentState, newCurrentState, N4 * sizeof(int));
	fitnessFunctionAllOverState(currentState, currentFitnessList);
}
AgentSudoku:: ~AgentSudoku(){
	delete currentState;
	delete bestMilestoneState;
	delete mutState;
	delete currentFitnessList;
	delete mutFitnessList;
	delete localBestMutState;
	delete localBestMutFitnessList;
}


// CurrentState & fitness

QStringList AgentSudoku:: printState( const int *state) {
	int i;
	QString outstr;
	QStringList list;

	if(NULL == state){
		qDebug() << "ERROR printState(): state is NULLL";
		return list;
	}

	for(i = 0; i < N4; ++i){
		// list
		list << QString::number(state[i]) ;
		// debug
		if(i % N == 0)
			outstr += "  ";
		if(i % N2 == 0){
			qDebug() << outstr;
			outstr = "";
		}
		if(i % (N2*3) == 0 && 0 != i) {
			qDebug() << " ";
		}
		outstr +=  QString::number(state[i]) ;
	}

	ResultEmiterSigleton::getInstance()->sentResultRequest(list);

	qDebug() << outstr;

	return list;

}


int  AgentSudoku:: resetMilestoneState(){

	if(currentFitness <= bestMilestoneFitness){
		bestMilestoneFitness = currentFitness;
		memcpy(bestMilestoneState, currentState, N4 * sizeof(int));
	}
	return 0;
}


// generate new state for new agents
int	 AgentSudoku:: generateNewState(){
	int subarray[N2];
	int i,
			offset,
			sequence,
			randomNumber;

	memcpy ( currentState, fixed, N4 * sizeof(int) );

	for(sequence = 0; sequence < N2; ++sequence){
		// set every ones to zero
		offset = sequence * N2;
		for(i = 0; i < N2; ++i)
			subarray[i] = NOT_USED;

		// znacenie ktore su uz pouzite
		for(i = 0; i < N2; ++i)
			if(0 != fixed[offset + i])
				subarray[ fixed[offset + i ] - 1] = USED;

		// postupne pre vsetky pozicie sa budu generovat cisla ktore este niesu pouzite
		for(i = 0; i < N2; ++i)
			if(NOT_USED == currentState[offset + i]){
				do{
					randomNumber = (int) (N2 * (rand() / (RAND_MAX + 1.0)));
				}while(USED == subarray[randomNumber] );
				currentState[offset + i] = randomNumber + 1;
				subarray[randomNumber] = USED;
			}
	}
	currentFitness = fitnessFunctionAllOverState(currentState, currentFitnessList);

	return 0;
}

// fitness funcion
int  AgentSudoku:: fitnessFunctionAllOverState(const int *actualState, int *fitnessList) {
	int existArray[N2];
	int fitness = 0,
			rowFitness,
			k,
			i,
			j,
			position,
			offset;


	// evaluate for column
	for(offset = 0; offset < N2; ++offset){
		// clear existArray
		rowFitness = 0;
		for(i = 0; i < N2; ++i)
			existArray[i] = NOT_USED;
		position = offset;
		for(i = 0; i < N2; ++i){
			if(existArray[ actualState[position] - 1 ])
				++rowFitness;
			else
				existArray[ actualState[position] - 1 ] = USED;
			position += N2;
		}
		fitnessList[offset] = rowFitness;
		fitness += rowFitness;

	}


	// evaluate for blocks
	for(k = 0; k < N; ++k){
		// clear existArray
		for(j = 0; j < N; ++j){
			offset = k * N2 * N  + j * N;
			rowFitness = 0;

			for(i = 0; i < N2; ++i)
				existArray[i] = NOT_USED;

			for(i = 0; i < N2; ++i){
				if(i == N)
					offset = offset + N + N;
				if(i == N + N)
					offset = offset + N + N;
				position = i  + offset;

				if(existArray[ actualState[position] - 1 ])
					++rowFitness;
				else
					existArray[ actualState[position] - 1 ] = USED;
			}
			fitnessList[k*N + j + N2] = rowFitness;
			fitness += rowFitness;

		}
	}
	return fitness;
}

// mutation + heuristic + fast fitness function (via change)
int	 AgentSudoku:: mutationUseHeurRetFitness(int *newState, const int *newFitnessList) const {          // mutation  MUT-5  only 1 row WITHOUT probability using fixedList, new fitness
	int i,
			position1,
			position2,
			offset,
			swap,
			counter,
			numConflicts,
			tabuCounter;
	int existArray[N2];

	memcpy(newState, currentState, N4 * sizeof(int));
	memcpy((void *)newFitnessList, currentFitnessList, 2*N2*sizeof(int));


	// random row with nofixed positions >= 2
	tabuCounter = 0;


	do{
		do{
			i = (int) (N2 * (rand() / (RAND_MAX + 1.0)));
			offset = i * N2;
			counter = fixedList[offset + i];
		}while(counter < 2);

		// random 2 not the same positions
		position1 = (int) (counter * (rand() / (RAND_MAX + 1.0)));
		do{
			position2 = (int) (counter * (rand() / (RAND_MAX + 1.0)));
		}while(position1 == position2);

		// get positions from fixedList
		position1 = fixedList[offset + i + position1 + 1];
		position2 = fixedList[offset + i + position2 + 1];

		// Tabu heuristics

		tabuCounter++;
		numConflicts = 0;

		if(TABU == tabuList[(position1 + offset)*N2 + (newState[position2 + offset] - 1) ] )
			numConflicts += 2;
		if(TABU == tabuList[(position2 + offset)*N2 + (newState[position1 + offset] - 1) ] )
			numConflicts += 2;

		if(TABU == tabuList[(position1 + offset)*N2 + (newState[position1 + offset] - 1) ] )
			numConflicts--;
		if(TABU == tabuList[(position2 + offset)*N2 + (newState[position2 + offset] - 1) ] )
			numConflicts--;


	}while( (0 < numConflicts  &&  4 > tabuCounter) || (1 < numConflicts  &&  8 > tabuCounter));



	// swap positions
	swap = newState[position1 + offset];
	newState[position1 + offset] = newState[position2 + offset];
	newState[position2 + offset] = swap;

	//  Fitness , evaluate only changes------------------------------------------
	int fc1, fc2, fb1, fb2, fit, k, j1, j2, c, pos, offs;

	if(3 > i) k = 0;
	else if(5 < i) k = 2;
	else k = 1;

	if(3 > position1) j1 = 0;
	else if(5 < position1) j1 = 2;
	else j1 = 1;

	if(3 > position2) j2 = 0;
	else if(5 < position2) j2 = 2;
	else j2 = 1;

	// column 1
	fit = 0;
	for(c = 0; c < N2; ++c)
		existArray[c] = NOT_USED;
	pos = position1;
	for(c = 0; c < N2; ++c){
		if(existArray[ newState[pos] - 1 ])
			++fit;
		else
			existArray[ newState[pos] - 1 ] = USED;
		pos += N2;
	}
	fc1 = fit - mutFitnessList[position1];
	mutFitnessList[position1] += fc1; ;

	// column 2
	fit = 0;
	for(c = 0; c < N2; ++c)
		existArray[c] = NOT_USED;
	pos = position2;
	for(c = 0; c < N2; ++c){
		if(existArray[ newState[pos] - 1 ])
			++fit;
		else
			existArray[ newState[pos] - 1 ] = USED;
		pos += N2;
	}
	fc2 = fit - mutFitnessList[position2];
	mutFitnessList[position2] += fc2; ;


	// block1
	offs = k * N2 * N  + j1 * N;
	fit = 0;
	for(c = 0; c < N2; ++c)
		existArray[c] = NOT_USED;

	for(c = 0; c < N2; ++c){
		if(c == N)
			offs = offs + N + N;
		if(c == N + N)
			offs = offs + N + N;
		pos = c  + offs;

		if(existArray[ newState[pos] - 1 ])
			++fit;
		else
			existArray[ newState[pos] - 1 ] = USED;
	}
	fb1 = fit - mutFitnessList[k*3 + j1 + N2];
	mutFitnessList[k*3 + j1 + N2] += fb1; ;

	// block2
	fb2 = 0;
	if(j1 != j2 ){
		offs = k * N2 * N  + j2 * N;
		fit = 0;

		for(c = 0; c < N2; ++c)
			existArray[c] = NOT_USED;

		for(c = 0; c < N2; ++c){
			if(c == N)
				offs = offs + N + N;
			if(c == N + N)
				offs = offs + N + N;
			pos = c  + offs;

			if(existArray[ newState[pos] - 1 ])
				++fit;
			else
				existArray[ newState[pos] - 1 ] = USED;
		}
		fb2 = fit - mutFitnessList[k*3 + j2 + N2];
		mutFitnessList[k*3 + j2 + N2] += fb2;
	}

	// fitness
	return currentFitness + fc1 + fc2 + fb1 + fb2;

}
// local search (edited hill climing algorithm)
int  AgentSudoku:: localSearchUseHeuristic(){
	int trial,
			mutFitness,
			localBestMutFitness,
			reward;
	int *swapState;

	reward = currentFitness;
	localBestMutFitness = MAX_FITNESS;


	for(trial = 0; trial < parMaxTrials; ++trial  ){
		mutFitness = mutationUseHeurRetFitness( mutState, mutFitnessList);
		MEA::	addCounterTrials();

		// preco memcopy?????  -> memcopy if parallelism
		if(mutFitness <= localBestMutFitness ){
			memcpy(localBestMutFitnessList, mutFitnessList, 2*N2*sizeof(int));
			localBestMutFitness = mutFitness;

			swapState = mutState;
			mutState = localBestMutState;
			localBestMutState = swapState;

		}

	}
	if(localBestMutFitness <= getCurrentFitness() ){
		memcpy(currentFitnessList, localBestMutFitnessList, 2*N2*sizeof(int));
		currentFitness = localBestMutFitness;

		swapState = localBestMutState;
		localBestMutState = currentState;
		currentState = swapState;
		reward = currentFitness + 1;

	}
	if(reward > currentFitness)
		return 1;
	else
		return 0;
}







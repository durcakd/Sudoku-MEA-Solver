/*
* Bakalarsky projekt
* Využitie multiagentového evolučného algoritmu v probléme z umelej inteligencie
* FIIT-5212-5799
* subor: agentsudoku.cpp
* autor: Dávid Durčák,  ič:5799
* datum: 13.05.2013
*/



#include <cstdio>
#include <cstdlib>
#include <QString>
#include <QDebug>

#include "mea.h"
#include "agentsudoku.h"
#include "resultemitersigleton.h"

// ================================================

// Constructors & Destructors
AgentSudoku:: AgentSudoku(int newLifePoints, int *fixedState, int *nfixedLists, int nparMaxTrials,  int *parTabuList){
	tabuList			= parTabuList;
	fixed				= fixedState;
	fixedList			= nfixedLists;
	currentState		= new int[NN_NN];
	bestMilestoneState	= new int[NN_NN] ;
	mutState			= new int[NN_NN] ;
	currentFitnessList  = new int[2 * NN];
	mutFitnessList		= new int[2 * NN];
	localBestMutState		= new int[NN_NN] ;
	localBestMutFitnessList = new int[2 * NN];

	currentFitness			= MAX_FITNESS;
	bestMilestoneFitness	= MAX_FITNESS;
	lifePoints				= newLifePoints;
	//parLifePoints			= newLifePoints;
	parMaxTrials			= nparMaxTrials;

	//swapState				= NULL;
}
AgentSudoku:: AgentSudoku(int newLifePoints, int *fixedState, int *nfixedLists, int* newCurrentState, int newCurrentFitness, int nparMaxTrials, int *parTabuList){
	tabuList				= parTabuList;
	fixed					= fixedState;
	fixedList				= nfixedLists;
	bestMilestoneState		= new int[NN_NN] ;
	mutState				= new int[NN_NN] ;
	currentFitnessList		= new int[2 * NN];
	mutFitnessList			= new int[2 * NN];
	localBestMutState		= new int[NN_NN] ;
	localBestMutFitnessList = new int[2 * NN];

	bestMilestoneFitness	= MAX_FITNESS;
	lifePoints				= newLifePoints;
	//parLifePoints			= newLifePoints;
	parMaxTrials			= nparMaxTrials;


	currentFitness			= newCurrentFitness;
	currentState			= new int[NN_NN];

	memcpy(currentState, newCurrentState, NN_NN * sizeof(int));
	fitnessFunctionAllOverState(currentState, currentFitnessList);
	//swapState				= NULL;
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
int  AgentSudoku:: getCurrentFitness(){
	return currentFitness;
}


int	*AgentSudoku:: getCurrentState(){
	return currentState;
}
void AgentSudoku:: setCurrentState(int *newState){
	currentState = newState;
}
QStringList AgentSudoku:: printState(int *state){
	int i;
	QString outstr;
	QStringList list;

	if(NULL == state){
		qDebug() << "ERROR printState(): state is NULLL";
		return list;
	}

	for(i = 0; i < NN*NN; ++i){
		// list
		list << QString::number(state[i]) ;
		// debug
		if(i % N == 0)
			outstr += "  ";
		if(i % NN == 0){
			qDebug() << outstr;
			outstr = "";
		}
		if(i % (NN*3) == 0 && 0 != i) {
			qDebug() << " ";
		}
		outstr +=  QString::number(state[i]) ;
	}

	ResultEmiterSigleton::getInstance()->sentResultRequest(list);

	qDebug() << outstr;

	return list;

}


void AgentSudoku:: addLifePoints(){
	lifePoints++;
}
int  AgentSudoku:: decLifePoints(){
	//CString outstr;
	//outstr.Format( "\nAS(%2d) declife %d -> %d",name, lifePoints, lifePoints - 1   );
	//CLogger::Instance()->write(outstr);
	lifePoints--;
	return lifePoints;
}

// BestMilestoneState
int  AgentSudoku:: getBestMilestoneFitness(){
	return bestMilestoneFitness;
}

int *AgentSudoku:: getBestMilestoneState(){
	return bestMilestoneState;
}
void AgentSudoku:: setBestMilestoneState(int *newState){
	bestMilestoneState = newState;
}

int  AgentSudoku:: resetMilestoneState(){

	if(currentFitness <= bestMilestoneFitness){
		bestMilestoneFitness = currentFitness;
		memcpy(bestMilestoneState, currentState, NN_NN * sizeof(int));

	}
	return 0;
}



// most importatnt metods

// generate new state for new agents
int	 AgentSudoku:: generateNewState(){
	int subarray[NN];
	int i,
			offset,
			sequence,
			randomNumber;

	// for every one subseqencie:
	/*if(NULL == currentState){
		currentState = new int[NN_NN];
	}*/

	memcpy ( currentState, fixed, NN_NN * sizeof(int) );

	for(sequence = 0; sequence < NN; ++sequence){
		// set every ones to zero
		offset = sequence * NN;
		for(i = 0; i < NN; ++i)
			subarray[i] = NOT_USED;

		// znacenie ktore su uz pouzite
		for(i = 0; i < NN; ++i)
			if(0 != fixed[offset + i])
				subarray[ fixed[offset + i ] - 1] = USED;

		// postupne pre vsetky pozicie sa budu generovat cisla ktore este niesu pouzite
		for(i = 0; i < NN; ++i)
			if(NOT_USED == currentState[offset + i]){
				do{
					randomNumber = (int) (NN * (rand() / (RAND_MAX + 1.0)));
					//randomNumber = rand() % NN;
				}while(USED == subarray[randomNumber] );
				currentState[offset + i] = randomNumber + 1;
				subarray[randomNumber] = USED;
			}
	}
	currentFitness = fitnessFunctionAllOverState(currentState, currentFitnessList);

	return 0;
}

// fitness funcion
int  AgentSudoku:: fitnessFunctionAllOverState(int *actualState, int *fitnessList){
	int existArray[NN];
	int fitness = 0,
			rowFitness,
			k,
			i,
			j,
			position,
			offset;


	// evaluate for column
	for(offset = 0; offset < NN; ++offset){
		// clear existArray
		rowFitness = 0;
		for(i = 0; i < NN; ++i)
			existArray[i] = NOT_USED;
		position = offset;
		for(i = 0; i < NN; ++i){
			if(existArray[ actualState[position] - 1 ])
				++rowFitness;
			else
				existArray[ actualState[position] - 1 ] = USED;
			position += NN;
		}
		fitnessList[offset] = rowFitness;
		fitness += rowFitness;

	}


	// evaluate for blocks
	for(k = 0; k < N; ++k){
		// clear existArray
		for(j = 0; j < N; ++j){
			offset = k * NN * N  + j * N;
			rowFitness = 0;

			for(i = 0; i < NN; ++i)
				existArray[i] = NOT_USED;

			for(i = 0; i < NN; ++i){
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
			fitnessList[k*N + j + NN] = rowFitness;
			fitness += rowFitness;

		}
	}
	return fitness;
}

// mutation + heuristic + fast fitness function (via change)
int	 AgentSudoku:: mutationUseHeurRetFitness(int *newState, int *newFitnessList){          // mutation  MUT-5  only 1 row WITHOUT probability using fixedList, new fitness
	int i,
			position1,
			position2,
			offset,
			swap,
			counter,
			numConflicts,
			tabuCounter;
	//QString outstr;
	int existArray[NN];
	//CString outstr;

	memcpy(newState, currentState, NN_NN*sizeof(int));
	memcpy(newFitnessList, currentFitnessList, 2*NN*sizeof(int));


	// random row with nofixed positions >= 2
	tabuCounter = 0;


	do{
		do{
			i = (int) (NN * (rand() / (RAND_MAX + 1.0)));
			//i = (rand() % NN);
			offset = i * NN;
			counter = fixedList[offset + i];
		}while(counter < 2);

		// random 2 not the same positions
		position1 = (int) (counter * (rand() / (RAND_MAX + 1.0)));
		//position1 = (rand() % counter);
		do{
			position2 = (int) (counter * (rand() / (RAND_MAX + 1.0)));
			//position2 = (rand() % counter);
		}while(position1 == position2);

		// get positions from fixedList
		position1 = fixedList[offset + i + position1 + 1];
		position2 = fixedList[offset + i + position2 + 1];

		// Tabu heuristics
		// if(TABU == tabuList[(i*NN + j)*NN + k])
		// k = newState[position1 + offset] - 1;
		// (i*NN + j)*NN = (position1 + offset)*NN


		tabuCounter++;
		numConflicts = 0;

		if(TABU == tabuList[(position1 + offset)*NN + (newState[position2 + offset] - 1) ] )
			numConflicts += 2;
		if(TABU == tabuList[(position2 + offset)*NN + (newState[position1 + offset] - 1) ] )
			numConflicts += 2;

		if(TABU == tabuList[(position1 + offset)*NN + (newState[position1 + offset] - 1) ] )
			numConflicts--;
		if(TABU == tabuList[(position2 + offset)*NN + (newState[position2 + offset] - 1) ] )
			numConflicts--;


		//outstr.Format("2>   %d  %d", tabuCounter, numConflicts ); CLogger::Instance()->write(outstr);
		//outstr.Format("3>   %d  %d", tabuCounter, numConflicts ); CLogger::Instance()->write(outstr);

	}while( (0 < numConflicts  &&  4 > tabuCounter) || (1 < numConflicts  &&  8 > tabuCounter));
	//}while( (1 < numConflicts  &&  10 > tabuCounter));





	// swap positions
	swap = newState[position1 + offset];
	newState[position1 + offset] = newState[position2 + offset];
	newState[position2 + offset] = swap;
	//outstr.Format("Mut %d ->   %d >< %d", i, position1, position2  ); CLogger::Instance()->write(outstr);

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
	for(c = 0; c < NN; ++c)
		existArray[c] = NOT_USED;
	pos = position1;
	for(c = 0; c < NN; ++c){
		if(existArray[ newState[pos] - 1 ])
			++fit;
		else
			existArray[ newState[pos] - 1 ] = USED;
		pos += NN;
	}
	fc1 = fit - mutFitnessList[position1];
	mutFitnessList[position1] += fc1; ;

	// column 2
	fit = 0;
	for(c = 0; c < NN; ++c)
		existArray[c] = NOT_USED;
	pos = position2;
	for(c = 0; c < NN; ++c){
		if(existArray[ newState[pos] - 1 ])
			++fit;
		else
			existArray[ newState[pos] - 1 ] = USED;
		pos += NN;
	}
	fc2 = fit - mutFitnessList[position2];
	mutFitnessList[position2] += fc2; ;
	//fitnessList[offset] = rowFitness;


	// block1
	offs = k * NN * N  + j1 * N;
	fit = 0;
	for(c = 0; c < NN; ++c)
		existArray[c] = NOT_USED;

	for(c = 0; c < NN; ++c){
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
	fb1 = fit - mutFitnessList[k*3 + j1 + NN];
	mutFitnessList[k*3 + j1 + NN] += fb1; ;

	// block2
	fb2 = 0;
	if(j1 != j2 ){
		//printf(" <> ");
		offs = k * NN * N  + j2 * N;
		fit = 0;

		for(c = 0; c < NN; ++c)
			existArray[c] = NOT_USED;

		for(c = 0; c < NN; ++c){
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
		fb2 = fit - mutFitnessList[k*3 + j2 + NN];
		mutFitnessList[k*3 + j2 + NN] += fb2;
	}

	// fitness
	//outstr.format(" %d  %d  %d  %d,.. %d  %d  %d  %d",currentFitnessList[position1]+fc1, currentFitnessList[position2]+fc2, currentFitnessList[k*3 + j1 + NN]+fb1,currentFitnessList[k*3 + j2 + NN] + fb2,fc1, fc2, fb1, fb2);
	//CLogger::Instance()->write(outstr);
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
	//maxTrials = 1 + (parLifePoints - lifePoints)*10/parLifePoints;
	for(trial = 0; trial < parMaxTrials; ++trial  ){
		//mutFitness = mutationReturnFitness( mutState, mutFitnessList);
		mutFitness = mutationUseHeurRetFitness( mutState, mutFitnessList);
		MEA::	addCounterTrials();
		/*if(mutFitness != fitnessFunction(mutState)){
				CLogger::Instance()->write("ERROR:: NEROVNA SA ");
				int rFitness, s;
					rFitness = 0;
					for(s = 0; s < 2*NN; s++ ){
						rFitness += currentFitnessList[s];
					}
				//mutFitness = AgentSudoku::fitnessFunction(mutState);
			}*/
		// preco memcopy?????  -> memcopy if parallelism

		if(mutFitness <= localBestMutFitness ){
			memcpy(localBestMutFitnessList, mutFitnessList, 2*NN*sizeof(int));
			localBestMutFitness = mutFitness;

			swapState = mutState;
			mutState = localBestMutState;
			localBestMutState = swapState;


			//memcpy(agent->getCurrentState(), mutState, NN_NN * sizeof(int));
			//reward = 1;
		}

	}
	if(localBestMutFitness <= getCurrentFitness() ){
		memcpy(currentFitnessList, localBestMutFitnessList, 2*NN*sizeof(int));
		currentFitness = localBestMutFitness;

		swapState = localBestMutState;
		localBestMutState = currentState;
		currentState = swapState;
		reward = currentFitness + 1;

		//memcpy(agent->getCurrentState(), mutState, NN_NN * sizeof(int));
		//reward = 1;
	}
	if(reward > currentFitness)
		return 1;
	else
		return 0;
}







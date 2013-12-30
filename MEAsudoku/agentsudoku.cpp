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


// ================================================

// Constructors & Destructors
AgentSudoku:: AgentSudoku(int newLifePoints, int *fixedState, int *nfixedLists, int nparMaxTrials, double nparMutProbability, int *parTabuList){
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
	parMutProbability		= nparMutProbability;

	//swapState				= NULL;
}
AgentSudoku:: AgentSudoku(int newLifePoints, int *fixedState, int *nfixedLists, int* newCurrentState, int newCurrentFitness, int nparMaxTrials, double nparMutProbability, int *parTabuList){
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
	parMutProbability		= nparMutProbability;


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
void AgentSudoku:: setCurrentFitness(){
	currentFitness = fitnessFunctionAllOverState(currentState, currentFitnessList);
}
void AgentSudoku:: setCurrentFitness(int newFitness){
	currentFitness = newFitness;
}

int	*AgentSudoku:: getCurrentState(){
	return currentState;
}
void AgentSudoku:: setCurrentState(int *newState){
	currentState = newState;
}
void AgentSudoku:: printState(int *state){
	int i;
	QString outstr;

	//CLogger::Instance()->write(" PRINT STATE-----------------------------);

	if(NULL == state){
		qWarning() << "ERROR printState(): state is NULLL";
		return;
	}

	for(i = 0; i < NN*NN; ++i){
		if(i % N == 0)
			outstr += "  ";
		if(i % NN == 0){
			qWarning() << outstr;
			outstr = "";
		}
		if(i % (NN*3) == 0 && 0 != i) {
			qWarning() << " ";
		}
		outstr +=  state[i] ;
	}
	qWarning() << outstr;
}

void AgentSudoku:: scprintState(int *state, int *fixed){
	int i;
	QString outstr = "";
	//HANDLE hConsole;

	//CLogger::Instance()->write(" PRINT STATE-----------------------------);
	if(NULL == state){
		qWarning() << "ERROR printState(): state is NULLL";
		return;
	}


	outstr = "";
	for(i = 0; i < NN*NN; ++i){
		if(i % 3 == 0 && 0 != i)
			outstr += "   ";
		if(i % NN == 0 && 0 != i ){
			qWarning() << outstr;
			outstr = "";
		}
		if(i % (NN*3) == 0 && 0 != i) {
			qWarning() << " ";
		}
		outstr += state[i] ;
		// color
		/*
		if(0 != fixed[i]){
			SetConsoleTextAttribute(hConsole, 8);
			printf(" %d", state[i]);
			SetConsoleTextAttribute(hConsole, 4);

		}else
			printf(" %d", state[i]);
			*/
	}
	qWarning() << outstr;
	//SetConsoleTextAttribute(hConsole, 7);

}

// LifePoints
void AgentSudoku:: setLifePoints(int points){
	lifePoints = points;
}
int  AgentSudoku:: getLifePoints(){
	return lifePoints;
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
void AgentSudoku:: setBestMilestoneFitness(int newFitness){
	bestMilestoneFitness = newFitness;
}
int *AgentSudoku:: getBestMilestoneState(){
	return bestMilestoneState;
}
void AgentSudoku:: setBestMilestoneState(int *newState){
	bestMilestoneState = newState;
}
void AgentSudoku:: copyToBestMilestoneState(int *state){

	memcpy(bestMilestoneState, state, NN_NN * sizeof(int));
}
int  AgentSudoku:: resetMilestoneState(){

	if(currentFitness <= bestMilestoneFitness){
		bestMilestoneFitness = currentFitness;
		memcpy(bestMilestoneState, currentState, NN_NN * sizeof(int));

	}
	return 0;
}

int  AgentSudoku:: getName(){
	return name;
}
void AgentSudoku:: setName(int newName){
	name = newName;
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


// 2. version of local search, not use in final version
int  AgentSudoku:: localSearchUseFitList(){
	int trial,
		mutFitness,
		reward;
	int *swapState;

	reward = currentFitness;
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
			if(mutFitness <= getCurrentFitness() ){
				memcpy(currentFitnessList, mutFitnessList, 2*NN*sizeof(int));

				reward = currentFitness + 1;

				currentFitness = mutFitness;

				swapState = mutState;
				mutState = currentState;
				currentState = swapState;



				//memcpy(agent->getCurrentState(), mutState, NN_NN * sizeof(int));
				//reward = 1;
			}
		}
	if(reward > currentFitness)
		return 1;
	else
		return 0;
}

/*
// old

int	 AgentSudoku:: mutation(int *newState)          // mutation  MUT-3  only 1 row WITHOUT probability using fixedList
{
	int i,
		position1,
		position2,
		offset,
		swap,
		counter;
	//CString outstr;

	memcpy(newState, currentState, NN_NN*sizeof(int));

	// random row with nofixed positions >= 2
	do{
		i = (rand() % NN);
		offset = i * NN;
		counter = fixedList[offset + i];
	}while(counter < 2);

	// random 2 not the same positions
	position1 = (rand() % counter);
	do{
		position2 = (rand() % counter);
	}while(position1 == position2);

	// get positions from fixedList
	position1 = fixedList[offset + i + position1 + 1];
	position2 = fixedList[offset + i + position2 + 1];

	// swap positions
	swap = newState[position1 + offset];
	newState[position1 + offset] = newState[position2 + offset];
	newState[position2 + offset] = swap;
	//outstr.Format("Mut %d ->   %d >< %d", i, position1, position2  );
	//CLogger::Instance()->write(outstr);


	return 0;
}
int  AgentSudoku:: fitnessFunction(int *actualState){
	int existArray[NN];
	int fitness = 0,
		k,
		i,
		j,
		position,
		offset;
	//CString outstr;

	// evaluate for column
	for(offset = 0; offset < NN; ++offset){
		// clear existArray
		for(i = 0; i < NN; ++i)
			existArray[i] = NOT_USED;
		position = offset;
		for(i = 0; i < NN; ++i){
			if(existArray[ actualState[position] - 1 ])
				++fitness;
			else
				existArray[ actualState[position] - 1 ] = USED;
			position += NN;
		}
		//outstr.Format( "fit %d", fitness    ); CLogger::Instance()->write(outstr);
	}


	// evaluate for blocks
	for(k = 0; k < N; ++k){
		// clear existArray
		for(j = 0; j < N; ++j){
			offset = k * NN * N  + j * N;


			for(i = 0; i < NN; ++i)
				existArray[i] = NOT_USED;

			for(i = 0; i < NN; ++i){
				if(i == N)
					offset = offset + N + N;
				if(i == N + N)
					offset = offset + N + N;
				//position = i  + offset;

				if(existArray[ actualState[i  + offset] - 1 ])
					++fitness;
				else
					existArray[ actualState[i  + offset] - 1 ] = USED;
			}
			//outstr.Format( "fit %d", fitness    ); CLogger::Instance()->write(outstr);
		}
	}

	return fitness;
}
int  AgentSudoku:: localSearch(){
	int trial,
		mutFitness,
		reward;
	int *swapState;

	reward = currentFitness;
	for(trial = 0; trial < parMaxTrials; ++trial  ){
			mutation(mutState);
			mutFitness = AgentSudoku::fitnessFunction(mutState);

			// preco memcopy?????  -> memcopy if parallelism
			if(mutFitness <= getCurrentFitness()){
				currentFitness = mutFitness;

				swapState = mutState;
				mutState = currentState;
				currentState = swapState;

				//memcpy(agent->getCurrentState(), mutState, NN_NN * sizeof(int));
				//reward = 1;
			}
		}
	if(reward > currentFitness)
		return 1;
	else
		return 0;
}
int	 AgentSudoku:: mutationReturnFitness(int *newState, int *newFitnessList){          // all row with probability using fixedList, new fitness
	int i,
		position1,
		position2,
		offset,
		swap,
		counter;
	int existArray[NN];
	//CString outstr;
	int fc1, fc2, fb1, fb2, fit, k, j1, j2, c, pos, offs, mutFitness;

	memcpy(newState, currentState, NN_NN*sizeof(int));
	memcpy(newFitnessList, currentFitnessList, 2*NN*sizeof(int));


	// row with probability, that has nofixed positions >= 2
	mutFitness = currentFitness;

	for(i = 0; i < NN; ++i){
		if( (rand() % 100  <  (int)(parMutProbability * 100)) && 1 < fixedList[i * NN + i] ){

			offset = i * NN;
			counter = fixedList[offset + i];

			// random 2 not the same positions
			position1 = (rand() % counter);
			do{
				position2 = (rand() % counter);
			}while(position1 == position2);

			// get positions from fixedList
			position1 = fixedList[offset + i + position1 + 1];
			position2 = fixedList[offset + i + position2 + 1];

			// swap positions
			swap = newState[position1 + offset];
			newState[position1 + offset] = newState[position2 + offset];
			newState[position2 + offset] = swap;
			//outstr.Format("Mut %d ->   %d >< %d", i, position1, position2  ); CLogger::Instance()->writeg(outstr);

			//  Fitness , evaluate only changes------------------------------------------


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
			mutFitness = mutFitness + fc1 + fc2 + fb1 + fb2 ;

		}
	}
	return mutFitness;
}

*/
// new methods


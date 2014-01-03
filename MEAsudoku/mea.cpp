/*
* Bakalarsky projekt
* Využitie multiagentového evolučného algoritmu v probléme z umelej inteligencie
* FIIT-5212-5799
* subor: MEA.cpp
* autor: Dávid Durčák,  ič:5799
* datum: 13.05.2013
*/

#include <QString>
#include <QDebug>
#include "mea.h"


int MEA:: counterTrial;


// Constructor & Destructor
MEA::MEA(){
	counterTrial = 0;
	srand((unsigned int)time(NULL));
}
MEA::~MEA(){
	std::list< AgentSudoku * >::iterator  itAgent;

	delete fixedState;
	delete fixedLists;
	delete tabuList;
	eliteList.~EliteList();

	for(itAgent = agents.begin(); itAgent != agents.end();  ++itAgent){
		//(*itAgent)->~AgentSudoku();
		delete (*itAgent);
	}
	agents.clear();
}
void MEA:: setParameters(double nmutProbability, int nnumAgents,int nmaxGenrations,int nmaxTrials,
							int	nlifePoints,int	nbirthStep,int nmilestoneStep,
							int nelitelistSize, char *nsudokuFile, bool ntestMode){

	parMutProbability = nmutProbability;

	parNumAgents		= nnumAgents;
	parMaxGenrations	= nmaxGenrations;
	parMaxTrials		= nmaxTrials;
	parStartLifePoints	= nlifePoints;
	parBirthStep		= nbirthStep;
	parMilestoneStep	= nmilestoneStep;
	parElitelistSize	= nelitelistSize;

	sudokuFile = nsudokuFile;

	testMode = ntestMode;
}





// main MEA optimalization method ( function of MEA)
int MEA:: optimize(){
	int generation;//, i;
	int *solution;
	//int meanPopSize = 0;
	//CString outstr;

	eliteList.setParameters(parElitelistSize);

	//printf("\n=========INITIALIZATION========");
	if(initialization())
		return -1;
	//printf("\n=========GENERATE AGENTS=======");
	generateAgents();
	//CLogger::Instance()->write("SOLVING ...");

	//testEliteList();



	counterTrial = 0;
	int maxFitTrias = parMaxGenrations-2*parNumAgents;
	generation = 0;

	while(counterTrial < maxFitTrias){
		generation++;
		solution = controlFitness();

		if(NULL != solution){
			if(! testMode){
				AgentSudoku::printState(solution);
			}
			return counterTrial;
		}

		decLifePointsAndEliminate();

		if( (0 == generation % parBirthStep ) && ( agents.size() < (unsigned)parNumAgents )){
				birthNewAgent();
		}
		//printAgents();

		localSearch(generation);

		//meanPopSize += agents.size();
		//printBestMeanFitAndAgentsSize();
	}
	//outstr.Format("CT   %.3d   Gen %5d", counterTrial, generation); CLogger::Instance()->write(outstr);
	//CString outstr;
	//outstr.Format("mean Pop size  (%.3lf)", ((double)meanPopSize)/parMaxGenrations); CLogger::Instance()->write(outstr);
	//outstr.Format("NOT found Solution  (%d)", counterTrial); CLogger::Instance()->write(outstr);
	//CLogger::Instance()->write("     not");
	return 0;
}


// start initialization, load & prepare data
int MEA:: initialization(){
	int oneFixed,
		i = 0;
	int offset, j, k, counter;


	FILE *file = NULL;
	QString outstr;
	//outstr.Format("%s", sudokuFile );
	//m_listbox->AddString(outstr);

	if(NULL == (fixedState = new int[NN_NN])){
		qWarning() << "Error: malloc - fixedState in initialization()";
		return 1;
	}

	if(NULL == (file = fopen(sudokuFile, "r"))){
		qWarning() << "Error: cannot open file " << sudokuFile;

		return 1;
	}


	while((fscanf(file, " %d", &oneFixed )) != EOF) {
		fixedState[i] = oneFixed;
		//if(i % NN == 0) putchar('\n');;
		i++;
		//printf(" %d", oneFixed);
	}
	if(EOF == fclose(file)){
		qWarning() << "Error: cannot close file " <<  sudokuFile;
	}
	if(i != NN_NN){
		qWarning() << "Error: load Sudoku frm file (no exactly 81 numbers)";

		return 1;
	}
	//===================================================
	// initialization of fixedLists
	// its matrix NN rows x (NN+1) columns ,  1. in column is count non fixed positions in row
	if(NULL == (fixedLists = new int[NN_NN + NN])){
		qWarning() << "Error: malloc - fixedLists in initialization()";

		return 1;
	}

	offset = 0;
	for(i = 0; i < NN; i++){
		counter = 0;
		k = offset + i;   // = i * (NN + 1);

		for(j = 0; j < NN; j++){
			if(0 == fixedState[offset + j]){
				k++;
				fixedLists[k] = j;
				counter++;
			}
		}
		fixedLists[offset + i] = counter;
		offset += NN;
	}
	/*// test
	offset = 0;
	for(i = 0; i < NN; i++){
		counter = fixedLists[offset];
		printf("%d = ", counter);
		for(j = 0; j < counter; j++){
			printf(" %d", fixedLists[offset + j+1]);
		}
		putchar('\n');
		offset += (NN + 1);
	}*/

	// =====================================================
	// initialization of TabuList
	// it NNxNNxNN 3D array where is check for each cell, if every number can be there
	if(NULL == (tabuList = new int[NN_NN*NN])){
		qWarning() << "Error: malloc - tabuLists in initialization()";

		return 1;
	}
		// make tabu cube2
	memset( tabuList, 0,  (NN_NN)*NN * sizeof(int));

	offset = 0;
	for(i = 0; i < NN; i++){
		for(j = 0; j < NN; j++){
			oneFixed = fixedState[i*NN + j] - 1;
			if(0 <= oneFixed){
				// TABUing in column
				for(k = 0; k < NN; k++){
					tabuList[(k*NN + j)*NN + oneFixed] = TABU;
				}
				// TABUing in block
				offset = (i/N) * NN * N  + (j/3) * N;
				for(k = 0; k < NN; ++k){
					if(k == N)
						offset = offset + N + N;
					if(k == N + N)
						offset = offset + N + N;
					tabuList[(k  + offset)*NN + oneFixed] = TABU;
				}
			}
		}
	}
	/*
	// test prints
	offset = 0;
	for(i = 0; i < NN; i++){
		outstr = "";
		for(j = 0; j < NN; j++){
			counter = NN;

			for(k = 0; k < NN; k++){
				if(TABU == tabuList[(i*NN + j)*NN + k])
					counter--;
			}
			oneFixed = fixedState[offset + j];
			outstr.Format(outstr + "%c ", oneFixed != 0 ? '.' : counter + 48);

		}
		CLogger::Instance()->write(outstr);
		offset += NN;
	}
	*/
	return 0;
}


// generate initial ppopulation of agents
int MEA:: generateAgents(){
	int i;
	std::list< AgentSudoku * >::iterator  itAgent;

	for(i = 0; i < parNumAgents;  ++i)
		agents.push_back(new AgentSudoku(parStartLifePoints, fixedState, fixedLists, parMaxTrials,	parMutProbability, tabuList));

	for(itAgent = agents.begin(); itAgent != agents.end();  ++itAgent){
		(*itAgent)->generateNewState();

	}
	return 0;
}

// chceck if solution was found
int * MEA:: controlFitness(){
	std::list< AgentSudoku * >::iterator  itAgent;

	for(itAgent = agents.begin(); itAgent != agents.end();  ++itAgent){
		if(OPTIMAL_FITNESS == (*itAgent)->getCurrentFitness()){

			return (*itAgent)->getCurrentState();
		}
	}
	return NULL;
}

// decrease of lifespan and elimination of agents, that don't have any life points
// (push bestmilestone state to Elit List)
int MEA:: decLifePointsAndEliminate(){
	std::list< AgentSudoku * >::iterator  itAgent, auxIt;
	//QString outstr;

	//CLogger::Instance()->write("LIFE POINTS:");

	for(itAgent = agents.begin(); itAgent != agents.end();  ){
		auxIt = itAgent;
		++itAgent;
		//outstr.Format("\nM(%d)LF %3d   CF  %3d  %p",(*auxIt)->getName(),(*auxIt)->getLifePoints(), (*auxIt)->getCurrentFitness(),(*auxIt)->getCurrentState() );

		if(0 >= (*auxIt)->decLifePoints()){
			// check if agents has milestone state
			if(MAX_FITNESS == (*auxIt)->getBestMilestoneFitness() ){
				// if no , last state become elit state
				eliteList.pushState((*auxIt)->getCurrentFitness(), (*auxIt)->getCurrentState() );
				(*auxIt)->setCurrentState(NULL);
			}else{
				eliteList.pushState((*auxIt)->getBestMilestoneFitness(), (*auxIt)->getBestMilestoneState());
				(*auxIt)->setBestMilestoneState(NULL);
			}

			delete (*auxIt);
			agents.erase(auxIt);
		}
	}
	return 0;
}

// birth new agent(get state from elit list)
int MEA:: birthNewAgent(){
	//QString outstr;

	STATE randomState = eliteList.getRandomState();
	//STATE randomState = eliteList.getRandomStateUsingRulet();
	//STATE randomState = eliteList.getRandomStateUsingTournament();
	//outstr.Format("EliteList:: getRandomState() get state %3d %p", randomState.fitness, randomState.state); CLogger::Instance()->write(outstr);

	if(NULL != randomState.state)
		agents.push_back(new AgentSudoku(parStartLifePoints, fixedState, fixedLists, randomState.state, randomState.fitness, parMaxTrials,	parMutProbability,tabuList));
	else
		agents.push_back(new AgentSudoku(parStartLifePoints, fixedState, fixedLists, parMaxTrials,	parMutProbability, tabuList));

	++counterAgents;
	agents.back()->setName(counterAgents);
	//outstr.Format("agent back %3d.  F %3d   CS  %p,",agents.back()->getName(), agents.back()->getCurrentFitness(), agents.back()->getCurrentState()); m_listbox->AddString(outstr);
	//CLogger::Instance()->write("agent birt");
	return 0;
}

//   local search
int MEA:: localSearch(int generation){
	//int reward;
	std::list< AgentSudoku * >::iterator  itAgent;

	//CLogger::Instance()->write("LOCAL SEARCH ");

	for(itAgent = agents.begin(); itAgent != agents.end(); ++itAgent ){


		// reward or punishment
		//if((*itAgent)->localSearch()){
		//if(  (*itAgent)->localSearchUseFitList()){
		if(  (*itAgent)->localSearchUseHeuristic()){
			(*itAgent)->addLifePoints();
			//reward = 1;
		}
		// punishment hasnt be
		/*else{
			(*itAgent)->decLifePoints();

		}*/

		if(0 != generation && 0 == generation % parMilestoneStep ){
			(*itAgent)->resetMilestoneState();
		}
	}

	return 0;
}




// aux methods

// aux print agents information
void MEA:: printAgents(){
	std::list< AgentSudoku * >::iterator  itAgent;
	QString outstr;

	qWarning() << "Agents count = " << agents.size();


	for(itAgent = agents.begin(); itAgent != agents.end();  ++itAgent){
		outstr ="  agent " + (*itAgent)->getName();
		outstr += ":  life " + (*itAgent)->getLifePoints();
		outstr +=  ",  F "		+ (*itAgent)->getCurrentFitness();
		outstr +=  ",  M "		+ (*itAgent)->getBestMilestoneFitness();
		outstr +=  ",  CS "	;
		QString pStr;
		pStr.sprintf("%08p",  (*itAgent)->getCurrentState()) ;
		qWarning() << outstr << pStr;
	}

}

// statistics information about all agents
void MEA:: printBestMeanFitAndAgentsSize(){
	std::list< AgentSudoku * >::iterator  itAgent;
	QString outstr;

	int bestFit,
		meanFit = 0,
		bestElitFit,
		meanElitFit = 0,
		elitCounter = 0;


	if(agents.empty()){
		qWarning() << "Warning  : agents.size() == 0,  (printBestMeanFitAndAgentsSize())";
		return;
	}

	bestElitFit = bestFit = MAX_FITNESS;
	bestElitFit = agents.front()->getBestMilestoneFitness();

	for(itAgent = agents.begin(); itAgent != agents.end();  ++itAgent){
		if(bestFit > (*itAgent)->getCurrentFitness()){
			bestFit = (*itAgent)->getCurrentFitness();
		}
		meanFit += (*itAgent)->getCurrentFitness();

		if(MAX_FITNESS != (*itAgent)->getBestMilestoneFitness()){
			elitCounter++;
			if(bestElitFit > (*itAgent)->getBestMilestoneFitness() ){
				bestElitFit = (*itAgent)->getBestMilestoneFitness();
			}
			meanElitFit += (*itAgent)->getBestMilestoneFitness();
		}
	}

	meanFit = meanFit / agents.size();
	if(0 < elitCounter){
		meanElitFit = meanElitFit / elitCounter;
	}else{
		bestElitFit = 0;
		meanElitFit = 0;
	}
	outstr = " ; " +  bestFit;
	outstr +=  " ; " + meanFit;
	outstr +=  " ; " + bestElitFit;
	outstr +=  " ; " + meanElitFit;
	outstr +=  ";  " + agents.size();
	outstr += ";  " + eliteList.getBestFitness();
	outstr += "; " +  eliteList.getNumberOfEliteStates();
	qWarning() << outstr;
}

// testing metod
void MEA:: testEliteList(){
	QString outstr;
	std::list< AgentSudoku * >::iterator  itAgent;

	qWarning() << "TEST Elite List";

	eliteList.setParameters(parElitelistSize);
	eliteList.printEliteList();
	for(itAgent = agents.begin(); itAgent != agents.end();  ++itAgent){
		eliteList.pushState((*itAgent)->getCurrentFitness(),(*itAgent)->getCurrentState());
		//eliteList.printEliteList();
	}
	eliteList.printEliteList();



	//===============================================================
	// test decAndEliminate---------------------
	std::list< AgentSudoku * >::iterator   auxIt;
	int i;
	qWarning() << "TEST decAndEliminate()  LIFE POINTS:";
	i = 0;
	for(itAgent = agents.begin(); itAgent != agents.end(); ++itAgent){
		//if(i %3 == 0)
		if(i %3 == 0 || i %3 == 2)
		//if(i  != 14)
			(*itAgent)->setLifePoints(0);
		i++;
	}
	for(itAgent = agents.begin(); itAgent != agents.end();  ){
		auxIt = itAgent;
		++itAgent;
		outstr = "M(" + (*auxIt)->getName();
		outstr += ")***" + (*auxIt)->getLifePoints();
		outstr += "  ";

		if(0 >= (*auxIt)->decLifePoints()){
			agents.erase(auxIt);
			//printf("-> agent kaput");
			outstr += "-> agent kaput";
		}
		qWarning() << outstr;
	}
	//=========================

}

void MEA::	addCounterTrials(){
	MEA::counterTrial++;
}

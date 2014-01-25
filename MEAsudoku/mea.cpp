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

#include "resultemitersigleton.h"


int MEA:: counterTrial;


// Constructor & Destructor
MEA::MEA(){
	counterTrial = 0;
	srand((unsigned int)time(NULL));
	fixedState	= NULL;
	fixedLists	= NULL;
	tabuList	= NULL;
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
void MEA:: setParameters( const int* givenData, int nnumAgents,int nmaxGenrations,int nmaxTrials,
							int	nlifePoints,int	nbirthStep,int nmilestoneStep,
							int nelitelistSize, bool ntestMode){


	parNumAgents		= nnumAgents;
	parMaxGenrations	= nmaxGenrations;
	parMaxTrials		= nmaxTrials;
	parStartLifePoints	= nlifePoints;
	parBirthStep		= nbirthStep;
	parMilestoneStep	= nmilestoneStep;
	parElitelistSize	= nelitelistSize;

	testMode = ntestMode;

	if(NULL == (fixedState = new int[NN_NN + NN])){
		qDebug() << "Error: malloc - fixedState in setParameters()";

		return;
	}
	memcpy(fixedState, givenData, NN_NN * sizeof(int));

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
				//emit pushMsg("jopjopok");
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





	//===================================================
	// initialization of fixedLists
	// its matrix NN rows x (NN+1) columns ,  1. in column is count non fixed positions in row
	if(NULL == (fixedLists = new int[NN_NN + NN])){
		qDebug() << "Error: malloc - fixedLists in initialization()";

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
		qDebug() << "Error: malloc - tabuLists in initialization()";

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
		agents.push_back(new AgentSudoku(parStartLifePoints, fixedState, fixedLists, parMaxTrials, tabuList));

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
		agents.push_back(new AgentSudoku(parStartLifePoints, fixedState, fixedLists, randomState.state, randomState.fitness, parMaxTrials,	tabuList));
	else
		agents.push_back(new AgentSudoku(parStartLifePoints, fixedState, fixedLists, parMaxTrials, tabuList));

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




void MEA::	addCounterTrials(){
	MEA::counterTrial++;
}



QStringList MEA:: printState(const int *state){
	QStringList list;
	//CLogger::Instance()->write(" PRINT STATE-----------------------------);

	if(NULL == state){
		qDebug() << "ERROR printState(): state is NULLL";
		return list;
	}

	for(int i = 0; i < NN*NN; ++i){
		list << QString::number(state[i]) ;
	}
	return list;
}

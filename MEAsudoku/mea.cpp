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
	srand((unsigned int)time(NULL));

	counterTrial = 0;
	fixedState	= NULL;
	fixedLists	= NULL;
	tabuList	= NULL;
}
MEA::~MEA(){
	std::list< AgentSudoku * >::iterator  itAgent;

	delete fixedState;
	delete fixedLists;
	delete tabuList;

	for(itAgent = agents.begin(); itAgent != agents.end();  ++itAgent){
		delete (*itAgent);
	}
	agents.clear();

}
void MEA:: setParameters( const int* givenData, int nnumAgents,int nmaxGenrations,int nmaxTrials,
						  int	nlifePoints,int	nbirthStep,int nmilestoneStep,
						  int nelitelistSize ){

	parNumAgents		= nnumAgents;
	parMaxGenrations	= nmaxGenrations;
	parMaxTrials		= nmaxTrials;
	parStartLifePoints	= nlifePoints;
	parBirthStep		= nbirthStep;
	parMilestoneStep	= nmilestoneStep;
	parElitelistSize	= nelitelistSize;

	if(NULL == (fixedState = new int[N4 + N2])){
		qDebug() << "Error: malloc - fixedState in setParameters()";

		return;
	}
	memcpy(fixedState, givenData, N4 * sizeof(int));
}





// main MEA optimalization method ( function of MEA)
int MEA:: optimize(){
	int generation;
	const int *solution;

	eliteList.setParameters(parElitelistSize);

	if(initialization())
		return -1;

	generateAgents();

	counterTrial = 0;
	int maxFitTrias = parMaxGenrations-2*parNumAgents;
	generation = 0;

	while(counterTrial < maxFitTrias){
		generation++;
		solution = controlFitness();

		if(NULL != solution){
			AgentSudoku::printState(solution);
			return counterTrial;
		}

		decLifePointsAndEliminate();

		if( (0 == generation % parBirthStep ) && ( agents.size() < (unsigned)parNumAgents )){
			birthNewAgent();
		}

		localSearch(generation);

	}
	return 0;
}


// start initialization, load & prepare data
int MEA:: initialization() {
	int oneFixed,
			i = 0;
	int offset, j, k, counter;


	//===================================================
	// initialization of fixedLists
	// its matrix NN rows x (NN+1) columns ,  1. in column is count non fixed positions in row
	if(NULL == (fixedLists = new int[N4 + N2])){
		qDebug() << "Error: malloc - fixedLists in initialization()";

		return 1;
	}

	offset = 0;
	for(i = 0; i < N2; i++){
		counter = 0;
		k = offset + i;   // = i * (NN + 1);

		for(j = 0; j < N2; j++){
			if(0 == fixedState[offset + j]){
				k++;
				fixedLists[k] = j;
				counter++;
			}
		}
		fixedLists[offset + i] = counter;
		offset += N2;
	}

	// =====================================================
	// initialization of TabuList
	// it NNxNNxNN 3D array where is check for each cell, if every number can be there
	if(NULL == (tabuList = new int[N4*N2])){
		qDebug() << "Error: malloc - tabuLists in initialization()";

		return 1;
	}
	// make tabu cube2
	memset( tabuList, 0,  (N4)*N2 * sizeof(int));

	offset = 0;
	for(i = 0; i < N2; i++){
		for(j = 0; j < N2; j++){
			oneFixed = fixedState[i*N2 + j] - 1;
			if(0 <= oneFixed){
				// TABUing in column
				for(k = 0; k < N2; k++){
					tabuList[(k*N2 + j)*N2 + oneFixed] = TABU;
				}
				// TABUing in block
				offset = (i/N) * N2 * N  + (j/3) * N;
				for(k = 0; k < N2; ++k){
					if(k == N)
						offset = offset + N + N;
					if(k == N + N)
						offset = offset + N + N;
					tabuList[(k  + offset)*N2 + oneFixed] = TABU;
				}
			}
		}
	}
	return 0;
}


// generate initial ppopulation of agents
int MEA:: generateAgents() {
	int i;
	std::list< AgentSudoku * >::const_iterator  itAgent;

	for(i = 0; i < parNumAgents;  ++i)
		agents.push_back(new AgentSudoku(parStartLifePoints, fixedState, fixedLists, parMaxTrials, tabuList));

	for(itAgent = agents.begin(); itAgent != agents.end();  ++itAgent){
		(*itAgent)->generateNewState();

	}
	return 0;
}

// chceck if solution was found
const int *MEA::controlFitness() const{
	std::list< AgentSudoku * >::const_iterator  itAgent;

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
	//outstr.Format("EliteList:: getRandomState() get state %3d %p", randomState.fitness, randomState.state); CLogger::Instance()->write(outstr);

	if(NULL != randomState.state)
		agents.push_back(new AgentSudoku(parStartLifePoints, fixedState, fixedLists, randomState.state, randomState.fitness, parMaxTrials,	tabuList));
	else
		agents.push_back(new AgentSudoku(parStartLifePoints, fixedState, fixedLists, parMaxTrials, tabuList));

	++counterAgents;
	//outstr.Format("agent back %3d.  F %3d   CS  %p,",agents.back()->getName(), agents.back()->getCurrentFitness(), agents.back()->getCurrentState()); m_listbox->AddString(outstr);
	return 0;
}

//   local search
int MEA:: localSearch(int generation) const{
	std::list< AgentSudoku * >::const_iterator  itAgent;

	//CLogger::Instance()->write("LOCAL SEARCH ");

	for(itAgent = agents.begin(); itAgent != agents.end(); ++itAgent ){


		// reward or punishment
		if(  (*itAgent)->localSearchUseHeuristic()){
			(*itAgent)->addLifePoints();
		}
		// punishment hasnt be

		if(0 != generation && 0 == generation % parMilestoneStep ){
			(*itAgent)->resetMilestoneState();
		}
	}

	return 0;
}


QStringList MEA:: printState(const int *state) const {
	QStringList list;

	if(NULL == state){
		qDebug() << "ERROR printState(): state is NULLL";
		return list;
	}

	for(int i = 0; i < N4; ++i){
		list << QString::number(state[i]) ;
	}
	return list;
}

#include "sudokuthread.h"
#include <QTime>
#include <QElapsedTimer>

SudokuThread::SudokuThread()
{
}

void SudokuThread::setParameters( const PARAMETERS &parm,  int *givenData ){
	this->parm = parm;
	this->givenData = givenData;


}

void SudokuThread::run(){
	int result = 0,
			countTrials = 0;

	QTime time;
	time.start();


	while(result <= 0 && countTrials < 100){

		mea = new MEA();
		mea->setParameters( givenData,
							parm.popSize,			// numAgents
							parm.maxCalls,			// maxGenrations
							parm.localTrials,		// maxTrials
							parm.lifespan,			// lifePoints
							parm.birthPeriod,		// birthStep
							parm.milestonePeriod,	// milestoneStep
							parm.elitSize,			// elitelistSize,
							false );			// testMode ----!!!! VYHODIT ????


		result = mea->optimize();
		delete mea;
		countTrials++;
	}

	double runTime = time.elapsed() / 1000.0;

	QString msg;
	if(result <= 0){
		msg = "Solution was not found. \nTry again with different parameters.";
	} else {
		msg = QString::number(countTrials) + ". pokus  -- " + QString::number(runTime, 'f', 2) + " s";
	}

	emit done( msg);
}



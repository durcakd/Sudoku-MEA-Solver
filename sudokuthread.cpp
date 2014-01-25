#include "sudokuthread.h"
#include <QTime>
#include <QElapsedTimer>
#include <QDebug>

SudokuThread::SudokuThread()
{
	mAutoParams = false;
	mAbort		= false;
}

void SudokuThread::setParameters(const PARAMETERS &parm,  int *givenData , bool autoParams){
	this->parm = parm;
	this->givenData = givenData;
	this->mAutoParams = autoParams;

}

void SudokuThread::setAbort(){
	mAbort = true;
}

void SudokuThread::run(){
	int result = 0;
	int countTrials = 0;

	copyOriginParams();   // save original parameters

	QTime time;
	time.start();

	emit sendProgress( countTrials );
	emit sentStatusMsg( tr("Processing...")  );
	mAbort = false;
	while(result <= 0  &&  countTrials < NUMTESTS  &&  !mAbort ){

		if( mAutoParams ){
			computeParams( countTrials );
		}

		mea = new MEA();
		mea->setParameters( givenData,
							parm.popSize,			// numAgents
							parm.maxCalls,			// maxGenrations
							parm.localTrials,		// maxTrials
							parm.lifespan,			// lifePoints
							parm.birthPeriod,		// birthStep
							parm.milestonePeriod,	// milestoneStep
							parm.elitSize			// elitelistSize,
							);


		result = mea->optimize();
		delete mea;
		countTrials++;
		emit sendProgress( countTrials );
	}
	emit sendProgress( NUMTESTS );
	double runTime = time.elapsed() / 1000.0;

	QString msg;
	if(result <= 0){
		msg = tr("Solution not found, try again (") + QString::number(runTime, 'f', 2) + tr(" s)") ;
		if( mAbort ){
			msg = tr("Aborted");
		}
	} else {
		msg = QString::number(countTrials) + tr(". attempt  in time ") + QString::number(runTime, 'f', 2) + tr(" s") ;
	}

	emit done( msg);
	emit sentStatusMsg( msg );
}

// copy original parameters
void SudokuThread::copyOriginParams(){
	originParm.popSize = parm.popSize;
	originParm.elitSize = parm.elitSize;
	originParm.lifespan = parm.lifespan;
	originParm.birthPeriod = parm.birthPeriod;
	originParm.milestonePeriod = parm.milestonePeriod;
	originParm.localTrials = parm.localTrials;
	originParm.maxCalls = parm.maxCalls;
}

// compute new stronger parameters
void SudokuThread::computeParams(int trial){
	if( trial < 0 || trial >= NUMTESTS ){
		qDebug() << "Warning: SudokuThread.computeParams(): wrong parameter: trial = " << trial;
		return;
	}

	parm.popSize		= originParm.popSize	+ trial * 4;
	parm.elitSize		= originParm.elitSize + trial * 4;
	parm.lifespan		= originParm.lifespan + trial * 3;

	//parm.birthPeriod		= originParm.birthPeriod		;//+ trial / 5;
	//parm.milestonePeriod	= originParm.milestonePeriod	;//+ trial / 5;

	parm.localTrials	= originParm.localTrials	+ trial / 25;
	parm.maxCalls		= originParm.maxCalls		+ trial * 20000;

}












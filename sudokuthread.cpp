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
	int result = 0,
			countTrials = 0;

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

void SudokuThread::computeParams(int trial){
	if( trial < 0 || trial >= NUMTESTS ){
		qDebug() << "Warning: SudokuThread.computeParams(): wrong parameter: trial = " << trial;
		return;
	}

	parm.popSize		= parm.popSize	+ trial * 3;
	parm.elitSize		= parm.elitSize + trial * 3;

	parm.lifespan		= parm.lifespan + trial * 3;

	parm.birthPeriod		= parm.birthPeriod		;//+ trial / 5;
	parm.milestonePeriod	= parm.milestonePeriod	;//+ trial / 5;

	parm.localTrials	= parm.localTrials	+ trial / 25;
	parm.maxCalls		= parm.maxCalls		+ trial * 10000;

}












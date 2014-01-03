#include "sudokuthread.h"
#include <QTime>
#include <QElapsedTimer>

SudokuThread::SudokuThread()
{
}

void SudokuThread::setParameters( const PARAMETERS &parm,  int *givenData ){
	this->parm = parm;
	this->givenData = givenData;


	QString g = "ddd" +  QString::number( this->parm.birthPeriod) + "  " + QString::number(this->givenData[0]);
	emit done(g);

}

void SudokuThread::run(){
	int result = 0,
			countTrials = 0;

	QTime time;
	time.start();


	while(result <= 0 && countTrials < 100){

		//mea = new MEA();
		/*mea->setParameters( 0,		// mutProbability ----!!!! VYHODIT
						   parm.popSize,			// numAgents
						   parm.maxCalls,			// maxGenrations
						   parm.localTrials,		// maxTrials
						   parm.lifespan,			// lifePoints
						   parm.birthPeriod,		// birthStep
						   parm.milestonePeriod,	// milestoneStep
						   parm.elitSize,			// elitelistSize,
						   NULL,			// sudokuFiles[0] ----!!!! VYHODIT
						   false );			// testMode ----!!!! VYHODIT ????

		*/
		result = 1;//mea->optimize();
		delete mea;
		countTrials++;
	}

	sleep(1);

	double runTime = time.elapsed() / 1000.0;

	QString msg;
	if(result <= 0){
		msg = "Solution was not found. \nTry again with different parameters.";
	} else {
		msg = QString::number(countTrials) + ". pokus  -- " + QString::number(runTime, 'f', 2) + " s";
	}

	emit done( msg);
}

/*
// normal mode (run only 1 sudoku, until solution is found & display)
int Tests:: solving(){
	CString outstr;
	MEA *mea;
	int countOk, countTrials, result;
	clock_t runTime;

	srand((unsigned int)time(NULL));
	if(prepareOneSudokuFile()){
		return 1;
	}

	countOk = 0;
	countTrials = 0;
	runTime = clock();
	while(0 >= countOk && countTrials < 100){

		mea = new MEA();
		mea->setParameters(mutProbability, numAgents, maxGenrations, maxTrials,
			lifePoints, birthStep, milestoneStep, elitelistSize,
			sudokuFiles[0], testMode);
		result = mea->optimize();
		//mea->~MEA();
		delete mea;

		if(0 != result ){
			countOk++;
		}
		countTrials++;


	}
	//
	runTime = clock() - runTime;

	outstr.Format("%4d. pokus (%6.3lf s)", countTrials, ((double)runTime)/(CLOCKS_PER_SEC)   );
	CLogger::Instance()->write(outstr);
	if(0 == countOk && 100 == countTrials){
		CLogger::Instance()->write("Solution was not found.");
		CLogger::Instance()->write("Try again with different parameters.");
	}
	return 0;
}

*/

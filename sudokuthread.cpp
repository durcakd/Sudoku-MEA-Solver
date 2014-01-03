#include "sudokuthread.h"

SudokuThread::SudokuThread()
{
}

void SudokuThread::setParameters( const PARAMETERS &parm, const int *givenData ){
	//QString g = "ddd" +  QString::number( parm.birthPeriod) + "  " + QString::number(givenData[0]);
	//emit done(g);

}

void SudokuThread::run(){
	sleep(1);
	emit done("thread done");
}

#include "sudokuthread.h"

SudokuThread::SudokuThread()
{
}

void SudokuThread::setParameters( const PARAMETERS &parm){
	QString g = "ddd" +  QString::number( parm.birthPeriod);

	emit done( g);
}

void SudokuThread::run(){
	sleep(1);
	emit done("thread done");
}

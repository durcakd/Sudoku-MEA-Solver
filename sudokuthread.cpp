#include "sudokuthread.h"

SudokuThread::SudokuThread()
{
}

void SudokuThread::run(){
	sleep(1);
	emit done("thread done");
}

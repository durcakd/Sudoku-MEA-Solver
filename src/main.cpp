/**
  \author Dávid Durčák
  \date 20. December 2013
  Project: Sudoku MEA Solver
  */


#include <QApplication>

#include "sudokucontroller.h"

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);

	SudokuController controller(&app);

	return app.exec();
}

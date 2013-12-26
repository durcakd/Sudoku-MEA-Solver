#include "sudokucontroller.h"

// constructor
SudokuController::SudokuController(QApplication *app){
mApp = app;
mSudokuDialog = new SudokuDialog();
mSudokuTableModel = NULL;



mSudokuDialog->show();
}

// destructor
SudokuController::~SudokuController(){
	mApp;
	delete mSudokuDialog;
	delete mSudokuTableModel;
}

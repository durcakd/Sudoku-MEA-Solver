#include "sudokucontroller.h"

// constructor
SudokuController::SudokuController(QApplication *app){
	mApp = app;

	mSudokuTableModel = new SudokuTableModel();
	mSudokuDialog = new SudokuDialog();
	mSudokuDialog->setTableModel(mSudokuTableModel);

	createConnections();   // create connecions

	mSudokuDialog->show();
}

// destructor
SudokuController::~SudokuController(){
	mApp;
	delete mSudokuDialog;
	delete mSudokuTableModel;
}

// create connections
void SudokuController::createConnections(){
	// reading sudoku from file
	QObject::connect( mSudokuDialog, SIGNAL(requestForReadFile(QString)),
					  mSudokuTableModel, SLOT(openFile(QString)));
	//

}

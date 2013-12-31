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
	// saving sudoku to file
	QObject::connect( mSudokuDialog, SIGNAL(requestForSaveFile(QString)),
					  mSudokuTableModel, SLOT(saveToFile(QString)));
	// cleaning model for new sudoku
	QObject::connect( mSudokuDialog->getNewTB(), SIGNAL(clicked()),
					  mSudokuTableModel, SLOT(clean()));

	// confirm / unconfirm model in sudoku
	QObject::connect( mSudokuDialog, SIGNAL(requestForConfirm(bool)),
					  mSudokuTableModel, SLOT(confirm(bool)));
	//
	QObject::connect( mSudokuDialog->getThread(), SIGNAL(done(QString)),
					  mSudokuDialog, SLOT(threadDone(QString)));


}

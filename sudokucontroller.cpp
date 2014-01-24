#include "sudokucontroller.h"
#include "sudokuitemdelegate.h"

// constructor
SudokuController::SudokuController(QApplication *app){
	mApp = app;

	mSudokuTableModel	= new SudokuTableModel();
	mSudokuDialog		= new SudokuDialog();
	resultEmiter		= ResultEmiterSigleton::getInstance();

	mSudokuDialog->setTableModel( mSudokuTableModel );
	mSudokuDialog->setTableItemDelegate( new SudokuItemDelegate );

	createConnections();   // create connecions
	// initial open file
	mSudokuTableModel->openFile(":images/s04b.txt");

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

	// !!! experimental  -> Not WORK, because cos mea is NULL, & reatly is created new instance
	//QObject::connect( mSudokuDialog->getThread()->getMea(), SIGNAL(pushMsg(QString)),
	//				  mSudokuDialog, SLOT(threadDone(QString)));

	QObject::connect( resultEmiter, SIGNAL(sentResult(QStringList)),
					  mSudokuTableModel, SLOT(setGridData(QStringList)));

	// progress in statusBar
	QObject::connect( resultEmiter, SIGNAL(sentTempMsg(QString, int)),
					  mSudokuDialog->getStatusBar(), SLOT(showMessage(QString,int)) );

	// progress
	QObject::connect( mSudokuDialog->getThread(), SIGNAL(sendProgress(int)),
					  mSudokuDialog->getTriesPB(), SLOT(setValue(int)) );

}

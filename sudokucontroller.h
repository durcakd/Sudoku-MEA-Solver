#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QApplication>

#include "sudokudialog.h"
#include "sudokutablemodel.h"

class SudokuController
	{
public:
	SudokuController(QApplication *app);
	~SudokuController();

private:
	void createConnections();

	QApplication *mApp;
	SudokuDialog *mSudokuDialog;
	SudokuTableModel *mSudokuTableModel;



	};

#endif // CONTROLLER_H

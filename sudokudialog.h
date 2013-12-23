#ifndef SUDOKUDIALOG_H
#define SUDOKUDIALOG_H

#include <QDialog>
#include <QTableView>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>

#include "sudokutablemodel.h"

class SudokuDialog : public QDialog
	{
	Q_OBJECT
public:
	explicit SudokuDialog(QWidget *parent = 0);

signals:

public slots:
	bool open();

private:
	bool openFile(const QString &fileName);



	QTableView	*tableView;
	SudokuTableModel *sudokuTableModel;

	QPushButton *openFileB;
	QPushButton *saveToFileB;
	QPushButton *startB;
	QLineEdit	*progresLE;

	QLineEdit	*statusBarLE;
	QLineEdit	*popSizeLE;
	QLineEdit	*elitSizeLE;
	QLineEdit	*lifespanLE;
	QLineEdit	*birthPeriodLE;
	QLineEdit	*milestonePeriodLE;
	QLineEdit	*localTrialsLE;
	QLineEdit	*maxCallsLE;


	int *givenData;


	};

#endif // SUDOKUDIALOG_H

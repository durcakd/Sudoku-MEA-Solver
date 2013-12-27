#ifndef SUDOKUDIALOG_H
#define SUDOKUDIALOG_H

#include <QDialog>
#include <QTableView>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>

#include "sudokutablemodel.h"

class SudokuDialog : public QDialog
	{
	Q_OBJECT
public:
	explicit SudokuDialog(QWidget *parent = 0);

	QTableView	*getTableView()	const	{ return tableView; }
	QListWidget *getListWidged() const	{ return listWidged; }
	QPushButton *getstartB() const		{ return startB; }
	QPushButton *getsaveToFileB() const	{ return saveToFileB; }
	QPushButton *getopenFileB() const	{ return openFileB; }
	void setTableModel(QAbstractTableModel *model ) const;

signals:
	void requestForReadFile(const QString &);

public slots:
	void addStrToListWidged(const QString &str);

private slots:
	bool open();

	void on_popSizeLE_textChanged(const QString &str);
	void on_elitSizeLE_textChanged(const QString &str);
	void on_lifespanLE_textChanged(const QString &str);
	void on_birthPeriodLE_textChanged(const QString &str);
	void on_milestonePeriodLE_textChanged(const QString &str);
	void on_localTrialsLE_textChanged(const QString &str);
	void on_maxCallsLE_textChanged(const QString &str);

private:
	void pripareParametersLE();

	QTableView	*tableView;
	SudokuTableModel *sudokuTableModel;
	QListWidget *listWidged;

	QPushButton *openFileB;
	QPushButton *saveToFileB;
	QPushButton *startB;
	QPushButton *parametersB;
	QPushButton *listB;
	QLineEdit	*progresLE;


	QLineEdit	*statusBarLE;
	QLineEdit	*popSizeLE;
	QLineEdit	*elitSizeLE;
	QLineEdit	*lifespanLE;
	QLineEdit	*birthPeriodLE;
	QLineEdit	*milestonePeriodLE;
	QLineEdit	*localTrialsLE;
	QLineEdit	*maxCallsLE;

	int popSize;
	int elitSize;
	int lifespan;
	int birthPeriod;
	int milestonePeriod;
	int localTrials;
	int maxCalls;

	/*
	popSize
	elitSize
	lifespan
	birthPeriod
	milestonePeriod
	localTrials
	maxCalls
	*/


	int *givenData;


	};

#endif // SUDOKUDIALOG_H

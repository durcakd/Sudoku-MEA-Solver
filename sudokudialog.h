#ifndef SUDOKUDIALOG_H
#define SUDOKUDIALOG_H

#include <QDialog>
#include <QTableView>
#include <QPushButton>
#include <QToolButton>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QIcon>

#include "sudokutablemodel.h"
#include "sudokuthread.h"

class SudokuDialog : public QDialog
	{
	Q_OBJECT
public:
	explicit SudokuDialog(QWidget *parent = 0);

	QTableView	*getTableView()	const	{ return tableView; }
	QListWidget *getListWidged() const	{ return listWidged; }
	QPushButton *getStartB() const		{ return startB; }
	QPushButton *getOpenFileB() const	{ return openFileB; }
	QPushButton *getSaveToFileB() const	{ return saveToFileB; }
	//QToolButton *getopenFileTB() const	{ return openFileTB; }
	//QToolButton *getsaveToFileTB() const	{ return saveToFileTB; }
	QToolButton *getNewTB() const		{ return newTB; }
	QToolButton *getConfirmTB() const	{ return confirmTB; }
	const SudokuThread *getThread() const		{ return &thread; }
	void setTableModel(QAbstractTableModel *model ) const;

signals:
	void requestForReadFile(const QString &);
	void requestForSaveFile(const QString &);
	void requestForConfirm(const bool ok);

public slots:
	void addStrToListWidged(const QString &str);
	void threadDone(const QString msg);

private slots:
	bool open();
	bool save();
	void confirm(const bool ok);
	void on_newTB_clicked();
	void start();

	void on_popSizeLE_textChanged(const QString &str);
	void on_elitSizeLE_textChanged(const QString &str);
	void on_lifespanLE_textChanged(const QString &str);
	void on_birthPeriodLE_textChanged(const QString &str);
	void on_milestonePeriodLE_textChanged(const QString &str);
	void on_localTrialsLE_textChanged(const QString &str);
	void on_maxCallsLE_textChanged(const QString &str);

private:
	void pripareParametersLE();

	SudokuThread thread;
	QTableView	*tableView;
	SudokuTableModel *sudokuTableModel;
	QListWidget *listWidged;


	//QToolButton *openFileTB;
	//QToolButton *saveToFileTB;
	QToolButton *confirmTB;
	QToolButton *newTB;
	QToolButton *startTB;
	QToolButton *parametersTB;
	QToolButton *listTB;
	QIcon *confirmIcon;
	QIcon *unconfirmIcon;

	QPushButton *startB;
	QPushButton *openFileB;
	QPushButton *saveToFileB;
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

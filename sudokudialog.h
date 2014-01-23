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
#include <QProgressBar>
#include <QCheckBox>
#include <QGroupBox>

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
	QToolButton *getOpenFileTB() const	{ return openFileTB; }
	QToolButton *getSaveToFileTB() const	{ return saveToFileTB; }
	QToolButton *getNewTB() const		{ return newTB; }
	QToolButton *getConfirmTB() const	{ return confirmTB; }
	const SudokuThread *getThread() const		{ return &thread; }
	QProgressBar *getTriesPB() const	{ return triesPB; }

	void setTableModel( QAbstractTableModel *model );
	void setTableItemDelegate( QAbstractItemDelegate *delegate );

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

	void setEasy();
	void setMedium();
	void setHard();
	void autoChangeState( bool checked );

private:
	void pripareParametersLE();

	bool autoParams;
	SudokuThread thread;
	QTableView	*tableView;
	SudokuTableModel *sudokuTableModel;
	QListWidget *listWidged;


	QToolButton *openFileTB;
	QToolButton *saveToFileTB;
	QToolButton *confirmTB;
	QToolButton *newTB;
	QToolButton *listTB;
	QIcon *confirmIcon;
	QIcon *unconfirmIcon;

	QPushButton *startB;
	QLineEdit	*progresLE;
	QPushButton *easyB;
	QPushButton *mediumB;
	QPushButton *hardB;


	QCheckBox *autoCB;

	QLineEdit	*statusBarLE;
	QLineEdit	*popSizeLE;
	QLineEdit	*elitSizeLE;
	QLineEdit	*lifespanLE;
	QLineEdit	*birthPeriodLE;
	QLineEdit	*milestonePeriodLE;
	QLineEdit	*localTrialsLE;
	QLineEdit	*maxCallsLE;

	QGroupBox	*parametersGroup;

	QProgressBar *triesPB;

	PARAMETERS parm;
	/*
	popSize
	elitSize
	lifespan
	birthPeriod
	milestonePeriod
	localTrials
	maxCalls
	*/



	};

#endif // SUDOKUDIALOG_H

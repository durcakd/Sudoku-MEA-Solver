#ifndef SUDOKUDIALOG_H
#define SUDOKUDIALOG_H

#include <QDialog>
#include <QTableView>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>

class SudokuDialog : public QDialog
	{
	Q_OBJECT
public:
	explicit SudokuDialog(QWidget *parent = 0);

signals:

public slots:

private:
	QTableView	*tableView;

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

	};

#endif // SUDOKUDIALOG_H

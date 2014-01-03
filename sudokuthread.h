#ifndef SUDOKUTHREAD_H
#define SUDOKUTHREAD_H

#include <QThread>
#include "constants.h"
#include "MEAsudoku/mea.h"

class SudokuThread : public QThread
	{
	Q_OBJECT
public:
	explicit SudokuThread();

	void setParameters( const PARAMETERS &parm, int *givenData);

signals:
	void done(const QString msg);

protected:
	void run();

private:
	PARAMETERS parm;
	int *givenData;
	MEA *mea;
	};

#endif // SUDOKUTHREAD_H

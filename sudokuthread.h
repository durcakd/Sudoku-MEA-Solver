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
	MEA *getMea() const { return mea; }

signals:
	void done(const QString msg);
	void sendProgress( int tryStep );

protected:
	void run();

private:
	bool autoParams;
	PARAMETERS parm;
	int *givenData;
	MEA *mea;


	void computeParams(int trial);
	};

#endif // SUDOKUTHREAD_H

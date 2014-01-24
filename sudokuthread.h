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

	void setParameters( const PARAMETERS &parm, int *givenData, bool autoParams);
	void setAbort();
	MEA *getMea() const { return mea; }

signals:
	void done(const QString msg);
	void sendProgress( int tryStep );
	void sentStatusMsg( const QString &msg, int timeout ) const;

protected:
	void run();

private:
	bool mAutoParams;
	bool mAbort;
	PARAMETERS parm;
	int *givenData;
	MEA *mea;


	void computeParams(int trial);
	};

#endif // SUDOKUTHREAD_H

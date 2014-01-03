#ifndef SUDOKUTHREAD_H
#define SUDOKUTHREAD_H

#include <QThread>
#include "constants.h"

class SudokuThread : public QThread
	{
	Q_OBJECT
public:
	explicit SudokuThread();

	void setParameters( const PARAMETERS &parm);

signals:
	void done(const QString msg);

protected:
	void run();

private:



	};

#endif // SUDOKUTHREAD_H

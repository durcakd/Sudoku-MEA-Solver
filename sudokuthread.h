#ifndef SUDOKUTHREAD_H
#define SUDOKUTHREAD_H

#include <QThread>

class SudokuThread : public QThread
	{
	Q_OBJECT
public:
	explicit SudokuThread();

signals:
	void done(const QString msg);

protected:
	void run();

private:



	};

#endif // SUDOKUTHREAD_H

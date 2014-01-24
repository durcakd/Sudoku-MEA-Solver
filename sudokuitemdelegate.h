#ifndef SUDOKUITEMDELEGATE_H
#define SUDOKUITEMDELEGATE_H

#include <QItemDelegate>



class SudokuItemDelegate : public QItemDelegate
	{
	Q_OBJECT
public:
	explicit SudokuItemDelegate(QObject *parent = 0);

protected:

	// Reimplement from base class to catch key press event
	// to catch press right & left keys to close editor and edit
	// next or previous item.
	bool eventFilter(QObject *object, QEvent *event);

	};

#endif // SUDOKUITEMDELEGATE_H

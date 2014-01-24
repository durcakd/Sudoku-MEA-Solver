#include "sudokuitemdelegate.h"
#include <QEvent>
#include <QKeyEvent>
SudokuItemDelegate::SudokuItemDelegate(QObject *parent) :
	QItemDelegate(parent)
{
}



// reimplement event filter to catch left & right keys to edit other items
bool SudokuItemDelegate::eventFilter(QObject *object, QEvent *event)
{
	QWidget *editor = qobject_cast<QWidget*>(object);
	if (!editor)
		return false;
	if (event->type() == QEvent::KeyPress) {
		switch (static_cast<QKeyEvent *>(event)->key()) {
		case Qt::Key_Tab:
		case Qt::Key_Right:
			emit commitData(editor);
			emit closeEditor(editor, QAbstractItemDelegate::EditNextItem );
			return true;
		case Qt::Key_Backtab:
		case Qt::Key_Left:
			emit commitData(editor);
			emit closeEditor(editor, QAbstractItemDelegate::EditPreviousItem );
			return true;
		default:
			return QItemDelegate::eventFilter(object, event);
		}
	} else {
		return QItemDelegate::eventFilter(object, event);
	}
}

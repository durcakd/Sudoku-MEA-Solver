#include <QApplication>

#include "sudokudialog.h"

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);
	SudokuDialog *dialog = new SudokuDialog();
	dialog->show();
	return app.exec();
}

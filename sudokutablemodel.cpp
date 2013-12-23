#include "sudokutablemodel.h"

// constructor
SudokuTableModel::SudokuTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{
}

// row count
int SudokuTableModel::rowCount(const QModelIndex & /*parent*/) const
{
	return 2;
}

// column count
int SudokuTableModel::columnCount(const QModelIndex &/*parent*/ ) const
{
	return 3;
}

// data
QVariant SudokuTableModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole){
		return QString("Row%1, Column%2")
				.arg(index.row() + 1)
				.arg(index.column() + 1);
	}
	return QVariant();
}

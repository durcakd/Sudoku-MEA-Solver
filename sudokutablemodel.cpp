#include "sudokutablemodel.h"
#include <QDebug>
#include <QFont>
#include <QBrush>

// constructor
SudokuTableModel::SudokuTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	m_gridData = new QString[ROWS * COLS];
}

// row count
int SudokuTableModel::rowCount(const QModelIndex & /*parent*/) const
{
	return ROWS;
}

// column count
int SudokuTableModel::columnCount(const QModelIndex &/*parent*/ ) const
{
	return COLS;
}

// data
QVariant SudokuTableModel::data(const QModelIndex &index, int role) const
{
	int row = index.row();
	int col = index.column();
	// generate a log message when this method gets called
	qDebug() << QString("%1 x %2 ->r  %3")
				.arg(row).arg(col).arg(role);

	switch(role) {
	case Qt::DisplayRole:
		if (row == 0 && col == 1) return QString("<--left");
		if (row == 1 && col == 1) return QString("right-->");

		return m_gridData[dd(row,col)];
		break;
	case Qt::FontRole:
		if (row == 0 && col == 0) //change font only for cell(0,0)
		{
			QFont boldFont;
			boldFont.setBold(true);
			return boldFont;
		}
		break;
	case Qt::BackgroundRole:

		if (row == 1 && col == 2)  //change background only for cell(1,2)
		{
			QBrush redBackground(Qt::red);
			return redBackground;
		}
		break;
	case Qt::TextAlignmentRole:

		if (row == 1 && col == 1) //change text alignment only for cell(1,1)
		{
			return Qt::AlignRight + Qt::AlignVCenter;
		}
		break;
	case Qt::CheckStateRole:

		if (row == 1 && col == 0) //add a checkbox to cell(1,0)
		{
			return Qt::Checked;
		}
	}
	return QVariant();
}

// set data
bool SudokuTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	int row = index.row();
	int col = index.column();
	if (role == Qt::EditRole)
	{
		//save value from editor to member m_gridData
		m_gridData[dd(row,col)] = value.toString();
	}
	return true;
}

// flags
Qt::ItemFlags SudokuTableModel::flags(const QModelIndex & index) const
{
	return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}


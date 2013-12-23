#include "sudokutablemodel.h"
#include <QDebug>
#include <QFont>
#include <QBrush>

// constructor
SudokuTableModel::SudokuTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	mGridData = new QString[ROWS * COLS];
	mGivenData = new int[ROWS * COLS];
	for(int i = 0; i < N4; i++){
		mGivenData[i] = 0;
		mGridData[i] = "";
		if(i % 5 == 0) {
			mGivenData[i] = 5;
			mGridData[i] = "5";
		}
	}
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
	//qDebug() << QString("%1 x %2 ->r  %3").arg(row).arg(col).arg(role);

	switch(role) {
	case Qt::DisplayRole:
		return mGridData[dd(row,col)];
		break;

		// change font only for given cell
	case Qt::FontRole:
		if (mGivenData[dd(row,col)] != 0) {
			QFont boldFont;
			boldFont.setBold(true);
			return boldFont;
		}
		break;

		// background NxN cells together
	case Qt::BackgroundRole:
		if(row < 3 || row > 5){
			if(col < 3 || col > 5){
				return QBrush(Qt::gray);
			}
		} else if(col > 2 && col < 6){
			return QBrush(Qt::gray);
		}
		break;

		// all cell are centered
	case Qt::TextAlignmentRole:
		return Qt::AlignCenter | Qt::AlignVCenter;
		break;

		/*//add a checkbox to cell
	case Qt::CheckStateRole:
		return Qt::Checked;
*/
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
		mGridData[dd(row,col)] = value.toString();
	}
	return true;
}

// flags
Qt::ItemFlags SudokuTableModel::flags(const QModelIndex & index) const
{
	int row = index.row();
	int col = index.column();
	if (mGivenData[dd(row,col)] == 0) {
		return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
	} else {
		return  Qt::ItemIsEnabled  ;
	}

}


#ifndef SUDOKUTABLEMODEL_H
#define SUDOKUTABLEMODEL_H

#include <QAbstractTableModel>

#define dd(r,c) ((r)*COLS + (c))

enum { N	= 3 ,
	   N2	= N*N ,
	   N4	= N2*N2 ,
	   COLS	= N2 ,
	   ROWS = N2 };

class SudokuTableModel : public QAbstractTableModel
	{
	Q_OBJECT
public:
	explicit SudokuTableModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex & index, const QVariant & value,
				 int role = Qt::EditRole);
	Qt::ItemFlags flags(const QModelIndex & index) const ;



private:
	QString *mGridData;
	int *mGivenData;


	};

#endif // SUDOKUTABLEMODEL_H

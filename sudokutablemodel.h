#ifndef SUDOKUTABLEMODEL_H
#define SUDOKUTABLEMODEL_H

#include "constants.h"
#include <QAbstractTableModel>


#define dd(r,c) ((r)*COLS + (c))



class SudokuTableModel : public QAbstractTableModel
	{
	Q_OBJECT
public:
	explicit SudokuTableModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex & index) const ;
	bool setData(const QModelIndex & index, const QVariant & value,
				 int role = Qt::EditRole);

	void setGivenData(const int *givenData);
	int *givenData() const;

public slots:
	bool openFile(const QString &fileName);
	bool saveToFile(const QString &fileName);


private:

	QString *mGridData;
	int *mGivenData;


	};

#endif // SUDOKUTABLEMODEL_H

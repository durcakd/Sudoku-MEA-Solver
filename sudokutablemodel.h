#ifndef SUDOKUTABLEMODEL_H
#define SUDOKUTABLEMODEL_H

#include <QAbstractTableModel>

class SudokuTableModel : public QAbstractTableModel
	{
	Q_OBJECT
public:
	explicit SudokuTableModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:

	};

#endif // SUDOKUTABLEMODEL_H

#include "sudokutablemodel.h"
#include <QDebug>
#include <QFont>
#include <QBrush>
#include <QFile>

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
	if (!index.isValid())
		return QVariant();

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
			boldFont.setWeight(QFont::Black);
			return boldFont;
		}
		break;

	case Qt::ForegroundRole:
		if (mGivenData[dd(row,col)] == 0) {
			return QBrush( Qt::darkGreen );
		}
		break;

		// all cell are centered

		// background NxN cells together
	case Qt::BackgroundRole:
		if(row < 3 || row > 5){
			if(col < 3 || col > 5){
				return QBrush( Qt::lightGray );
			}
		} else if(col > 2 && col < 6){
			return QBrush( Qt::lightGray );
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
	if (index.isValid() && role == Qt::EditRole)
	{
		//save value from editor to member m_gridData

		QString str = value.toString();
		bool ok = false;
		int num = str.toInt(&ok);

		if(str.size() != 1 || !ok){
			return false;
		} else if(num == 0){
			str = "";
		}

		mGridData[dd(index.row(),index.column())] = str;

		emit dataChanged(index, index);
		return true;
	}
	return false;
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


// set given data
void SudokuTableModel::setGivenData(const int *givenData){
	if(givenData != NULL){
		memcpy(mGivenData, givenData, N4*sizeof(int));
	}
	for(int i = 0; i < N4; i++){
		if(mGivenData[i] == 0) {
			mGridData[i] = "";
		}else{
			mGridData[i] = QString::number(mGivenData[i]);
		}
	}
	reset();
	emit sentStatusMsg( tr("Sudoku successfully loaded"), 5000 );
}

// get given data
int *SudokuTableModel::givenData() const{
	return mGivenData;
}



// PUBLIC SLOTS:
// open file
bool SudokuTableModel::openFile(const QString &fileName){
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "Cannot open file for reading: "
				 << qPrintable(file.errorString());
		emit sentStatusMsg( tr("Can not open file for reading"), 5000 );
		return false;
	}

	QTextStream in(&file);
	int counter = 0;
	int given[N4];

	while (!in.atEnd()) {
		QString line = in.readLine();
		QStringList fields = line.trimmed().split(' ');
		//qDebug() << line.trimmed() << " size: " << fields.size();
		if (fields.size() == N2) {
			for (int j = 0; j < N2; j++){
				given[counter++] = fields.at(j).toInt();
			}
			if(counter >= N4){
				setGivenData(given);
				file.close();
				return true;
			}
		}
	}
	setGivenData(given);
	file.close();
	return true;
}

// save to file
bool SudokuTableModel::saveToFile(const QString &fileName){

	// prepare output fileName
	//QFileInfo fileInfo(fileName);
	//QString outFileName = fileInfo.absolutePath() + "/" +
	//		fileInfo.completeBaseName() + "_converted." + fileInfo.suffix();


	QFile outFile(fileName);
	if (!outFile.open(QIODevice::WriteOnly)) {
		qDebug() << "Cannot open file for writing: "
				 << qPrintable(outFile.errorString());
		emit sentStatusMsg( tr("Can not open file for writing"), 5000 );
		return false;
	}

	QTextStream out(&outFile);
	out.setCodec("UTF-8");

	for(int i = 0; i < N4; i++){
		if (mGridData[i] == ""){
			out << "0 ";
		}else{
			out << mGridData[i] << " ";
		}

		if((i+1)%N2 == 0){
			out << endl;
		}
	}

	outFile.close();
	emit sentStatusMsg( tr("Sudoku successfully saved"), 5000 );
	return true;
}

// clean
void SudokuTableModel::clean(){
	for(int i = 0; i < N4; i++){
		mGivenData[i] = 0;
		mGridData[i] = "";
	}
	reset();
	emit sentStatusMsg( tr("Empty Sudoku prepared"), 5000 );
}

// confirm
void SudokuTableModel::confirm(const bool ok){

	// confirm - have to be always
	for(int i = 0; i < N4; i++){
		if (mGridData[i] == ""){
			mGivenData[i] = 0;
		}else{
			mGivenData[i] = mGridData[i].toInt();
		}
	}

	if (!ok) {  // if unconfirm
		for(int i = 0; i < N4; i++){
			mGivenData[i] = 0;
		}
	}
	reset();
}

// set grid data
void SudokuTableModel::setGridData(const QStringList &list){
	if(list.size() == ROWS * COLS){

		for( int i = 0;  i < ROWS * COLS; i++ ){
			mGridData[i] = list.at(i);
		}

	} else {
		qDebug() << "SudokuTableModel:setGridData(): Error: get list with wrong size.";
	}
	this->reset();
}

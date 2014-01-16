#include "sudokudialog.h"
#include <QHeaderView>
#include <QDebug>
#include <QGroupBox>
#include <QGridLayout>
#include <QFileDialog>

// constructor
SudokuDialog::SudokuDialog(QWidget *parent) :
	QDialog(parent)
{
	tableView = new QTableView;
	sudokuTableModel = NULL;
	//openFileTB	= new QToolButton;
	//saveToFileTB = new QToolButton;
	startTB		= new QToolButton;
	confirmTB	= new QToolButton;
	newTB		= new QToolButton;
	parametersTB = new QToolButton;
	listTB		= new QToolButton;

	easyB		= new QPushButton(tr("Easy"));
	mediumB		= new QPushButton(tr("Medium"));
	hardB		= new QPushButton(tr("Hard"));


	confirmIcon		= new QIcon(":/images/confirm.png");
	unconfirmIcon	= new QIcon(":/images/unconfirm.png");
	//openFileTB->setIcon(QIcon(":/images/open.png"));
	//saveToFileTB->setIcon(QIcon(":/images/save.png"));
	startTB->setIcon(QIcon(":/images/start.png"));
	confirmTB->setIcon(*confirmIcon);
	newTB->setIcon(QIcon(":/images/new.png"));
	parametersTB->setIcon(QIcon(":/images/parameters.png"));
	listTB->setIcon(QIcon(":/images/list.png"));

	startB		= new QPushButton(tr("Start"));
	openFileB	= new QPushButton(tr("Open from file"));
	saveToFileB	= new QPushButton(tr("Save to File"));
	statusBarLE	= new QLineEdit;
	listWidged	= new QListWidget;


	popSizeLE		= new QLineEdit;
	elitSizeLE		= new QLineEdit;
	lifespanLE		= new QLineEdit;
	birthPeriodLE	= new QLineEdit;
	milestonePeriodLE = new QLineEdit;
	localTrialsLE	= new QLineEdit;
	maxCallsLE		= new QLineEdit;

	QLabel *popSizeL		= new QLabel(tr("Population size"));
	QLabel *elitSizeL		= new QLabel(tr("Elite list size"));
	QLabel *lifespanL		= new QLabel(tr("Lifespan"));
	QLabel *birthPeriodL	= new QLabel(tr("Birth period"));
	QLabel *milestonePeriodL = new QLabel(tr("Milestone period"));
	QLabel *localTrialsL	= new QLabel(tr("Local search trials"));
	QLabel *maxCallsL		= new QLabel(tr("Max trials (fitness calls)"));


	// layovting
	QGridLayout *paramLayout = new QGridLayout;
	paramLayout->addWidget( popSizeL,0,0 );
	paramLayout->addWidget( elitSizeL,1,0 );
	paramLayout->addWidget( lifespanL,2,0 );
	paramLayout->addWidget( birthPeriodL,3,0 );
	paramLayout->addWidget( milestonePeriodL,4,0 );
	paramLayout->addWidget( localTrialsL,5,0 );
	paramLayout->addWidget( maxCallsL,6,0 );
	//labelLayout->addStretch();

	paramLayout->addWidget( popSizeLE,0,1 );
	paramLayout->addWidget( elitSizeLE,1,1 );
	paramLayout->addWidget( lifespanLE,2,1 );
	paramLayout->addWidget( birthPeriodLE,3,1 );
	paramLayout->addWidget( milestonePeriodLE,4,1 );
	paramLayout->addWidget( localTrialsLE,5,1 );
	paramLayout->addWidget( maxCallsLE,6,1 );
	//editLineLayout->addStretch();

	QGroupBox *parametersGroup = new QGroupBox("MEA parameters" );
	parametersGroup->setLayout(paramLayout);


	QHBoxLayout *controlLayout = new QHBoxLayout;
	controlLayout->addWidget( tableView );
	controlLayout->addWidget( parametersGroup);

	// ----

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget( newTB );
	//buttonLayout->addWidget( openFileTB );
	//buttonLayout->addWidget( saveToFileTB );
	buttonLayout->addWidget( confirmTB );

	buttonLayout->addWidget( openFileB );
	buttonLayout->addWidget( saveToFileB );
	buttonLayout->addWidget( parametersTB );
	buttonLayout->addStretch();

	buttonLayout->addWidget( easyB );
	buttonLayout->addWidget( mediumB );
	buttonLayout->addWidget( hardB );


	QHBoxLayout *button2Layout = new QHBoxLayout;
	button2Layout->addWidget( startTB );
	button2Layout->addWidget( startB );

	button2Layout->addStretch();
	button2Layout->addWidget( listTB );

	// -----



	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout( controlLayout );
	mainLayout->addLayout( buttonLayout );
	mainLayout->addLayout( button2Layout );
	mainLayout->addWidget( statusBarLE );
	mainLayout->addWidget( listWidged );

	setLayout(mainLayout);
	layout()->setSizeConstraint(QLayout::SetFixedSize);
	statusBarLE->setEnabled(false);
	setWindowTitle(tr("Sudoku MEA Solver"));
	//
	parametersTB->setCheckable(true);
	parametersGroup->hide();

	listTB->setCheckable(true);
	listWidged->hide();

	easyB->hide();
	mediumB->hide();
	hardB->hide();

	connect( this->parametersTB, SIGNAL(toggled(bool)),
			 parametersGroup, SLOT(setVisible(bool)));
	connect( this->parametersTB, SIGNAL(toggled(bool)),
			 easyB, SLOT(setVisible(bool)));
	connect( this->parametersTB, SIGNAL(toggled(bool)),
			 mediumB, SLOT(setVisible(bool)));
	connect( this->parametersTB, SIGNAL(toggled(bool)),
			 hardB, SLOT(setVisible(bool)));

	connect( listTB, SIGNAL(toggled(bool)),
			 listWidged, SLOT(setVisible(bool)));


	// contect slots
	connect( openFileB, SIGNAL(clicked()),
			 this, SLOT(open()));
	connect( saveToFileB, SIGNAL(clicked()),
			 this, SLOT(save()));

	confirmTB->setCheckable(true);
	connect( confirmTB, SIGNAL(toggled(bool)),
			 this, SLOT(confirm(bool)));
	connect( newTB, SIGNAL(clicked()),
			 this, SLOT(on_newTB_clicked()));

	connect( startB, SIGNAL(clicked()),
			 this, SLOT(start()));
	connect( startTB, SIGNAL(clicked()),
			 this, SLOT(start()));

	// parrameters
	connect( easyB, SIGNAL(clicked()),
			 this, SLOT(setEasy()) );
	connect( mediumB, SIGNAL(clicked()),
			 this, SLOT(setMedium()) );
	connect( hardB, SIGNAL(clicked()),
			 this, SLOT(setHard()) );


	pripareParametersLE();
}

// SLOTs for parametre's line edits to change parameters variables
void SudokuDialog::on_popSizeLE_textChanged(const QString &str){
	if(popSizeLE->hasAcceptableInput()){
		parm.popSize = str.toInt();
	}
}
void SudokuDialog::on_elitSizeLE_textChanged(const QString &str){
	if(elitSizeLE->hasAcceptableInput()){
		parm.elitSize = str.toInt();
	}
}
void SudokuDialog::on_lifespanLE_textChanged(const QString &str){
	if(lifespanLE->hasAcceptableInput()){
		parm.lifespan = str.toInt();
	}
}
void SudokuDialog::on_birthPeriodLE_textChanged(const QString &str){
	if(birthPeriodLE->hasAcceptableInput()){
		parm.birthPeriod = str.toInt();
	}
}
void SudokuDialog::on_milestonePeriodLE_textChanged(const QString &str){
	if(milestonePeriodLE->hasAcceptableInput()){
		parm.milestonePeriod = str.toInt();
	}
}
void SudokuDialog::on_localTrialsLE_textChanged(const QString &str){
	if(localTrialsLE->hasAcceptableInput()){
		parm.localTrials = str.toInt();
	}
}
void SudokuDialog::on_maxCallsLE_textChanged(const QString &str){
	if(maxCallsLE->hasAcceptableInput()){
		parm.maxCalls = str.toInt();
	}
}

// preparing parameters edit lines
void SudokuDialog::pripareParametersLE(){

	// parameter's line edit seting validator
	QIntValidator *validator = new QIntValidator(1, 1000000000, this);
	popSizeLE->setValidator(validator);
	elitSizeLE->setValidator(validator);
	lifespanLE->setValidator(validator);
	birthPeriodLE->setValidator(validator);
	milestonePeriodLE->setValidator(validator);
	localTrialsLE->setValidator(validator);
	maxCallsLE->setValidator(validator);

	// connect parameter's line edit to change parameter's variables
	connect( popSizeLE, SIGNAL(textChanged(QString)),
			 this, SLOT(on_popSizeLE_textChanged(QString)));
	connect( elitSizeLE, SIGNAL(textChanged(QString)),
			 this, SLOT(on_elitSizeLE_textChanged(QString)));
	connect( lifespanLE, SIGNAL(textChanged(QString)),
			 this, SLOT(on_lifespanLE_textChanged(QString)));
	connect( birthPeriodLE, SIGNAL(textChanged(QString)),
			 this, SLOT(on_birthPeriodLE_textChanged(QString)));
	connect( milestonePeriodLE, SIGNAL(textChanged(QString)),
			 this, SLOT(on_milestonePeriodLE_textChanged(QString)));
	connect( localTrialsLE, SIGNAL(textChanged(QString)),
			 this, SLOT(on_localTrialsLE_textChanged(QString)));
	connect( maxCallsLE, SIGNAL(textChanged(QString)),
			 this, SLOT(on_maxCallsLE_textChanged(QString)));

	// set default values of parameters
	setEasy();
}

void SudokuDialog::setEasy(){
	popSizeLE->setText( QString::number( EPOPSIZE ));
	elitSizeLE->setText( QString::number( EELITESIZE ));
	lifespanLE->setText( QString::number( ELIFESPAN ));
	birthPeriodLE->setText( QString::number( EBIRTHPERIOD ));
	milestonePeriodLE->setText( QString::number( EMILESTONEPERIOD ));
	localTrialsLE->setText( QString::number( ELOCALTRIALS ));
	maxCallsLE->setText( QString::number( EMAXCALLS ));
}
void SudokuDialog::setMedium(){
	popSizeLE->setText( QString::number( MPOPSIZE ));
	elitSizeLE->setText( QString::number( MELITESIZE ));
	lifespanLE->setText( QString::number( MLIFESPAN ));
	birthPeriodLE->setText( QString::number( MBIRTHPERIOD ));
	milestonePeriodLE->setText( QString::number( MMILESTONEPERIOD ));
	localTrialsLE->setText( QString::number( MLOCALTRIALS ));
	maxCallsLE->setText( QString::number( MMAXCALLS ));
}
void SudokuDialog::setHard(){
	popSizeLE->setText( QString::number( HPOPSIZE ));
	elitSizeLE->setText( QString::number( HELITESIZE ));
	lifespanLE->setText( QString::number( HLIFESPAN ));
	birthPeriodLE->setText( QString::number( HBIRTHPERIOD ));
	milestonePeriodLE->setText( QString::number( HMILESTONEPERIOD ));
	localTrialsLE->setText( QString::number( HLOCALTRIALS ));
	maxCallsLE->setText( QString::number( HMAXCALLS ));
}

void SudokuDialog::setTableModel( QAbstractTableModel *model ){

	sudokuTableModel = static_cast<SudokuTableModel*>(model);

	tableView->setModel( model );
	//tableView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	tableView->resizeColumnsToContents();
	tableView->resizeRowsToContents();
	tableView->setFixedHeight(tableView->rowHeight(0)*9 + 2);
	tableView->setFixedWidth(tableView->columnWidth(0)*9 + 2);
	tableView->verticalHeader()->setVisible(false);
	tableView->horizontalHeader()->setVisible(false);

	tableView->setSelectionMode(QAbstractItemView::SingleSelection);

}

// SLOT add str to list widged
void SudokuDialog::addStrToListWidged(const QString &str){
	if(!str.isEmpty()){
		listWidged->addItem(str);
		listWidged->scrollToBottom();
	}
}

// PRIVATE SLOTS:
// open
bool SudokuDialog::open(){
	const QString fileFilters = tr("Text files (*.txt)\n"
								   "Sudoku files (*.sud)\n"
								   "All files (*)");

	QString fileName =
			QFileDialog::getOpenFileName(this, tr("Open"), ".", fileFilters);
	if (fileName.isEmpty()){
		return false;
	}

	confirmTB->setChecked(false);
	emit requestForReadFile(fileName);

	return true;
}

// save
bool SudokuDialog::save(){
	const QString fileFilters = tr("Text files (*.txt)\n"
								   "Sudoku files (*.sud)\n");

	QString fileName =
			QFileDialog::getSaveFileName(this, tr("Save"), ".", fileFilters);
	if (fileName.isEmpty()){
		return false;
	}

	emit requestForSaveFile(fileName);
	return true;
}

// confirm
void SudokuDialog::confirm(const bool ok){
	if(ok){
		confirmTB->setIcon(*unconfirmIcon);
	}else{
		confirmTB->setIcon(*confirmIcon);
	}
	emit requestForConfirm(!ok);
}

// new clear
void SudokuDialog::on_newTB_clicked(){
	confirmTB->setChecked(true);
}

// start thread
void  SudokuDialog::start(){
	if(sudokuTableModel == NULL){
		qDebug() << "Error: dialog.start() called without set table model before";
		return;
	}

	startB->setEnabled(false);
	startTB->setEnabled(false);

	if( !thread.isRunning()){
		thread.setParameters( parm, sudokuTableModel->givenData() );
		thread.start();
	}
}
// get result from thread
void  SudokuDialog::threadDone(const QString msg){
	addStrToListWidged(msg);
	startB->setEnabled(true);
	startTB->setEnabled(true);

}

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
	givenData = new int[81];


	tableView = new QTableView;
	sudokuTableModel = new SudokuTableModel;

	openFileB	= new QPushButton(tr("Open from file"));
	saveToFileB	= new QPushButton(tr("Save to File"));
	startB		= new QPushButton(tr("Start"));
	parametersB	= new QPushButton(tr("Par"));
	listB		= new QPushButton(tr("Info"));
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
	buttonLayout->addWidget( openFileB );
	buttonLayout->addWidget( startB );
	buttonLayout->addStretch();
	buttonLayout->addWidget( parametersB );

	QHBoxLayout *button2Layout = new QHBoxLayout;
	button2Layout->addWidget( saveToFileB );
	button2Layout->addStretch();
	button2Layout->addWidget( listB );
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
	parametersB->setCheckable(true);
	parametersGroup->hide();

	listB->setCheckable(true);
	listWidged->hide();

	connect( this->parametersB, SIGNAL(toggled(bool)),
			 parametersGroup, SLOT(setVisible(bool)));
	connect( listB, SIGNAL(toggled(bool)),
			 listWidged, SLOT(setVisible(bool)));

	// contect slots
	connect( openFileB, SIGNAL(clicked()),
			 this, SLOT(open()));

	pripareParametersLE();
}

// SLOTs for parametre's line edits to change parameters variables
void SudokuDialog::on_popSizeLE_textChanged(const QString &str){
	if(popSizeLE->hasAcceptableInput()){
		popSize = str.toInt();
	}
}
void SudokuDialog::on_elitSizeLE_textChanged(const QString &str){
	if(elitSizeLE->hasAcceptableInput()){
		elitSize = str.toInt();
	}
}
void SudokuDialog::on_lifespanLE_textChanged(const QString &str){
	if(lifespanLE->hasAcceptableInput()){
		lifespan = str.toInt();
	}
}
void SudokuDialog::on_birthPeriodLE_textChanged(const QString &str){
	if(birthPeriodLE->hasAcceptableInput()){
		birthPeriod = str.toInt();
	}
}
void SudokuDialog::on_milestonePeriodLE_textChanged(const QString &str){
	if(milestonePeriodLE->hasAcceptableInput()){
		milestonePeriod = str.toInt();
	}
}
void SudokuDialog::on_localTrialsLE_textChanged(const QString &str){
	if(localTrialsLE->hasAcceptableInput()){
		localTrials = str.toInt();
	}
}
void SudokuDialog::on_maxCallsLE_textChanged(const QString &str){
	if(maxCallsLE->hasAcceptableInput()){
		maxCalls = str.toInt();
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
	popSizeLE->setText( QString::number( POPSIZE ));
	elitSizeLE->setText( QString::number( ELITESIZE ));
	lifespanLE->setText( QString::number( LIFESPAN ));
	birthPeriodLE->setText( QString::number( BIRTHPERIOD ));
	milestonePeriodLE->setText( QString::number( MILESTONEPERIOD ));
	localTrialsLE->setText( QString::number( LOCALTRIALS ));
	maxCallsLE->setText( QString::number( MAXCALLS ));

}

void SudokuDialog::setTableModel(QAbstractTableModel *model ) const{
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


// open SLOT
bool SudokuDialog::open(){
	const QString fileFilters = tr("Text files (*.txt)\n"
								   "Sudoku files (*.sud)\n"
								   "All files (*)");

	QString fileName =
			QFileDialog::getOpenFileName(this, tr("Open"), ".", fileFilters);
	if (fileName.isEmpty()){
		return false;
	}

	emit requestForReadFile(fileName);
	return true;
}

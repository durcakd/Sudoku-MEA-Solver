#include "sudokudialog.h"
#include <QHeaderView>


SudokuDialog::SudokuDialog(QWidget *parent) :
	QDialog(parent)
{

	tableView = new QTableView;
	sudokuTableModel = new SudokuTableModel;
	tableView->setModel(sudokuTableModel);
	//tableView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	tableView->resizeColumnsToContents();
	tableView->resizeRowsToContents();
	tableView->setFixedHeight(tableView->rowHeight(0)*9 + 2);
	tableView->setFixedWidth(tableView->columnWidth(0)*9 + 2);
	tableView->verticalHeader()->setVisible(false);
	tableView->horizontalHeader()->setVisible(false);


	openFileB	= new QPushButton(tr("Open from file"));
	saveToFileB	= new QPushButton(tr("Save to File"));
	startB		= new QPushButton(tr("Start"));
	statusBarLE		= new QLineEdit;

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
	QVBoxLayout *labelLayout = new QVBoxLayout;
	labelLayout->addWidget( popSizeL );
	labelLayout->addWidget( elitSizeL );
	labelLayout->addWidget( lifespanL );
	labelLayout->addWidget( birthPeriodL );
	labelLayout->addWidget( milestonePeriodL );
	labelLayout->addWidget( localTrialsL );
	labelLayout->addWidget( maxCallsL );

	QVBoxLayout *editLineLayout = new QVBoxLayout;
	editLineLayout->addWidget( popSizeLE );
	editLineLayout->addWidget( elitSizeLE );
	editLineLayout->addWidget( lifespanLE );
	editLineLayout->addWidget( birthPeriodLE );
	editLineLayout->addWidget( milestonePeriodLE );
	editLineLayout->addWidget( localTrialsLE );
	editLineLayout->addWidget( maxCallsLE );

	QVBoxLayout *buttonLayout = new QVBoxLayout;
	buttonLayout->addWidget( openFileB );
	buttonLayout->addWidget( saveToFileB );
	buttonLayout->addWidget( startB );
	buttonLayout->addStretch();

	QHBoxLayout *controlLayout = new QHBoxLayout;
	controlLayout->addLayout( labelLayout );
	controlLayout->addLayout( editLineLayout );
	controlLayout->addLayout( buttonLayout );


	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget( tableView );
	mainLayout->addLayout( controlLayout );
	mainLayout->addWidget( statusBarLE );

	setLayout(mainLayout);

	statusBarLE->setEnabled(false);
	setWindowTitle(tr("Sudoku MEA Solver"));

	//

}

#include "setuppageworldeditor.h"



SetupPageWorldEditor::SetupPageWorldEditor(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent)
{
     //qDebug() << "SetupPageWorldEditor::SetupPageWorldEditor"  << endl;
    //worldPanel = new QWidget;
    dataProxy = dp;
    world = new World(dataProxy, Q_FUNC_INFO);
    util = new Utilities();

    setupEntityDialog = new SetupEntityDialog();

    worldModel = new QSqlRelationalTableModel(this);
    worldView = new QTableView;
    worldView->setContextMenuPolicy(Qt::CustomContextMenu);
    worldView->setSortingEnabled(true);


    createWorldModel();
    createWorldPanel();

    worldView->setCurrentIndex(worldModel->index(0, 0));

    addEntityPushButton = new QPushButton;
    delEntityPushButton = new QPushButton;
    editEntityPushButton = new QPushButton;
    exportWorldPushButton  = new QPushButton;
    loadWorldPushButton  = new QPushButton;

    addEntityPushButton->setText(tr("Add"));
    delEntityPushButton->setText(tr("Delete"));
    editEntityPushButton->setText(tr("Edit"));

    exportWorldPushButton->setText(tr("Export World"));
    loadWorldPushButton->setText(tr("Import World"));

    addEntityPushButton->setEnabled(false);
    delEntityPushButton->setEnabled(false);
    editEntityPushButton->setEnabled(false);

    addEntityPushButton->setToolTip(tr("Still not implemented."));
    delEntityPushButton->setToolTip(tr("Still not implemented."));
    editEntityPushButton->setToolTip(tr("Still not implemented."));

    exportWorldPushButton->setEnabled(false);
    loadWorldPushButton->setEnabled(true);
    exportWorldPushButton->setToolTip(tr("Still not implemented."));
    loadWorldPushButton->setToolTip(tr("Import a new cty.csv file"));


    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(exportWorldPushButton);
    buttonsLayout->addWidget(loadWorldPushButton);
    buttonsLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));
    buttonsLayout->addWidget(addEntityPushButton);
    buttonsLayout->addWidget(editEntityPushButton);
    buttonsLayout->addWidget(delEntityPushButton);
/*
    QHBoxLayout *wbuttonsLayout = new QHBoxLayout;
    wbuttonsLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));
    wbuttonsLayout->addWidget(exportWorldPushButton);
    wbuttonsLayout->addWidget(loadWorldPushButton);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addLayout(wbuttonsLayout);
    buttonsLayout->addLayout(ebuttonsLayout);
*/
    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(worldView);
    layout->addLayout(buttonsLayout);

    setLayout(layout);

    createActions();

    if (isWorldEmpty())
    {
        QString ctyfile = util->getCTYFile();

        QMessageBox msgBox;
        if (QFile::exists(ctyfile))
        {

            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(tr("An entities information file (cty.csv) has been detected in your KLog folder and will be loaded."));
            msgBox.exec();
            world->recreate(ctyfile);
            worldModel->select();
            //slotImportWorldButtonClicked();
        }
        else
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("No entities information file (cty.csv) has been detected in your KLog folder."));
            msgBox.setInformativeText(tr("KLog will not be able to show entities information."));
            msgBox.exec();
        }

    }
     //qDebug() << "SetupPageWorldEditor::SetupPageWorldEditor - END"  << endl;
}

SetupPageWorldEditor::~SetupPageWorldEditor()
{
     //qDebug() << "SetupPageWorldEditor::~SetupPageWorldEditor"  << endl;

}

void SetupPageWorldEditor::createWorldPanel()
{

    worldView->setModel(worldModel);
    QString stringQuery = QString("SELECT * FROM entity");
    QSqlQuery query(stringQuery);
    QSqlRecord rec = query.record(); // Number of columns
    int columns = rec.count();

    for (int i = 0; i < columns; i++ ){
        worldView->setColumnHidden(i, true);
    }

    columns = rec.indexOf("mainprefix");
    worldView->setColumnHidden(columns, false);
    columns = rec.indexOf("name");
    worldView->setColumnHidden(columns, false);
    columns = rec.indexOf("dxcc");
    worldView->setColumnHidden(columns, false);
    columns = rec.indexOf("continent");
    worldView->setColumnHidden(columns, false);
    columns = rec.indexOf("cqz");
    worldView->setColumnHidden(columns, false);
    columns = rec.indexOf("ituz");
    worldView->setColumnHidden(columns, false);
    columns = rec.indexOf("utc");
    worldView->setColumnHidden(columns, false);
    columns = rec.indexOf("latitude");
    worldView->setColumnHidden(columns, false);
    columns = rec.indexOf("longitude");
    worldView->setColumnHidden(columns, false);
    columns = rec.indexOf("deleted");
    worldView->setColumnHidden(columns, false);
    columns = rec.indexOf("sincedate");
    worldView->setColumnHidden(columns, false);
    columns = rec.indexOf("todate");
    worldView->setColumnHidden(columns, false);


    worldView->setItemDelegate(new QSqlRelationalDelegate(this));
    worldView->setSelectionMode( QAbstractItemView::SingleSelection);
    worldView->setSelectionBehavior(QAbstractItemView::SelectRows);
    worldView->resizeColumnsToContents();
    worldView->horizontalHeader()->setStretchLastSection(true);

}

void SetupPageWorldEditor::createWorldModel()
{
/*
    WORLD_DXCCid = 0,
    WORLD_Nameid = 1,
    WORLD_MainPrefix = 2,
    WORLD_CQZ = 3,
    WORLD_ITUZ = 4,
    WORLD_Cont = 5

*/

    QString stringQuery = QString("SELECT * FROM entity");
    QSqlQuery q(stringQuery);
    QSqlRecord rec = q.record();

    int nameCol;

    //worldModel = new QSqlRelationalTableModel(this);
    worldModel->setTable("entity");
    worldModel->setEditStrategy(QSqlTableModel::OnFieldChange);

    nameCol = rec.indexOf("mainprefix");
    worldModel->setSort(nameCol, Qt::AscendingOrder);

    worldModel->setHeaderData(nameCol, Qt::Horizontal, tr("Prefix"));
    nameCol = rec.indexOf("name");
    worldModel->setHeaderData(nameCol, Qt::Horizontal, tr("Entity"));
    nameCol = rec.indexOf("dxcc");
    worldModel->setHeaderData(nameCol, Qt::Horizontal, tr("ARRL ID"));
    nameCol = rec.indexOf("continent");
    worldModel->setHeaderData(nameCol, Qt::Horizontal, tr("Continent"));
    nameCol = rec.indexOf("cqz");
    worldModel->setHeaderData(nameCol, Qt::Horizontal, tr("CQ Zone"));
    nameCol = rec.indexOf("ituz");
    worldModel->setHeaderData(nameCol, Qt::Horizontal, tr("ITU Zone"));
    nameCol = rec.indexOf("utc");
    worldModel->setHeaderData(nameCol, Qt::Horizontal, tr("UTC"));
    nameCol = rec.indexOf("latitude");
    worldModel->setHeaderData(nameCol, Qt::Horizontal, tr("Latitude"));
    nameCol = rec.indexOf("longitude");
    worldModel->setHeaderData(nameCol, Qt::Horizontal, tr("Longitude"));

    nameCol = rec.indexOf("deleted");
    worldModel->setHeaderData(nameCol, Qt::Horizontal, tr("Deleted"));

    nameCol = rec.indexOf("sincedate");
    worldModel->setHeaderData(nameCol, Qt::Horizontal, tr("Since Date"));
    nameCol = rec.indexOf("todate");
    worldModel->setHeaderData(nameCol, Qt::Horizontal, tr("To Date"));

    worldModel->select();
}

void SetupPageWorldEditor::createActions()
{
     //qDebug() << "SetupPageWorldEditor::createActions" << endl;
    connect(addEntityPushButton, SIGNAL(clicked()), this, SLOT(slotAddButtonClicked()) );
    connect(delEntityPushButton, SIGNAL(clicked()), this, SLOT(slotDelButtonClicked()) );
    connect(editEntityPushButton, SIGNAL(clicked()), this, SLOT(slotEditButtonClicked()) );

    connect(loadWorldPushButton, SIGNAL(clicked()), this, SLOT(slotImportWorldButtonClicked()) );




    //connect(worldView, SIGNAL(doubleClicked ( const QModelIndex& ) ), this, SLOT(slotDoubleClickEntity( const QModelIndex& ) ) );

    //SIGNAL received from the setupEntityDialog
    // void entityAdded(const QStringList _qs); // entity
    connect(setupEntityDialog, SIGNAL(entityAdded(QStringList)), this, SLOT(slotAnalyzeEntityAddedSignal(QStringList) ) );

}

void SetupPageWorldEditor::slotAnalyzeEntityAddedSignal(const QStringList _qs)
{
    /*
     //qDebug() << "SetupPageWorldEditor::slotAnalyzeEntityAddedSignal\n" <<
                _qs.at(0) << "\n" <<
                _qs.at(1) << "\n" <<
                _qs.at(2) << "\n" <<
                _qs.at(3) << "\n" <<
                _qs.at(4) << "\n" <<
                _qs.at(5) << "\n" <<
                _qs.at(6) << "\n" <<
                _qs.at(7) << "\n" <<
                _qs.at(8) << "\n" <<
                _qs.at(9) << "\n" <<
                _qs.at(10) << "\n" << endl;
                */
}

bool SetupPageWorldEditor::isWorldEmpty()
{
    // I need to check if the world is empty and I have the CTY.CSV file
    if (world->getHowManyEntities()<1)
    {
        return true;
    }
    else
    {
       return false;
    }
    //return true;
}

void SetupPageWorldEditor::slotAddButtonClicked()
{
     //qDebug() << "SetupPageWorldEditor::slotAddButtonClicked" << endl;
    setupEntityDialog->exec();
    //TODO
}

void SetupPageWorldEditor::slotDelButtonClicked()
{
     //qDebug() << "SetupPageWorldEditor::slotDelButtonClicked" << endl;
    //TODO
}

void SetupPageWorldEditor::slotEditButtonClicked()
{
     //qDebug() << "SetupPageWorldEditor::slotEditButtonClicked" << endl;
    //TODO
}

/*
void SetupPageWorldEditor::slotDoubleClickEntity( const QModelIndex & index)
{
     //qDebug() << "SetupPageWorldEditor::slotDoubleClickEntity" << endl;
    //TODO

    //QSqlQuery query;
    //QString queryString;
    //int row = index.row();

}
*/

void SetupPageWorldEditor::slotImportWorldButtonClicked()
{
     //qDebug() << "SetupPageWorldEditor::slotImportWorldButtonClicked" << endl;
    QString klogDir;
    klogDir = util->getHomeDir();
    QString worldFile;
    worldFile.clear();
    worldFile = QFileDialog::getOpenFileName(this, tr("Open File"), klogDir, tr("BigCTY (*.csv)"));

    QMessageBox msgBox;
     //qDebug() << "SetupPageWorldEditor::slotImportWorldButtonClicked: " << worldFile << endl;

    if (world->recreate(worldFile) )
    {

        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("Entities information has been updated."));
        worldModel->select();
    }
    else
    {
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Entities information has not been updated."));
    }


    msgBox.exec();
     //qDebug() << "SetupPageWorldEditor::slotImportWorldButtonClicked - END" << endl;
}



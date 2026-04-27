/***************************************************************************
                          setuppageworldeditor.h  -  description
                             -------------------
    begin                : jun 2012
    copyright            : (C) 2012 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

#include "setuppageworldeditor.h"
#include "../callsign.h"

// ---------------------------------------------------------------------------
// AddSpecialCallsignDialog implementation
// ---------------------------------------------------------------------------

AddSpecialCallsignDialog::AddSpecialCallsignDialog(World *w, QWidget *parent)
    : QDialog(parent), world(w)
{
    setWindowTitle(tr("Add Special Callsign"));

    callLineEdit = new QLineEdit(this);
    callLineEdit->setPlaceholderText(tr("e.g. RI1ANY"));

    entityCombo = new QComboBox(this);
    {
        QSqlQuery q;
        q.setForwardOnly(true);
        if (q.exec("SELECT name, dxcc FROM entity WHERE (deleted IS NULL OR deleted=0) ORDER BY name"))
        {
            while (q.next())
                entityCombo->addItem(q.value(0).toString(), q.value(1).toInt());
        }
    }

    cqzCheck = new QCheckBox(tr("Override CQ Zone:"), this);
    cqzSpin  = new QSpinBox(this);
    cqzSpin->setRange(1, 40);
    cqzSpin->setEnabled(false);
    connect(cqzCheck, &QCheckBox::toggled, cqzSpin, &QSpinBox::setEnabled);

    ituzCheck = new QCheckBox(tr("Override ITU Zone:"), this);
    ituzSpin  = new QSpinBox(this);
    ituzSpin->setRange(1, 90);
    ituzSpin->setEnabled(false);
    connect(ituzCheck, &QCheckBox::toggled, ituzSpin, &QSpinBox::setEnabled);

    // Proper slot — fires every time the entity combo changes
    connect(entityCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AddSpecialCallsignDialog::slotEntityChanged);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *cqzRow = new QHBoxLayout;
    cqzRow->addWidget(cqzCheck);
    cqzRow->addWidget(cqzSpin);
    cqzRow->addStretch();

    auto *ituzRow = new QHBoxLayout;
    ituzRow->addWidget(ituzCheck);
    ituzRow->addWidget(ituzSpin);
    ituzRow->addStretch();

    auto *form = new QFormLayout;
    form->addRow(tr("Callsign:"), callLineEdit);
    form->addRow(tr("Entity:"),   entityCombo);
    form->addRow(cqzRow);
    form->addRow(ituzRow);
    form->addRow(buttonBox);

    setLayout(form);

    // Populate zone spinboxes with the first entity's defaults
    slotEntityChanged(0);
}

void AddSpecialCallsignDialog::slotEntityChanged(int /*index*/)
{
    const int dxcc = entityCombo->currentData().toInt();
    const int cq   = world->getEntityCqz(dxcc);
    const int itu  = world->getEntityItuz(dxcc);
    if (cq  > 0) cqzSpin->setValue(cq);
    if (itu > 0) ituzSpin->setValue(itu);
}

QString AddSpecialCallsignDialog::callsign()     const { return callLineEdit->text().toUpper().trimmed(); }
int     AddSpecialCallsignDialog::selectedDxcc() const { return entityCombo->currentData().toInt(); }
int     AddSpecialCallsignDialog::cqz()          const { return cqzCheck->isChecked()  ? cqzSpin->value()  : -1; }
int     AddSpecialCallsignDialog::ituz()         const { return ituzCheck->isChecked() ? ituzSpin->value() : -1; }

SetupPageWorldEditor::SetupPageWorldEditor(DataProxy_SQLite *dp, World *injectedWorld, QWidget *parent) : QWidget(parent)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    //worldPanel = new QWidget;
    dataProxy = dp;
   //qDebug() << Q_FUNC_INFO << " - 00";
   // world = new World(dataProxy, Q_FUNC_INFO);
    world = injectedWorld;
   //qDebug() << Q_FUNC_INFO << " - 01";
    util = new Utilities(Q_FUNC_INFO);
   //qDebug() << Q_FUNC_INFO << " - 02";

    setupEntityDialog = new SetupEntityDialog();
   //qDebug() << Q_FUNC_INFO << " - 03";
    worldModel = new QSqlRelationalTableModel(this);
   //qDebug() << Q_FUNC_INFO << " - 04";
    worldView = new QTableView;
    worldView->setContextMenuPolicy(Qt::CustomContextMenu);
    worldView->setSortingEnabled(true);
   //qDebug() << Q_FUNC_INFO << " - 10";
    createWorldModel();
    createWorldPanel();
   //qDebug() << Q_FUNC_INFO << " - 20";

    worldView->setCurrentIndex(worldModel->index(0, 0));

    addEntityPushButton = new QPushButton;
    delEntityPushButton = new QPushButton;
    editEntityPushButton = new QPushButton;
    exportWorldPushButton  = new QPushButton;
    loadWorldPushButton  = new QPushButton;

    addEntityPushButton->setText(tr("Add"));
    delEntityPushButton->setText(tr("Delete"));
    editEntityPushButton->setText(tr("Edit"));

   //qDebug() << Q_FUNC_INFO << " - 30";
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
   //qDebug() << Q_FUNC_INFO << " - 40";

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(exportWorldPushButton);
    buttonsLayout->addWidget(loadWorldPushButton);
    buttonsLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));
    buttonsLayout->addWidget(addEntityPushButton);
    buttonsLayout->addWidget(editEntityPushButton);
    buttonsLayout->addWidget(delEntityPushButton);

    createSpecialCallsignsPanel();

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(worldView);
    layout->addLayout(buttonsLayout);
    layout->addWidget(specialCallsignsGroup);
   //qDebug() << Q_FUNC_INFO << " - 50";
    setLayout(layout);

    createActions();
   //qDebug() << Q_FUNC_INFO << " - 52";
    if (isWorldEmpty())
    {
       //qDebug() << Q_FUNC_INFO << " - 53";
        QString ctyfile = util->getCTYFile();
       //qDebug() << Q_FUNC_INFO << " - 54";
        QMessageBox msgBox;
        if (QFile::exists(ctyfile))
        {
           //qDebug() << Q_FUNC_INFO << " - 60";
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(tr("An entities information file (cty.csv) has been detected in your KLog folder and will be loaded."));
            msgBox.exec();
           //qDebug() << Q_FUNC_INFO << " - 65";
            world->recreate(ctyfile);
           //qDebug() << Q_FUNC_INFO << " - 66";
            worldModel->select();
           //qDebug() << Q_FUNC_INFO << " - 67";
            //slotImportWorldButtonClicked();
        }
        else
        {
           //qDebug() << Q_FUNC_INFO << " - 70";
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("No entities information file (cty.csv) has been detected in your KLog folder."));
            msgBox.setInformativeText(tr("KLog will not be able to show entities information."));
            msgBox.exec();
        }
   //qDebug() << Q_FUNC_INFO << " - 80";
    }
   //qDebug() << Q_FUNC_INFO << " - END";
}

SetupPageWorldEditor::~SetupPageWorldEditor()
{
       //qDebug() << "SetupPageWorldEditor::~SetupPageWorldEditor" ;
    //delete(world);
    delete(setupEntityDialog);
    delete(util);
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
       //qDebug() << "SetupPageWorldEditor::createActions";
    connect(addEntityPushButton, SIGNAL(clicked()), this, SLOT(slotAddButtonClicked()) );
    connect(delEntityPushButton, SIGNAL(clicked()), this, SLOT(slotDelButtonClicked()) );
    connect(editEntityPushButton, SIGNAL(clicked()), this, SLOT(slotEditButtonClicked()) );

    connect(loadWorldPushButton, SIGNAL(clicked()), this, SLOT(slotImportWorldButtonClicked()) );

    connect(addSpecialButton,    &QPushButton::clicked, this, &SetupPageWorldEditor::slotAddSpecialCallsignClicked);
    connect(removeSpecialButton, &QPushButton::clicked, this, &SetupPageWorldEditor::slotRemoveSpecialCallsignClicked);




    //connect(worldView, SIGNAL(doubleClicked ( const QModelIndex& ) ), this, SLOT(slotDoubleClickEntity( const QModelIndex& ) ) );

    //SIGNAL received from the setupEntityDialog
    // void entityAdded(const QStringList _qs); // entity
    connect(setupEntityDialog, SIGNAL(entityAdded(QStringList)), this, SLOT(slotAnalyzeEntityAddedSignal(QStringList) ) );
}

void SetupPageWorldEditor::slotAnalyzeEntityAddedSignal(const QStringList _qs)
{
    Q_UNUSED(_qs);
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
                _qs.at(10) << "\n";
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
       //qDebug() << "SetupPageWorldEditor::slotAddButtonClicked";
    setupEntityDialog->exec();
    //TODO
}

void SetupPageWorldEditor::slotDelButtonClicked()
{
       //qDebug() << "SetupPageWorldEditor::slotDelButtonClicked";
    //TODO
}

void SetupPageWorldEditor::slotEditButtonClicked()
{
       //qDebug() << "SetupPageWorldEditor::slotEditButtonClicked";
    //TODO
}

/*
void SetupPageWorldEditor::slotDoubleClickEntity( const QModelIndex & index)
{
       //qDebug() << "SetupPageWorldEditor::slotDoubleClickEntity";
    //TODO

    // qSqlQuery query;
    // qString queryString;
    //int row = index.row();

}
*/

void SetupPageWorldEditor::slotImportWorldButtonClicked()
{
       //qDebug() << "SetupPageWorldEditor::slotImportWorldButtonClicked";
    QString klogDir;
    klogDir = util->getHomeDir();
    QString worldFile;
    worldFile.clear();
    worldFile = QFileDialog::getOpenFileName(this, tr("Open File"), klogDir, tr("BigCTY (*.csv)"));

    QMessageBox msgBox;
       //qDebug() << "SetupPageWorldEditor::slotImportWorldButtonClicked: " << worldFile;

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
    refreshSpecialCallsignsTable();
       //qDebug() << "SetupPageWorldEditor::slotImportWorldButtonClicked - END";
}

void SetupPageWorldEditor::createSpecialCallsignsPanel()
{
    specialCallsignsGroup = new QGroupBox(tr("Special Callsigns"));
    specialCallsignsGroup->setToolTip(tr("Callsigns not in the CTY data that should map to a specific DXCC entity (e.g. expedition callsigns like RI1ANY for Antarctica)."));

    specialCallsignsTable = new QTableWidget(0, 4, specialCallsignsGroup);
    specialCallsignsTable->setHorizontalHeaderLabels({tr("Callsign"), tr("Entity"), tr("CQ Zone"), tr("ITU Zone")});
    specialCallsignsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    specialCallsignsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    specialCallsignsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    specialCallsignsTable->horizontalHeader()->setStretchLastSection(true);
    specialCallsignsTable->verticalHeader()->setVisible(false);

    addSpecialButton    = new QPushButton(tr("Add"));
    removeSpecialButton = new QPushButton(tr("Remove"));
    removeSpecialButton->setEnabled(false);

    connect(specialCallsignsTable, &QTableWidget::itemSelectionChanged, this, [this]() {
        removeSpecialButton->setEnabled(!specialCallsignsTable->selectedItems().isEmpty());
    });

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(addSpecialButton);
    btnLayout->addWidget(removeSpecialButton);

    QVBoxLayout *groupLayout = new QVBoxLayout;
    groupLayout->addWidget(specialCallsignsTable);
    groupLayout->addLayout(btnLayout);
    specialCallsignsGroup->setLayout(groupLayout);

    refreshSpecialCallsignsTable();
}

void SetupPageWorldEditor::refreshSpecialCallsignsTable()
{
    specialCallsignsTable->setRowCount(0);
    const auto entries = dataProxy->getSpecialCallsignPairs();
    for (const auto &e : entries)
    {
        const int row = specialCallsignsTable->rowCount();
        specialCallsignsTable->insertRow(row);
        specialCallsignsTable->setItem(row, 0, new QTableWidgetItem(e.callsign));
        specialCallsignsTable->setItem(row, 1, new QTableWidgetItem(world->getEntityName(e.dxcc)));
        specialCallsignsTable->setItem(row, 2, new QTableWidgetItem(QString::number(e.cqz)));
        specialCallsignsTable->setItem(row, 3, new QTableWidgetItem(QString::number(e.ituz)));
    }
    specialCallsignsTable->resizeColumnToContents(0);
    specialCallsignsTable->resizeColumnToContents(2);
    specialCallsignsTable->resizeColumnToContents(3);
    removeSpecialButton->setEnabled(false);
}

void SetupPageWorldEditor::slotAddSpecialCallsignClicked()
{
    AddSpecialCallsignDialog dlg(world, this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    const QString call = dlg.callsign();
    if (call.isEmpty())
        return;

    Callsign cs(call);
    if (!cs.isValid())
    {
        QMessageBox::warning(this, tr("Invalid Callsign"),
                             tr("'%1' is not a valid callsign.").arg(call));
        return;
    }

    if (!dataProxy->addSpecialCallsign(call, dlg.selectedDxcc(), dlg.cqz(), dlg.ituz()))
    {
        QMessageBox::warning(this, tr("Error"),
                             tr("Could not add the special callsign. It may already exist."));
        return;
    }

    world->readWorld();
    refreshSpecialCallsignsTable();
}

void SetupPageWorldEditor::slotRemoveSpecialCallsignClicked()
{
    const int row = specialCallsignsTable->currentRow();
    if (row < 0)
        return;

    const QString call = specialCallsignsTable->item(row, 0)->text();
    const int ret = QMessageBox::question(this, tr("Remove Special Callsign"),
                                          tr("Remove '%1' from the special callsigns list?").arg(call));
    if (ret != QMessageBox::Yes)
        return;

    if (dataProxy->removeSpecialCallsign(call))
    {
        world->readWorld();
        refreshSpecialCallsignsTable();
    }
    else
    {
        QMessageBox::warning(this, tr("Error"),
                             tr("Could not remove the special callsign '%1'.").arg(call));
    }
}



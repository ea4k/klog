/***************************************************************************
                          mainwindowinputothers.cpp  -  description
                             -------------------
    begin                : Ago 2016
    copyright            : (C) 2016 by Jaime Robles
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


#include "mainwindowinputothers.h"
#include "../adif.h"
#include "../callsign.h"

MainWindowInputOthers::MainWindowInputOthers(DataProxy_SQLite *dp, World *injectedWorld, QWidget *parent) :
    QWidget(parent)
{
       //qDebug() << Q_FUNC_INFO;
    logLevel = None;
    dataProxy = dp;
    //world = new World(dataProxy,Q_FUNC_INFO);
    world = injectedWorld;

    util = new Utilities(Q_FUNC_INFO);
    //util->setLongPrefixes(dataProxy->getLongPrefixes());
    //util->setSpecialCalls(dataProxy->getSpecialCallsigns());

    //QLabel *entityPrimLabel, *entitySecLabel, *iotaAwardLabel, *entityNameLabel, *propModeLabel;
    iotaContinentComboBox           = new QComboBox();
    entityPrimDivComboBox           = new QComboBox();
    entitySecDivComboBox            = new QComboBox();
    entityNameComboBox              = new QComboBox();
    propModeComboBox                = new QComboBox();
    iotaNumberLineEdit              = new QLineEdit();
    keepPropCheckBox                = new QCheckBox();
    showAllCheckBox                 = new QCheckBox();
    userDefinedADIFComboBox         = new QComboBox();
    userDefinedADIFValueLineEdit    = new QLineEdit();

    // TODO: I should find the way to connect the SAT tabwidget's signal to set the propmode in this widget
    //       Now it is done though the mainwindow but I should avoid depending on that class for that, if possible
    //connect(satTabWidget, SIGNAL(setPropModeSat(QString)), this, SLOT(slotSetPropMode(QString)) ) ;
    connect(entityNameComboBox,             SIGNAL(currentIndexChanged(int)),   this, SLOT(slotEntityNameComboBoxChanged() ) ) ;
    connect(propModeComboBox,               SIGNAL(currentIndexChanged(int)),   this, SLOT(slotPropModeComboBoxChanged() ) ) ;
    connect(userDefinedADIFComboBox,        SIGNAL(currentIndexChanged(int)),   this, SLOT(slotUserDefinedADIFComboBoxChanged() ) ) ;
    connect(userDefinedADIFValueLineEdit,   SIGNAL(textChanged(QString)),       this, SLOT(slotSetCurrentUserData() ) );
    connect(entityPrimDivComboBox,          SIGNAL(currentIndexChanged(int)),   this, SLOT(slotPrimarySubdivisionsComboBoxChanged()) )  ;
    connect(showAllCheckBox,                SIGNAL(stateChanged(int)),          this, SLOT(slotShowAllCheckBoxChanged() ) ) ;

    createUI();
    setInitialADIFValues ();
       //qDebug() << Q_FUNC_INFO << " - END";
}

MainWindowInputOthers::~MainWindowInputOthers()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    delete(util);
}

QSO MainWindowInputOthers::getQSOData(QSO _qso)
{
    QSO qso = _qso;

    qso.setDXCC(getEntity() % 1000);
   //qDebug() << Q_FUNC_INFO << " - DXCC: " << qso.getDXCC();
    qso.setState(getState());
    //qso.setCounty()           //TODO: Related to entitySecDivComboBox when implemented
    qso.setIOTA(getIOTA());
    qso.setPropMode(getPropModeFromComboBox());
    qso.setSOTA_REF(getSOTA());
    qso.setAge(getAge());
    qso.setDistance(getDistance());
    qso.setSIG(getSIG());
    //qDebug() << Q_FUNC_INFO << " - AGE stored" << qso.getAge();
    qso.setSIG_INFO(getSIG_INFO());
    qso.setVUCCGrids(getVUCCGrids());
    qso.setWWFF_Ref(getWWFF_Ref());
    qso.setPOTA_Ref(getPOTA_REF());
    return qso;
}

void MainWindowInputOthers::setQSOData(const QSO &_qso)
{
    QSO qso(_qso);

    int qrzEntity = world->getQRZARRLId(qso.getCall());
    if (world->selectEntity(qrzEntity, qso.getDXCC()>0))
        setEntity(qrzEntity);
    else
    setEntity(qso.getDXCC());
    setPropMode(qso.getPropMode(), false);
    setSOTA(qso.getSOTA_REF());
    setAge(qso.getAge());
    setDistance(qso.getDistance());
    setVUCCGrids(qso.getVUCCGrids());
    setPOTA_REF(qso.getPOTA_Ref());
    setSIG(qso.getSIG());
    setSIG_INFO(qso.getSIG_INFO());
    setWWFF_Ref(qso.getWWFF_Ref());
    setIOTA(qso.getIOTA());
    setState(qso.getState());
}

void MainWindowInputOthers::createUI()
{
   //qDebug() << Q_FUNC_INFO ;
    logEvent (Q_FUNC_INFO, "Start", Debug);

    //entitiesList.clear();
    propModeList.clear();
    adifValidTypes.clear();
    autoUpdating = false;

    sota_ref    = QString();
    distance    = 0;
    age         = 0;
    currentInt  = 0;
    currentPref = QString();
    pota_ref    = QString();
    sig         = QString();
    sig_info    = QString();
    wwff_ref    = QString();
    darkMode    = false;

    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);
    palWhite.setColor(QPalette::Text, Qt::white);

    QLabel *entityPrimLabel = new QLabel(tr("Primary Div"));
    QLabel *entitySecLabel  = new QLabel(tr("Secondary Div"));
    QLabel *iotaAwardLabel  = new QLabel(tr("IOTA"));
    QLabel *entityNameLabel = new QLabel(tr("Entity"));
    QLabel *propModeLabel   = new QLabel(tr("Propagation mode"));
    QLabel *userSelectLabel = new QLabel(tr("Others"));

    entityPrimLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    entitySecLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    iotaAwardLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    entityNameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    propModeLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    userSelectLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    keepPropCheckBox->setText(tr("Keep propagation mode"));
    showAllCheckBox->setText(tr("Show All"));

    entityPrimDivComboBox->setToolTip(tr("Select the primary division for this QSO."));
    entitySecDivComboBox->setToolTip(tr("Select the secondary division for this QSO."));
    entityNameComboBox->setToolTip(tr("Select the entity for this QSO."));
    propModeComboBox->setToolTip(tr("Select the propagation mode for this QSO."));
    iotaContinentComboBox->setToolTip(tr("Select the IOTA continent for this QSO."));
    iotaNumberLineEdit->setToolTip(tr("Select the IOTA reference number for this QSO."));
    keepPropCheckBox->setToolTip(tr("Keeps the same propagation mode for next QSO."));
    showAllCheckBox->setToolTip(tr("Show all subdivisions for the current DXCC Entity."));

    userDefinedADIFComboBox->setToolTip(tr("Select the appropriate ADIF field for this QSO."));
    userDefinedADIFValueLineEdit->setToolTip (tr("Value for the selected ADIF field."));

    entityPrimDivComboBox->setEnabled(true);
    entitySecDivComboBox->setEnabled(false);
    entityNameComboBox->setEnabled(true);
    propModeComboBox->setEnabled(true);
    keepPropCheckBox->setEnabled(true);
    showAllCheckBox->setEnabled(true);

    QHBoxLayout *keepLayout = new QHBoxLayout;
    keepLayout->addWidget(propModeComboBox);
    //keepLayout->addWidget(keepPropCheckBox);
    keepLayout->setSizeConstraint(QLayout::SetFixedSize);


    QGridLayout *tabLayout = new QGridLayout;
    tabLayout->addWidget(entityNameLabel, 0, 0);
    tabLayout->addWidget(entityNameComboBox, 0, 1, 1, 2);
    tabLayout->addWidget(entityPrimLabel, 1, 0);
    tabLayout->addWidget(entityPrimDivComboBox, 1, 1, 1, 2);
    tabLayout->addWidget(showAllCheckBox, 1, 3);
    tabLayout->addWidget(entitySecLabel, 2, 0);
    tabLayout->addWidget(entitySecDivComboBox, 2, 1, 1, 2);
    tabLayout->addWidget(iotaAwardLabel, 3, 0);
    tabLayout->addWidget(iotaContinentComboBox, 3, 1);
    tabLayout->addWidget(iotaNumberLineEdit, 3, 2);
    tabLayout->addWidget(propModeLabel, 4, 0);
    tabLayout->addWidget(propModeComboBox, 4, 1, 1, 2);
    tabLayout->addLayout(keepLayout, 4, 1, 1, 2);

    tabLayout->addWidget(userSelectLabel, 5, 0);
    tabLayout->addWidget(userDefinedADIFComboBox, 5, 1);
    tabLayout->addWidget(userDefinedADIFValueLineEdit, 5, 2);
    tabLayout->addWidget(keepPropCheckBox, 6, 2);
    //tabLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(tabLayout);

   //qDebug() << Q_FUNC_INFO << ": Trying to add PropModes";
    propModeList = dataProxy->getPropModeList();
    if (propModeList.size()>1)
    {
       //qDebug() << Q_FUNC_INFO << ": Adding PropModes";
        propModeList.prepend("00 - " + tr("Not - Not Identified"));
        propModeComboBox->addItems(propModeList);
    }


    iotaContinentComboBox->addItems(dataProxy->getContinentShortNames());
    iotaNumberLineEdit->setInputMask("000");
    iotaNumberLineEdit->setText("000");
    readDarkMode();
    logEvent (Q_FUNC_INFO, "END", Debug);

    //qDebug() << Q_FUNC_INFO << ": (" << QString::number(this->size ().width ()) << "/" << QString::number(this->size ().height ()) << ")" ;
}

void MainWindowInputOthers::clear(bool _full)
{
    //qDebug() << Q_FUNC_INFO << ": Full = " << util->boolToQString(_full);
    logEvent (Q_FUNC_INFO, "Start", Debug);
    entityNameComboBox->setCurrentIndex(0);
    userDefinedADIFComboBox->setCurrentIndex (0);
    entityPrimDivComboBox->clear();
    entityPrimDivComboBox->addItem("00-" + tr("None Identified") + " (000)");

    userDefinedADIFValueLineEdit->clear ();

    sota_ref    = QString();
    currentPref = QString();
    currentInt  = 0;
    distance    = 0.0;
    age         = 0;
    vucc_grids  = QString();
    pota_ref    = QString();
    sig         = QString();
    sig_info    = QString();
    wwff_ref    = QString();

    clearIOTA();
    //iotaContinentComboBox->setCurrentIndex(0);
    //iotaNumberLineEdit->setText("000");
    if ((!keepPropCheckBox->isChecked()) || _full)
    {
      //qDebug() << Q_FUNC_INFO << ": Clear the proModeComboBox";
      propModeComboBox->setCurrentIndex(0);
    }
    if (_full)
    {
        //qDebug() << Q_FUNC_INFO << ": Clear the keepPropCheckBox";
        keepPropCheckBox->setChecked (false);
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}


void MainWindowInputOthers::setEntitiesList()
{
      //qDebug() << Q_FUNC_INFO << " - Start";
    logEvent (Q_FUNC_INFO, "Start", Debug);
      QStringList entitiesList;
      entitiesList.append( dataProxy->getEntitiesNames(false));
      if (entitiesList.size()>1)
      {
          entitiesList.prepend("00-" + tr("Not Identified") + " (000)");
          entityNameComboBox->addItems(entitiesList);
      }

    logEvent (Q_FUNC_INFO, "END", Debug);
}



int MainWindowInputOthers::getEntity()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    QString aux = entityNameComboBox->currentText();
    if (!aux.contains ('('))
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return -1;
    }
    //qDebug() << Q_FUNC_INFO << ": " << (aux.split('(').at(1).chopped(1));
    //return (aux.split('(').at(1).chopped(1)).toInt ();
    QString a = aux.split('(').at(1);
    a.chop(1);
    //qDebug() << Q_FUNC_INFO << ": " << a;
    logEvent (Q_FUNC_INFO, "END", Debug);
    return a.toInt ();
}

QString MainWindowInputOthers::getEntityPrefix()
{
    //qDebug() << Q_FUNC_INFO << ": " << (entityNameComboBox->currentText()).split('-').at(0);
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    QString prefix = (entityNameComboBox->currentText()).split('-').at(0);
    if (prefix.startsWith('*'))
         prefix.remove(QChar('*'), Qt::CaseInsensitive);
    return prefix;
    //return world->getQRZARRLId(pref);
}

void MainWindowInputOthers::setPropMode(const QString &_qs, bool _keep)
{
   //qDebug() << Q_FUNC_INFO << ": " << _qs << "/ Keep: " << util->boolToQString(_keep);
    logEvent (Q_FUNC_INFO, "Start", Debug);
    autoUpdating = true;
    if(( propModeComboBox->findText(_qs+" -", Qt::MatchContains))>0)
    {
       //qDebug() << Q_FUNC_INFO << " PropMode found" ;
        propModeComboBox->setCurrentIndex( propModeComboBox->findText(_qs+" -", Qt::MatchContains));
        keepPropCheckBox->setChecked(_keep);
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << " PropMode NOT found" ;
        propModeComboBox->setCurrentIndex(0);
        keepPropCheckBox->setChecked(false);
    }
    autoUpdating = false;
    logEvent (Q_FUNC_INFO, "END", Debug);
}

QString MainWindowInputOthers::getPropModeFromComboBox()
{
    //logEvent (Q_FUNC_INFO, "Start", Debug);

   //qDebug() << Q_FUNC_INFO << ": " << propModeComboBox->currentText();
    QString _pm = (((propModeComboBox->currentText()).split('-')).at(1)).simplified();
   //qDebug() << Q_FUNC_INFO << ": - 10" ;
    QString _n = (((propModeComboBox->currentText()).split('-')).at(0)).simplified();
   //qDebug() << Q_FUNC_INFO << ": - 11: " << _n ;

    if (_n == "00")
    {
       //qDebug() << Q_FUNC_INFO << ": - 12" ;
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return QString();
    }
   //qDebug() << Q_FUNC_INFO << ": - 14" ;
    logEvent (Q_FUNC_INFO, "END", Debug);
   //qDebug() << Q_FUNC_INFO << ": " << _pm ;
    return _pm;
}

bool MainWindowInputOthers::isSATPropagation()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (getPropModeFromComboBox() == "SAT")
    {
        logEvent (Q_FUNC_INFO, "END-True", Debug);
        return true;
    }
    else
    {
        logEvent (Q_FUNC_INFO, "END-False", Debug);
        return false;
    }
}

void MainWindowInputOthers::setPaletteIOTA(const bool _ok)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    if (_ok)
    {
        if (darkMode)
        {
            iotaNumberLineEdit->setPalette (palWhite);
        }
        else
        {
            iotaNumberLineEdit->setPalette (palBlack);
        }
    }
    else
    {
        iotaNumberLineEdit->setPalette (palRed);
    }
}


void MainWindowInputOthers::clearIOTA()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    iotaContinentComboBox->setCurrentIndex(0);
    iotaNumberLineEdit->setText("000");
    logEvent (Q_FUNC_INFO, "END", Debug);
    setPaletteIOTA(true);    //To avoid that 000 is considered wrong
}

void MainWindowInputOthers::setDarkMode (const bool _dm)
{
    darkMode = _dm;

    if (darkMode)
    {
       //qDebug() << Q_FUNC_INFO << " - True";
        iotaNumberLineEdit->setPalette(palWhite);
        userDefinedADIFValueLineEdit->setPalette(palWhite);
        iotaNumberLineEdit->setPalette(palWhite);
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << " - False";
        iotaNumberLineEdit->setPalette(palBlack);
        userDefinedADIFValueLineEdit->setPalette(palBlack);
        iotaNumberLineEdit->setPalette(palBlack);
    }
}

bool MainWindowInputOthers::isIOTAModified()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    return ((iotaContinentComboBox->currentIndex()>0) || (iotaNumberLineEdit->text()).toInt()>0);
}

void MainWindowInputOthers::setIOTA(const QString &_qs)
{//TODO: Seems to be better to send the color info like in: (it is much more flexible as I can send any color!)
    //void MainWindowInputQSL::setQSLVia(const QString &_qs, QColor qColor)
      //qDebug() << "MainWindow::setIOTA: " << _qs;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if ( (checkIfValidIOTA(_qs)).length() !=6 )
    {
        if (iotaNumberLineEdit->text() !=  "000")
            iotaNumberLineEdit->setPalette(palRed);
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return;
    }
    else
    {
        QStringList values = _qs.split("-", QT_SKIP);
          //qDebug() << Q_FUNC_INFO << ": IOTA " << _qs;
        iotaContinentComboBox->setCurrentIndex( iotaContinentComboBox->findText(values.at(0) ) );
        iotaNumberLineEdit->setText(values.at(1));
        if (darkMode)
        {
            iotaNumberLineEdit->setPalette(palWhite);
        }
        else
        {
            iotaNumberLineEdit->setPalette(palBlack);
        }
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}

QString MainWindowInputOthers::getIOTA()
{
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    return (checkIfValidIOTA(iotaContinentComboBox->currentText() + "-" + iotaNumberLineEdit->text()));
}

void MainWindowInputOthers::setIOTAContinent(const QString &_qs)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if(( iotaContinentComboBox->findText(_qs, Qt::MatchContains))>0)
    {
          //qDebug() << Q_FUNC_INFO << ": setting to index: " << QString::number(iotaContinentComboBox->findText(_qs, Qt::MatchContains));
        iotaContinentComboBox->setCurrentIndex( iotaContinentComboBox->findText(_qs, Qt::MatchContains));
    }
    else
    {
           //qDebug() << Q_FUNC_INFO << ": setting to index: 00";
        iotaContinentComboBox->setCurrentIndex(0);
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}

QString MainWindowInputOthers::checkIfValidIOTA(const QString &_tiota)
{
/**********************************
  IOTA should be always with this format: CC-NNN
  being:
     - CC the shortname of the continent
     - NNN Number of the reference.
       NNN has ALWAYS to include THREE(3) characters.
       ADIF Specs says:
            CC is the continent designator {NA, SA, EU , AF, OC, AS, AN}
            XXX is the island designator, where 0 <= XXX ,<= 999 [use leading zeroes]

Returns a valid format IOTA if possible and "" in other cases.

************************************/
      //qDebug() << Q_FUNC_INFO << ": " << _tiota;
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    if (_tiota.count("-") != 1)
    {
        logEvent (Q_FUNC_INFO, "END - Wrong format", Debug);
        return "";
    }

    QStringList values = _tiota.split("-", QT_SKIP);
    QString continent = values.at(0);
    QString number = values.at(1);

    if (!dataProxy->isValidContinentShortName(continent)) {
        logEvent (Q_FUNC_INFO, "END - Wrong continent", Debug);
        return "";
    }

    bool isNumberValid;
    int numberInt = number.toInt(&isNumberValid);

    if (!isNumberValid || numberInt <= 0 || numberInt >= 1000) {
        logEvent (Q_FUNC_INFO, "END - Wrong number", Debug);
        return "";
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
    return QString("%1-%2").arg(continent).arg(numberInt, 3, 10, QChar('0'));
}

void MainWindowInputOthers::slotPropModeComboBoxChanged()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (autoUpdating)
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return;
    }
    emit setPropMode(getPropModeFromComboBox());
    logEvent (Q_FUNC_INFO, "END", Debug);
}


void MainWindowInputOthers::setKeep(const bool _b)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << ": " << util->boolToQString(_b);
    keepPropCheckBox->setChecked (_b);
    logEvent (Q_FUNC_INFO, "END", Debug);
}

bool MainWindowInputOthers::getKeep()
{
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    //qDebug() << Q_FUNC_INFO;
    return keepPropCheckBox->isChecked ();
}

bool MainWindowInputOthers::setPOTA_REF(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    Adif adif(Q_FUNC_INFO);
    if (!adif.isValidPOTA(_op))
        return false;
    pota_ref = _op;
    slotUserDefinedADIFComboBoxChanged();
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

QString MainWindowInputOthers::getPOTA_REF()
{
    //qDebug() << Q_FUNC_INFO;
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    return pota_ref;
}

bool MainWindowInputOthers::setSIG(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    Adif adif(Q_FUNC_INFO);
    if (_op.length()<=0)
        return false;
    sig = _op;
    slotUserDefinedADIFComboBoxChanged();
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

QString MainWindowInputOthers::getSIG()
{
    //qDebug() << Q_FUNC_INFO;
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    return sig;
}

bool MainWindowInputOthers::setSIG_INFO(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    Adif adif(Q_FUNC_INFO);
    if (_op.length()<=0)
        return false;
    sig_info = _op;
    slotUserDefinedADIFComboBoxChanged();
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

QString MainWindowInputOthers::getSIG_INFO()
{
    //qDebug() << Q_FUNC_INFO;
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    return sig_info;
}


bool MainWindowInputOthers::setWWFF_Ref(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    Adif adif(Q_FUNC_INFO);
    if (!adif.isValidWWFF_Ref(_op))
        return false;
    wwff_ref = _op;
    //qDebug() << Q_FUNC_INFO << ": " << wwff_ref;
    slotUserDefinedADIFComboBoxChanged();
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

QString MainWindowInputOthers::getWWFF_Ref()
{
    qDebug() << Q_FUNC_INFO << ": " << wwff_ref;
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    return wwff_ref;
}

bool MainWindowInputOthers::setUserADIFTypeComboBox(const QString &_value)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (_value == "SOTA_REF")
    {
        userDefinedADIFComboBox->setCurrentIndex (0);
    }
    else if (_value == "AGE")
    {
        userDefinedADIFComboBox->setCurrentIndex (1);
    }
    else if (_value == "DISTANCE")
    {
        userDefinedADIFComboBox->setCurrentIndex (2);
    }
    else if (_value == "POTA_REF")
    {
        userDefinedADIFComboBox->setCurrentIndex (3);
    }
    else if (_value == "SIG")
    {
        userDefinedADIFComboBox->setCurrentIndex (4);
    }
    else if (_value == "SIG_INFO")
    {
        userDefinedADIFComboBox->setCurrentIndex (5);
    }
    else if (_value == "VUCC_GRIDS")
    {
        userDefinedADIFComboBox->setCurrentIndex (6);
    }
    else if (_value == "WWFF_REF")
    {
        userDefinedADIFComboBox->setCurrentIndex (7);
    }
    else
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return false;
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

QString MainWindowInputOthers::getUserADIFTypeComboBox()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    int value = (((userDefinedADIFComboBox->currentText ()).split('-')).at(0)).toInt ();
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(value);

    switch (value)
    {
    case 1:
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return "SOTA_REF";
    case 2:
        logEvent (Q_FUNC_INFO, "END-2", Debug);
        return "AGE";
    case 3:
        logEvent (Q_FUNC_INFO, "END-3", Debug);
        return "DISTANCE";
    case 4:
        logEvent (Q_FUNC_INFO, "END-4", Debug);
        return "POTA_REF";
    case 5:
        logEvent (Q_FUNC_INFO, "END-5", Debug);
        return "SIG";
    case 6:
        logEvent (Q_FUNC_INFO, "END-6", Debug);
        return "SIG_INFO";
    case 7:
        logEvent (Q_FUNC_INFO, "END-7", Debug);
        return "VUCC_GRIDS";
    case 8:
        logEvent (Q_FUNC_INFO, "END-8", Debug);
        return "WWFF_REF";
    default:
        logEvent (Q_FUNC_INFO, "END", Debug);
        return QString();
    }
}

bool MainWindowInputOthers::setUserADIFValue(const QString &_adifValue)
{
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    userDefinedADIFValueLineEdit->setText (_adifValue);
    return true;
}

QString MainWindowInputOthers::getUserADIFValue()
{
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    return userDefinedADIFValueLineEdit->text();
}

bool MainWindowInputOthers::setInitialADIFValues()
{
    logEvent (Q_FUNC_INFO, "END-", Debug);
    setEntitiesList();
    adifValidTypes  << "01-"    + tr("SOTA Ref")    << "02-" + tr ("Age")
                   << "03-"     + tr("Distance")    << "04-" + tr("POTA Ref")
                   << "05-"     + tr("SIG")         << "06-" + tr("SIG Info")
                   << "07-"     + tr ("VUCC grids") << "08-" + tr("WWFF Ref");

    userDefinedADIFComboBox->clear ();
    userDefinedADIFComboBox->addItems (adifValidTypes);
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

bool MainWindowInputOthers::setState(const QString &_op)
{ // Sets a subdivision/State
   //qDebug() << Q_FUNC_INFO << ": " << _op;
    if (_op.length()<1)
        return false;

    int indexC = entityPrimDivComboBox->findText(QString("%1-").arg(_op), Qt::MatchStartsWith);

   //qDebug() << Q_FUNC_INFO << _op << "/" << QString::number(indexC) << " / (6): "  << entityPrimDivComboBox->itemText(6);
    if (indexC<0)
        return false;

    entityPrimDivComboBox->setCurrentIndex(indexC);
    return true;
}

QString MainWindowInputOthers::getState()
{// Reads the subdivision. It return just the shortname/code for the subdivision/State
   //qDebug() << Q_FUNC_INFO << ": " << ((entityPrimDivComboBox->currentText()).split("-")).at(0);

    QString aux = entityPrimDivComboBox->currentText();
    if ((aux.startsWith("00-")) && (aux.endsWith("(000)")) )
        return QString();

    return ((entityPrimDivComboBox->currentText()).split("-")).at(0);
}

void MainWindowInputOthers::updatePrimarySubdivisionsComboBox(QList<PrimarySubdivision> _subdivisions)
{
    //qDebug() << Q_FUNC_INFO << " - count: " << QString::number(_subdivisions.count());
    entityPrimDivComboBox->clear();
    if (_subdivisions.isEmpty())
        return;

    QStringList listOfSubdivisions;
    listOfSubdivisions.clear();
    QStringList names;
    names.clear();


    foreach (PrimarySubdivision ps, _subdivisions)
    {

        QString subdivision = ps.shortName + "-" + ps.name;
        if (!listOfSubdivisions.contains(subdivision))
            listOfSubdivisions.append(subdivision);
        //qDebug() << Q_FUNC_INFO << ": Subdivision added: " << subdivision;
    }
    if (listOfSubdivisions.isEmpty())
        return;

    listOfSubdivisions.sort();
    listOfSubdivisions.removeDuplicates();
    listOfSubdivisions.prepend("00-" + tr("Not selected") + " (000)");

    entityPrimDivComboBox->addItems(listOfSubdivisions);
    if (listOfSubdivisions.length() == 2)
        entityPrimDivComboBox->setCurrentIndex(1);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindowInputOthers::setEntity(const int _entity)
{// Select the appropriate entity in the ComboBox
  //qDebug() << Q_FUNC_INFO << ": " << QString::number(_entity);
    logEvent (Q_FUNC_INFO, "Start", Debug);
  //qDebug() << Q_FUNC_INFO << " - 10" ;

    if (_entity<=0)
    {
      //qDebug() << Q_FUNC_INFO << " - 11" ;
        entityNameComboBox->setCurrentIndex(0);
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return;
    }
  //qDebug() << Q_FUNC_INFO << " - 20" ;
    currentInt = _entity;
    QString num = QString::number(_entity);
    if (_entity > 1000)
    {
      //qDebug() << Q_FUNC_INFO << " - 30" ;
       // num = (QString::number(_entity)).right(3);
    }
  //qDebug() << Q_FUNC_INFO << " - 40" ;

    int indexC = entityNameComboBox->findText("(" + num + ")", Qt::MatchEndsWith);
    if (indexC != -1)
    {
        entityNameComboBox->setCurrentIndex(indexC);
    }
    else
    {
      //qDebug() << Q_FUNC_INFO << " - Unable to find entity in the ComboBox";
    }

    setIOTAContinent(dataProxy->getContinentShortNameFromEntity(_entity)) ;
    //qDebug() << Q_FUNC_INFO << " - 100" ;
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainWindowInputOthers::setEntityAndPrefix(const int _entity, const QString &_qrz)
{
    //qDebug() << Q_FUNC_INFO << " - Start: " << QString::number(_entity) << "/" << _qrz;
    if (_entity<=0)
    {
      //qDebug() << Q_FUNC_INFO << " -  10";
        entityNameComboBox->setCurrentIndex(0);
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return;
    }

  //qDebug() << Q_FUNC_INFO << " - 15";
    setEntity(_entity);

    Callsign callsign(_qrz);
    if (callsign.isValid() || callsign.isValidPrefix())
        currentPref = callsign.getHostFullPrefix();

    QString prefixFromEntityNumber  = world->getEntityMainPrefix(_entity);        // The main prefix of the entity.
    QString hostFullPrefix          = callsign.getHostFullPrefix();               // The default is that showAll is not checked. Main prefix+ the area
    QString hostPrefix              = callsign.getHostPrefix();                   // The default is that showAll is not checked


    Callsign entityCall(prefixFromEntityNumber);    // To check if the prefixFromEntity is the main or not (EA->main, AM-> nor main, it should be EA)
    QString prefixForSubdivision = prefixFromEntityNumber;

   //qDebug() << Q_FUNC_INFO << " - Before: hostPrefix:              " << hostPrefix;
   //qDebug() << Q_FUNC_INFO << " - Before: prefixFromEntityNumber:  " << prefixFromEntityNumber;
     // Construct hostFullPrefix if hostPrefix is not the main prefix.
    if (hostPrefix != prefixFromEntityNumber)
    {
       //qDebug() << Q_FUNC_INFO << " - Building: " << prefixFromEntityNumber;
        hostFullPrefix = prefixFromEntityNumber + QString::number(callsign.getHostAreaNumber());
    }

  //qDebug() << Q_FUNC_INFO << " -  20";
  //qDebug() << Q_FUNC_INFO << "hostFullPrefix:             " << hostFullPrefix;
  //qDebug() << Q_FUNC_INFO << "hostPrefix:                 " << hostPrefix;
  //qDebug() << Q_FUNC_INFO << "prefixFromEntityNumber:     " << prefixFromEntityNumber;

// ea4k parece que falla para IT9

    if ((hostFullPrefix.isEmpty()) && (hostPrefix.isEmpty()))
        return;

    //qDebug() << Q_FUNC_INFO << " -  40";
    QList<PrimarySubdivision> primarySubdivisions;
    primarySubdivisions.clear();

    if (showAllCheckBox->isChecked())
    {
        primarySubdivisions.append(dataProxy->getPrimarySubDivisions(currentInt, QString()));
    }
    else
    {
        primarySubdivisions.append(dataProxy->getPrimarySubDivisions(currentInt, hostFullPrefix));
        currentPref = hostFullPrefix;
        if (primarySubdivisions.isEmpty())
        {
            //qDebug() << Q_FUNC_INFO << " -  50";
            //qDebug() << Q_FUNC_INFO << " - primarySubdivisions is empty with hostPrefix, running for the main prefix";
            primarySubdivisions.append(dataProxy->getPrimarySubDivisions(currentInt, hostPrefix));
            currentPref = hostPrefix;
            if (primarySubdivisions.isEmpty())
            {
                //qDebug() << Q_FUNC_INFO << " -  55";
                //qDebug() << Q_FUNC_INFO << " - primarySubdivisions is empty with mainprefix, running just with the entity";
                primarySubdivisions.append(dataProxy->getPrimarySubDivisions(currentInt, QString()));
                currentPref = QString();
            }
            //qDebug() << Q_FUNC_INFO << " -  59";
        }
    }


    //qDebug() << Q_FUNC_INFO << " - 60";
    //qDebug() << Q_FUNC_INFO << " - count: " << QString::number(primarySubdivisions.count());
    if (primarySubdivisions.isEmpty())
    {
        entityPrimDivComboBox->clear();
        return;
    }

    //qDebug() << Q_FUNC_INFO << " - 70 ";
    updatePrimarySubdivisionsComboBox(primarySubdivisions);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindowInputOthers::slotUserDefinedADIFComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO << ": " << getUserADIFTypeComboBox ();
    logEvent (Q_FUNC_INFO, "Start", Debug);
    QString currentTag = getUserADIFTypeComboBox ();

    setColorsForUserDefinedADIFValueLineEdit();

    if (currentTag == "SOTA_REF")
    {
        userDefinedADIFValueLineEdit->setText (sota_ref);
    }
    else if (currentTag == "AGE")
    {
        userDefinedADIFValueLineEdit->setText (QString::number(age));
    }
    else if (currentTag == "VUCC_GRIDS")
    {
        userDefinedADIFValueLineEdit->setText (vucc_grids);
    }
    else if (currentTag == "DISTANCE")
    {
        userDefinedADIFValueLineEdit->setText (QString::number(distance));
    }
    else if (currentTag == "POTA_REF")
    {
        userDefinedADIFValueLineEdit->setText (pota_ref);
    }
    else if (currentTag == "SIG")
    {
        userDefinedADIFValueLineEdit->setText (sig);
    }
    else if (currentTag == "SIG_INFO")
    {
        userDefinedADIFValueLineEdit->setText (sig_info);
    }
    else if (currentTag == "WWFF_REF")
    {
        userDefinedADIFValueLineEdit->setText (wwff_ref);
    }
}

bool MainWindowInputOthers::setVUCCGrids(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (checkVUCC_GRIDS(_op))
    {
        vucc_grids = _op;
        slotUserDefinedADIFComboBoxChanged();
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return true;
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
    return false;
}

bool MainWindowInputOthers::checkVUCC_GRIDS(const QString &_string)
{
    //qDebug() << Q_FUNC_INFO << ": " << _string;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (util->isValidVUCCGrids (_string))
    {
        setColorsForUserDefinedADIFValueLineEdit();
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return true;
    }
    else
    {
        userDefinedADIFValueLineEdit->setPalette (palRed);
        logEvent (Q_FUNC_INFO, "END-2", Debug);
        return false;
    }
}

QString MainWindowInputOthers::getVUCCGrids()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (checkVUCC_GRIDS (vucc_grids))
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return vucc_grids;
    }
    else
    {
        logEvent (Q_FUNC_INFO, "END-2", Debug);
        return QString();
    }
}

void MainWindowInputOthers::setColorsForUserDefinedADIFValueLineEdit()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (darkMode)
    {
        userDefinedADIFValueLineEdit->setPalette(palWhite);
    }
    else
    {
        userDefinedADIFValueLineEdit->setPalette(palBlack);
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}

bool MainWindowInputOthers::setSOTA(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    sota_ref = _op;
    slotUserDefinedADIFComboBoxChanged();
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

QString MainWindowInputOthers::getSOTA()
{
    //qDebug() << Q_FUNC_INFO;
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    return sota_ref;
}

bool MainWindowInputOthers::setAge(const double _op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    age = _op;
    slotUserDefinedADIFComboBoxChanged();
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

double MainWindowInputOthers::getAge()
{
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    //qDebug() << Q_FUNC_INFO << ": " << age;
    return age;
}

bool MainWindowInputOthers::setDistance(const double _op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    distance = _op;
    slotUserDefinedADIFComboBoxChanged();
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

double MainWindowInputOthers::getDistance()
{
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    return distance;
}

void MainWindowInputOthers::slotSetCurrentUserData()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    QString currentTag = getUserADIFTypeComboBox ();

    if (currentTag == "SOTA_REF")
    {
       sota_ref = userDefinedADIFValueLineEdit->text();
    }
    else if (currentTag == "AGE")
    {
        age = userDefinedADIFValueLineEdit->text().toDouble();
    }
    else if (currentTag == "VUCC_GRIDS")
    {
        vucc_grids = userDefinedADIFValueLineEdit->text().toUpper();
        userDefinedADIFValueLineEdit->setText (vucc_grids);
    }
    else if (currentTag == "DISTANCE")
    {
        distance = userDefinedADIFValueLineEdit->text().toDouble();
    }
    if (currentTag == "POTA_REF")
    {    
        pota_ref = userDefinedADIFValueLineEdit->text();
    }
    if (currentTag == "SIG")
    {
        sig = userDefinedADIFValueLineEdit->text();
        //setSIG(userDefinedADIFValueLineEdit->text());
        //sig = userDefinedADIFValueLineEdit->text();
    }
    if (currentTag == "SIG_INFO")
    {
        sig_info = userDefinedADIFValueLineEdit->text();
    }
    if (currentTag == "WWFF_REF")
    {
        //wwff_ref = userDefinedADIFValueLineEdit->text();
        wwff_ref = userDefinedADIFValueLineEdit->text();
        //setWWFF_Ref(userDefinedADIFValueLineEdit->text());
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}


void MainWindowInputOthers::slotPrimarySubdivisionsComboBoxChanged()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
   //qDebug() << Q_FUNC_INFO << entityPrimDivComboBox->currentText();

    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainWindowInputOthers::slotShowAllCheckBoxChanged()
{
    setEntityAndPrefix(currentInt, currentPref);
}

void MainWindowInputOthers::slotEntityNameComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO << entityNameComboBox->currentText();

    QString prefix = getEntityPrefix();
    //qDebug() << Q_FUNC_INFO << " - " << prefix;
    int entity = world->getQRZARRLId(prefix);
    setEntityAndPrefix(entity, prefix);
    //entityPrimDivComboBox->clear();
    entityPrimDivComboBox->addItem("00-" + tr("None Identified") + " (000)");
}

void MainWindowInputOthers::readDarkMode()
{
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("Colors");
    setDarkMode(settings.value("DarkMode", false).toBool ());
    settings.endGroup ();
}


void MainWindowInputOthers::setLogLevel (const DebugLogLevel _l)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    logLevel = _l;
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainWindowInputOthers::logEvent(const QString &_func, const QString &_msg,  DebugLogLevel _level)
{
    if (logLevel<=_level)
        emit debugLog (_func, _msg, _level);
}

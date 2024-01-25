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

MainWindowInputOthers::MainWindowInputOthers(DataProxy_SQLite *dp, QWidget *parent) :
    QWidget(parent)
{
       //qDebug() << Q_FUNC_INFO;
    logLevel = None;
    dataProxy = dp;

    util = new Utilities(Q_FUNC_INFO);

    //QLabel *entityPrimLabel, *entitySecLabel, *iotaAwardLabel, *entityNameLabel, *propModeLabel;
    iotaContinentComboBox = new QComboBox();
    entityPrimDivComboBox = new QComboBox();
    entitySecDivComboBox = new QComboBox();
    entityNameComboBox = new QComboBox();
    propModeComboBox = new QComboBox();
    iotaNumberLineEdit = new QLineEdit();
    keepPropCheckBox = new QCheckBox();
    userDefinedADIFComboBox = new QComboBox();
    userDefinedADIFValueLineEdit = new QLineEdit();

    // TODO: I should find the way to connect the SAT tabwidget's signal to set the propmode in this widget
    //       Now it is done though the mainwindow but I should avoid depending on that class for that, if possible
    //connect(satTabWidget, SIGNAL(setPropModeSat(QString)), this, SLOT(slotSetPropMode(QString)) ) ;
    connect(propModeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotPropModeComboBoxChanged() ) ) ;
    connect(userDefinedADIFComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotUSerDefinedADIFComboBoxChanged() ) ) ;
    connect(userDefinedADIFValueLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSetCurrentUSerData() ) );

    createUI();
    setInitialADIFValues ();
       //qDebug() << Q_FUNC_INFO << ": "- END";
}

MainWindowInputOthers::~MainWindowInputOthers()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    delete(util);
}

void MainWindowInputOthers::createUI()
{
      //qDebug() << Q_FUNC_INFO ;
    logEvent (Q_FUNC_INFO, "Start", Debug);

    entitiesList.clear();
    propModeList.clear();
    adifValidTypes.clear();
    autoUpdating = false;

    sota_ref = QString();
    distance = 0;
    age = 0;

    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);
    palWhite.setColor(QPalette::Text, Qt::white);

    QLabel *entityPrimLabel = new QLabel(tr("Primary Div"));
    QLabel *entitySecLabel = new QLabel(tr("Secondary Div"));
    QLabel *iotaAwardLabel = new QLabel(tr("IOTA"));
    QLabel *entityNameLabel = new QLabel(tr("Entity"));
    QLabel *propModeLabel = new QLabel(tr("Propagation mode"));
    QLabel *userSelectLabel = new QLabel(tr("Others"));

    entityPrimLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    entitySecLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    iotaAwardLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    entityNameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    propModeLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    userSelectLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    keepPropCheckBox->setText(tr("Keep propagation mode"));

    entityPrimDivComboBox->setToolTip(tr("Select the primary division for this QSO."));
    entitySecDivComboBox->setToolTip(tr("Select the secondary division for this QSO."));
    entityNameComboBox->setToolTip(tr("Select the entity for this QSO."));
    propModeComboBox->setToolTip(tr("Select the propagation mode for this QSO."));
    iotaContinentComboBox->setToolTip(tr("Select the IOTA continent for this QSO."));
    iotaNumberLineEdit->setToolTip(tr("Select the IOTA reference number for this QSO."));
    keepPropCheckBox->setToolTip(tr("Keeps the same propagation mode for next QSO."));

    userDefinedADIFComboBox->setToolTip(tr("Select the appropriate ADIF field for this QSO."));
    userDefinedADIFValueLineEdit->setToolTip (tr("Value for the selected ADIF field."));

    entityPrimDivComboBox->setEnabled(false);
    entitySecDivComboBox->setEnabled(false);
    entityNameComboBox->setEnabled(true);
    propModeComboBox->setEnabled(true);
    keepPropCheckBox->setEnabled(true);

    QHBoxLayout *keepLayout = new QHBoxLayout;
    keepLayout->addWidget(propModeComboBox);
    //keepLayout->addWidget(keepPropCheckBox);
    keepLayout->setSizeConstraint(QLayout::SetFixedSize);

    QGridLayout *tabLayout = new QGridLayout;
    tabLayout->addWidget(entityNameLabel, 0, 0);
    tabLayout->addWidget(entityNameComboBox, 0, 1, 1, 2);
    tabLayout->addWidget(entityPrimLabel, 1, 0);
    tabLayout->addWidget(entityPrimDivComboBox, 1, 1, 1, 2);
    tabLayout->addWidget(entitySecLabel, 2, 0);
    tabLayout->addWidget(entitySecDivComboBox, 2, 1, 1, 2);
    tabLayout->addWidget(iotaAwardLabel, 3, 0);
    tabLayout->addWidget(iotaContinentComboBox, 3, 1);
    tabLayout->addWidget(iotaNumberLineEdit, 3, 2);
    tabLayout->addWidget(propModeLabel, 4, 0);
    tabLayout->addLayout(keepLayout, 4, 1, 1, 2);
    tabLayout->addWidget(entityPrimLabel, 1, 0);
    tabLayout->addWidget(entityPrimDivComboBox, 1, 1, 1, 2);
    tabLayout->addWidget(userSelectLabel, 5, 0);
    tabLayout->addWidget(userDefinedADIFComboBox, 5, 1);
    tabLayout->addWidget(userDefinedADIFValueLineEdit, 5, 2);
    tabLayout->addWidget(keepPropCheckBox, 6, 2);
    //tabLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(tabLayout);

    if (entitiesList.size()>1)
    {
        entitiesList.prepend("00-" + tr("Not Identified") + " (000)");
        entityNameComboBox->addItems(entitiesList);
    }
    propModeList = dataProxy->getPropModeList();
    if (propModeList.size()>1)
    {
        propModeList.prepend("00 - " + tr("Not - Not Identified"));
        propModeComboBox->addItems(propModeList);
    }

    iotaContinentComboBox->addItems(dataProxy->getContinentShortNames());
    iotaNumberLineEdit->setInputMask("000");
    iotaNumberLineEdit->setText("000");
    logEvent (Q_FUNC_INFO, "END", Debug);

    //qDebug() << Q_FUNC_INFO << ": (" << QString::number(this->size ().width ()) << "/" << QString::number(this->size ().height ()) << ")" ;
}

void MainWindowInputOthers::clear(bool _full)
{
    qDebug() << Q_FUNC_INFO << ": Full = " << util->boolToQString(_full);
    logEvent (Q_FUNC_INFO, "Start", Debug);
    entityNameComboBox->setCurrentIndex(0);
    userDefinedADIFComboBox->setCurrentIndex (0);
    userDefinedADIFValueLineEdit->clear ();
    sota_ref = QString();
    distance = 0.0;
    age = 0;

    iotaContinentComboBox->setCurrentIndex(0);
    iotaNumberLineEdit->setText("000");
    if ((!keepPropCheckBox->isChecked()) || _full)
    {
      qDebug() << Q_FUNC_INFO << ": Clear the proModeComboBox";
      propModeComboBox->setCurrentIndex(0);
    }
    if (_full)
    {
        qDebug() << Q_FUNC_INFO << ": Clear the keepPropCheckBox";
        keepPropCheckBox->setChecked (false);
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainWindowInputOthers::setEntitiesList(const QStringList _qs)
{
      //qDebug() << Q_FUNC_INFO << ": " << QString::number(_qs.length());
    logEvent (Q_FUNC_INFO, "Start", Debug);
    entitiesList.clear();
    entitiesList << _qs;
    if (entitiesList.size()>1)
    {
        entitiesList.prepend("00-" + tr("Not Identified") + " (000)");
        //entitiesList.prepend("00-Not Identified (000)");
        entityNameComboBox->addItems(entitiesList);
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainWindowInputOthers::setEntity(const int _ent)
{// Select the appropriate entity in the ComboBox
       //qDebug() << Q_FUNC_INFO << ": " << QString::number(_ent);
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (_ent<=0)
    {
        entityNameComboBox->setCurrentIndex(0);
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return;
    }
    QString aux = QString();
    aux = QString::number(_ent);
    if (_ent > 1000)
    {
        aux = (QString::number(_ent)).right(3);
    }

    int indexC = entityNameComboBox->findText("(" + aux + ")", Qt::MatchEndsWith);

    //qDebug() << "MainWindow::selectCorrectEntity: " << pref << "/" << QString::number(indexC);
    entityNameComboBox->setCurrentIndex(indexC);
    setIOTAContinentFromEntity(_ent);
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
    return (entityNameComboBox->currentText()).split('-').at(0);
    //return world->getQRZARRLId(pref);
}

void MainWindowInputOthers::setPropMode(const QString &_qs, bool _keep)
{
    qDebug() << Q_FUNC_INFO << ": " << _qs << "/ Keep: " << util->boolToQString(_keep);
    logEvent (Q_FUNC_INFO, "Start", Debug);
    autoUpdating = true;
    if(( propModeComboBox->findText(_qs+" -", Qt::MatchContains))>0)
    {
        qDebug() << Q_FUNC_INFO << " PropMode found" ;
        propModeComboBox->setCurrentIndex( propModeComboBox->findText(_qs+" -", Qt::MatchContains));
        keepPropCheckBox->setChecked(_keep);
    }
    else
    {
        qDebug() << Q_FUNC_INFO << " PropMode NOT found" ;
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
        //qDebug() << Q_FUNC_INFO << ": - 13" ;
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


void MainWindowInputOthers::clearIOTA()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    iotaContinentComboBox->setCurrentIndex(0);
    iotaNumberLineEdit->setText("000");
    logEvent (Q_FUNC_INFO, "END", Debug);
    //iotaNumberLineEdit->setPalette(palBlack);
}

bool MainWindowInputOthers::isIOTAModified()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if ((iotaContinentComboBox->currentIndex()>0) || (iotaNumberLineEdit->text()).toInt()>0)
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

void MainWindowInputOthers::setIOTA(const QString &_qs)
{//TODO: Seems to be better to send the color info like in: (it is much more flexible as I can send any color!)

    //void MainWindowInputQSL::setQSLVia(const QString &_qs, QColor qColor)
      //qDebug() << "MainWindow::setIOTA: " << _qs;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if ( (checkIfValidIOTA(_qs)).length() !=6 )
    {
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
        if (getDarkMode())
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


void MainWindowInputOthers::setIOTAContinentFromEntity(const int _n)
{
      //qDebug() << "MainWindow::setIOTAContinentFromEntity:" << QString::number(_n);
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    setIOTAContinent(dataProxy->getContinentShortNameFromEntity(_n)) ;
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
    QString _continent;
    QString _number;

    if (_tiota.count("-") == 1)
    {
        QStringList _values = _tiota.split("-", QT_SKIP);
        _continent = _values.at(0);
        _number = _values.at(1);
    }
    else
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return "";
    }
      //qDebug() << Q_FUNC_INFO << ": (cont) " << _continent;
      //qDebug() << Q_FUNC_INFO << ": (numb): " << _number;

    // Check if continent is valid

    if (dataProxy->isValidContinentShortName(_continent))
    {
        if ( (_number.toInt() >0 ) && ((_number.toInt()) < 1000 ))
        {
            if ((_number.length()) == 3)
            {
                logEvent (Q_FUNC_INFO, "END-1", Debug);
                return _continent + "-" + _number ;
            }
            else if ((_number.length()) == 2)
            {
                logEvent (Q_FUNC_INFO, "END-2", Debug);
                return _continent + "-0" + QString::number((_number).toInt());
            }
            else if ((_number.length()) == 1)
            {
                logEvent (Q_FUNC_INFO, "END-3", Debug);
                return _continent + "-00" + QString::number((_number).toInt());
            }
            else
            {
                logEvent (Q_FUNC_INFO, "END-4", Debug);
                return "";
            }
        }
        else
        {
            logEvent (Q_FUNC_INFO, "END-5", Debug);
            return "";
        }
    }
    else
    {
        logEvent (Q_FUNC_INFO, "END", Debug);
        return QString();
    }
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
    qDebug() << Q_FUNC_INFO << ": " << util->boolToQString(_b);
    keepPropCheckBox->setChecked (_b);
    logEvent (Q_FUNC_INFO, "END", Debug);
}

bool MainWindowInputOthers::getKeep()
{
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    qDebug() << Q_FUNC_INFO;
    return keepPropCheckBox->isChecked ();
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
        return "VUCC_GRIDS";
    case 4:
        logEvent (Q_FUNC_INFO, "END-4", Debug);
        return "DISTANCE";
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
    adifValidTypes << "01-" + tr("SOTA Ref") << "02-" + tr ("Age") << "03-" + tr ("VUCC grids")
                   << "04-" + tr("Distance");
    userDefinedADIFComboBox->clear ();
    userDefinedADIFComboBox->addItems (adifValidTypes);
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

void MainWindowInputOthers::slotUSerDefinedADIFComboBoxChanged()
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
}

bool MainWindowInputOthers::setVUCCGrids(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (checkVUCC_GRIDS(_op))
    {
        vucc_grids = _op;
        slotUSerDefinedADIFComboBoxChanged();
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
    if (getDarkMode())
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
    slotUSerDefinedADIFComboBoxChanged();
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
    slotUSerDefinedADIFComboBoxChanged();
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

double MainWindowInputOthers::getAge()
{
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    return age;
}

bool MainWindowInputOthers::setDistance(const double _op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    distance = _op;
    slotUSerDefinedADIFComboBoxChanged();
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

double MainWindowInputOthers::getDistance()
{
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    return distance;
}

void MainWindowInputOthers::slotSetCurrentUSerData()
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
        //userDefinedADIFValueLineEdit->setText (QString::number(distance));
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}

bool MainWindowInputOthers::getDarkMode()
{
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    return ( iotaNumberLineEdit->palette().color (QPalette::Base) == "#646464");
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

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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/


#include "mainwindowinputothers.h"

MainWindowInputOthers::MainWindowInputOthers(DataProxy_SQLite *dp, QWidget *parent) :
    QWidget(parent)
{
       //qDebug() << "MainWindowInputOthers::MainWindowInputOthers" << QT_ENDL;
    entitiesList.clear();
    propModeList.clear();
    adifValidTypes.clear();
    autoUpdating = false;
    dataProxy = dp;
    propModeList = dataProxy->getPropModeList();
    sota_ref = QString();
    age = 0;
    util = new Utilities;
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
       //qDebug() << "MainWindowInputOthers::MainWindowInputOthers - END" << QT_ENDL;
}

MainWindowInputOthers::~MainWindowInputOthers(){}

void MainWindowInputOthers::createUI()
{
      //qDebug() << "MainWindowInputOthers::createUI" << QT_ENDL;

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

    if (propModeList.size()>1)
    {
        propModeList.prepend("00 - " + tr("Not - Not Identified"));
        propModeComboBox->addItems(propModeList);
    }

    iotaContinentComboBox->addItems(dataProxy->getContinentShortNames());
    iotaNumberLineEdit->setInputMask("000");
    iotaNumberLineEdit->setText("000");


    //qDebug() << Q_FUNC_INFO << ": (" << QString::number(this->size ().width ()) << "/" << QString::number(this->size ().height ()) << ")" ;
}

void MainWindowInputOthers::clear(bool _full)
{
      //qDebug() << "MainWindowInputOthers::clear" << QT_ENDL;
    entityNameComboBox->setCurrentIndex(0);
    userDefinedADIFComboBox->setCurrentIndex (0);
    userDefinedADIFValueLineEdit->clear ();
    sota_ref = QString();
    age = 0;

    iotaContinentComboBox->setCurrentIndex(0);
    iotaNumberLineEdit->setText("000");
    if ((!keepPropCheckBox->isChecked()) || _full)
    {
      propModeComboBox->setCurrentIndex(0);
    }
    if (_full)
    {
        keepPropCheckBox->setChecked (false);
    }
}

void MainWindowInputOthers::setEntitiesList(const QStringList _qs)
{
      //qDebug() << "MainWindowInputOthers::setEntitiesList: " << QString::number(_qs.length()) << QT_ENDL;
    entitiesList.clear();
    entitiesList << _qs;
    if (entitiesList.size()>1)
    {
        entitiesList.prepend("00-" + tr("Not Identified") + " (000)");
        //entitiesList.prepend("00-Not Identified (000)");
        entityNameComboBox->addItems(entitiesList);
    }
}

void MainWindowInputOthers::setEntity(const int _ent)
{// Select the appropriate entity in the ComboBox
       //qDebug() << "MainWindowInputOthers::setEntity: " << QString::number(_ent) << QT_ENDL;
    if (_ent<=0)
    {
        entityNameComboBox->setCurrentIndex(0);
        return;
    }
    QString aux = QString();
    aux = QString::number(_ent);
    if (_ent > 1000)
    {
        aux = (QString::number(_ent)).right(3);
    }

    //QString pref = dataProxy->getEntityNameFromId(_ent);

    //int indexC = entityNameComboBox->f
    int indexC = entityNameComboBox->findText("(" + aux + ")", Qt::MatchEndsWith);

       //qDebug() << "MainWindow::selectCorrectEntity: " << pref << "/" << QString::number(indexC) << QT_ENDL;
    entityNameComboBox->setCurrentIndex(indexC);
    setIOTAContinentFromEntity(_ent);

}

int MainWindowInputOthers::getEntity()
{
    QString aux = entityNameComboBox->currentText();
    if (!aux.contains ('('))
    {
        return -1;
    }
    //qDebug() << Q_FUNC_INFO << ": " << (aux.split('(').at(1).chopped(1));
    //return (aux.split('(').at(1).chopped(1)).toInt ();
    QString a = aux.split('(').at(1);
    a.chop(1);
    //qDebug() << Q_FUNC_INFO << ": " << a;
    return a.toInt ();
}

QString MainWindowInputOthers::getEntityPrefix()
{
    //qDebug() << "MainWindowInputOthers::getEntityPrefix: " << (entityNameComboBox->currentText()).split('-').at(0) << QT_ENDL;
    return (entityNameComboBox->currentText()).split('-').at(0);
    //return world->getQRZARRLId(pref);
}

void MainWindowInputOthers::setPropMode(const QString &_qs, bool _keep)
{
      //qDebug() << "MainWindowInputOthers::setPropMode: " << _qs << QT_ENDL;
    autoUpdating = true;
    if(( propModeComboBox->findText(_qs+" -", Qt::MatchContains))>0)
    {
        propModeComboBox->setCurrentIndex( propModeComboBox->findText(_qs+" -", Qt::MatchContains));
        keepPropCheckBox->setChecked(_keep);
    }
    else
    {
        propModeComboBox->setCurrentIndex(0);
        keepPropCheckBox->setChecked(false);
    }
    autoUpdating = false;
}

QString MainWindowInputOthers::getPropModeFromComboBox()
{
    QString _pm = QString();
      //qDebug() << "MainWindow::getPropModeFromComboBox:" << propModeComboBox->currentText() << QT_ENDL;
    _pm = (((propModeComboBox->currentText()).split('-')).at(1)).simplified();
    QString _n = (((propModeComboBox->currentText()).split('-')).at(0)).simplified();
      //qDebug() << "MainWindow::getPropModeFromComboBox: " << _pm << QT_ENDL;

    if (_n == "00")
    {
        return QString();
    }
    return _pm;
}

bool MainWindowInputOthers::isSATPropagation()
{
    if (getPropModeFromComboBox() == "SAT")
    {
        return true;
    }
    else
    {
        return false;
    }
}


void MainWindowInputOthers::clearIOTA()
{
    iotaContinentComboBox->setCurrentIndex(0);
    iotaNumberLineEdit->setText("000");
    //iotaNumberLineEdit->setPalette(palBlack);
}

bool MainWindowInputOthers::isIOTAModified()
{
    if ((iotaContinentComboBox->currentIndex()>0) || (iotaNumberLineEdit->text()).toInt()>0)
    {
        return true;
    }
    else
    {
        return false;
    }

}

void MainWindowInputOthers::setIOTA(const QString &_qs)
{//TODO: Seems to be better to send the color info like in: (it is much more flexible as I can send any color!)

    //void MainWindowInputQSL::setQSLVia(const QString &_qs, QColor qColor)
      //qDebug() << "MainWindow::setIOTA: " << _qs << QT_ENDL;
    if ( (checkIfValidIOTA(_qs)).length() !=6 )
    {
        iotaNumberLineEdit->setPalette(palRed);
        return;
    }
    else
    {
        QStringList values = _qs.split("-", QT_SKIP);
          //qDebug() << "MainWindowInputOthers::setIOTA: IOTA " << _qs << QT_ENDL;
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
}

QString MainWindowInputOthers::getIOTA()
{
    return (checkIfValidIOTA(iotaContinentComboBox->currentText() + "-" + iotaNumberLineEdit->text()));
}


void MainWindowInputOthers::setIOTAContinentFromEntity(const int _n)
{
      //qDebug() << "MainWindow::setIOTAContinentFromEntity:" << QString::number(_n) << QT_ENDL;
    setIOTAContinent(dataProxy->getContinentShortNameFromEntity(_n)) ;
}

void MainWindowInputOthers::setIOTAContinent(const QString &_qs)
{
       //qDebug() << "MainWindowInputOthers::setIOTAContinent: " << _qs << QT_ENDL;
       //qDebug() << "MainWindowInputOthers::setIOTAContinent: setting to index(a): " << QString::number(iotaContinentComboBox->findText(_qs, Qt::MatchContains)) << QT_ENDL;
    if(( iotaContinentComboBox->findText(_qs, Qt::MatchContains))>0)
    {
          //qDebug() << "MainWindowInputOthers::setIOTAContinent: setting to index: " << QString::number(iotaContinentComboBox->findText(_qs, Qt::MatchContains)) << QT_ENDL;
        iotaContinentComboBox->setCurrentIndex( iotaContinentComboBox->findText(_qs, Qt::MatchContains));
    }
    else
    {
           //qDebug() << "MainWindowInputOthers::setIOTAContinent: setting to index: 00" << QT_ENDL;
        iotaContinentComboBox->setCurrentIndex(0);
    }
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
      //qDebug() << "MainWindowInputOthers::checkIfValidIOTA: " << _tiota << QT_ENDL;
    //bool _valid = false;
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
        return "";
    }

      //qDebug() << "MainWindowInputOthers::checkIfValidIOTA (cont) " << _continent << QT_ENDL;
      //qDebug() << "MainWindowInputOthers::checkIfValidIOTA (numb): " << _number << QT_ENDL;

    // Check if continent is valid

    if (dataProxy->isValidContinentShortName(_continent))
    {
        if ( (_number.toInt() >0 ) && ((_number.toInt()) < 1000 ))
        {
            if ((_number.length()) == 3)
            {
                return _continent + "-" + _number ;
            }
            else if ((_number.length()) == 2)
            {
                return _continent + "-0" + QString::number((_number).toInt());
            }
            else if ((_number.length()) == 1)
            {
                return _continent + "-00" + QString::number((_number).toInt());
            }
            else
            {
                return "";
            }
        }
        else
        {
            return "";
        }
    }
    else
    {
        return QString();
    }
}

void MainWindowInputOthers::slotPropModeComboBoxChanged()
{
    if (autoUpdating)
    {
        return;
    }
    emit setPropMode(getPropModeFromComboBox());

}


void MainWindowInputOthers::setKeep(const bool _b)
{
    keepPropCheckBox->setChecked (_b);
}

bool MainWindowInputOthers::getKeep()
{
    return keepPropCheckBox->isChecked ();
}

bool MainWindowInputOthers::setUserADIFTypeComboBox(const QString &_value)
{
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
        return false;
    }
    return true;
}

QString MainWindowInputOthers::getUserADIFTypeComboBox()
{
    int value = (((userDefinedADIFComboBox->currentText ()).split('-')).at(0)).toInt ();
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(value);
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(value);
    switch (value)
    {
    case 1:
        return "SOTA_REF";
    case 2:
        return "AGE";
    case 3:
        return "VUCC_GRIDS";
    default:
        return QString();
    }
}

bool MainWindowInputOthers::setUserADIFValue(const QString &_adifValue)
{
    userDefinedADIFValueLineEdit->setText (_adifValue);
    return true;
}

QString MainWindowInputOthers::getUserADIFValue()
{
    return userDefinedADIFValueLineEdit->text();

}

bool MainWindowInputOthers::setInitialADIFValues()
{
    adifValidTypes << "01-" + tr("SOTA Ref") << "02-" + tr ("Age") << "03-" + tr ("VUCC grids");
    userDefinedADIFComboBox->clear ();
    userDefinedADIFComboBox->addItems (adifValidTypes);
    return true;
}

void MainWindowInputOthers::slotUSerDefinedADIFComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO << ": " << getUserADIFTypeComboBox ();
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
}

bool MainWindowInputOthers::setVUCCGrids(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    if (checkVUCC_GRIDS(_op))
    {
        vucc_grids = _op;
        slotUSerDefinedADIFComboBoxChanged();
        return true;
    }
    return false;
}

bool MainWindowInputOthers::checkVUCC_GRIDS(const QString &_string)
{
    //qDebug() << Q_FUNC_INFO << ": " << _string;

    if (util->isValidVUCCGrids (_string))
    {
        setColorsForUserDefinedADIFValueLineEdit();
        return true;
    }
    else
    {
        userDefinedADIFValueLineEdit->setPalette (palRed);
        return false;
    }

}

QString MainWindowInputOthers::getVUCCGrids()
{
    if (checkVUCC_GRIDS (vucc_grids))
    {
        return vucc_grids;
    }
    else
    {
        return QString();
    }
}

void MainWindowInputOthers::setColorsForUserDefinedADIFValueLineEdit()
{
    if (getDarkMode())
    {
        userDefinedADIFValueLineEdit->setPalette(palWhite);
    }
    else
    {
        userDefinedADIFValueLineEdit->setPalette(palBlack);
    }
}


bool MainWindowInputOthers::setSOTA(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    sota_ref = _op;
    slotUSerDefinedADIFComboBoxChanged();
    return true;
}

QString MainWindowInputOthers::getSOTA()
{
    //qDebug() << Q_FUNC_INFO;
    return sota_ref;
}
bool MainWindowInputOthers::setAge(const double _op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    age = _op;
    slotUSerDefinedADIFComboBoxChanged();
    return true;
}

double MainWindowInputOthers::getAge()
{
    return age;
}

void MainWindowInputOthers::slotSetCurrentUSerData()
{
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
        //if (checkVUCC_GRIDS(userDefinedADIFValueLineEdit->text()))
        //{}
        vucc_grids = userDefinedADIFValueLineEdit->text().toUpper();
        userDefinedADIFValueLineEdit->setText (vucc_grids);
    }
}

bool MainWindowInputOthers::getDarkMode()
{
    if ( iotaNumberLineEdit->palette().color (QPalette::Base) == "#646464")
    {
        return true;
    }
    else
    {
        return false;
    }
}

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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/


#include "mainwindowinputothers.h"

MainWindowInputOthers::MainWindowInputOthers(QWidget *parent) :
    QWidget(parent)
{
   //qDebug() << "MainWindowInputOthers::MainWindowInputOthers" << endl;
    entitiesList.clear();
    propModeList.clear();

    dataProxy = new DataProxy_SQLite();
    propModeList = dataProxy->getPropModeList();

    //QLabel *entityPrimLabel, *entitySecLabel, *iotaAwardLabel, *entityNameLabel, *propModeLabel;
    iotaContinentComboBox = new QComboBox();
    entityPrimDivComboBox = new QComboBox();
    entitySecDivComboBox = new QComboBox();
    entityNameComboBox = new QComboBox();
    propModeComboBox = new QComboBox();
    iotaNumberLineEdit = new QLineEdit();

    // TODO: I should find the way to connect the SAT tabwidget's signal to set the propmode in this widget
    //       Now it is done though the mainwindow but I should avoid depending on that class for that, if possible
    //connect(satTabWidget, SIGNAL(setPropModeSat(QString)), this, SLOT(slotSetPropMode(QString)) ) ;


    createUI();
}

MainWindowInputOthers::~MainWindowInputOthers(){}

void MainWindowInputOthers::createUI()
{
   //qDebug() << "MainWindowInputOthers::createUI" << endl;

    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);

    QLabel *entityPrimLabel = new QLabel(tr("Primary Div"));
    QLabel *entitySecLabel = new QLabel(tr("Secondary Div"));
    QLabel *iotaAwardLabel = new QLabel(tr("IOTA"));
    QLabel *entityNameLabel = new QLabel(tr("Entity"));
    QLabel *propModeLabel = new QLabel(tr("Propagation mode"));

    entityPrimLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    entitySecLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    iotaAwardLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    entityNameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    propModeLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    entityPrimDivComboBox->setToolTip(tr("Select the primary division for this QSO"));
    entitySecDivComboBox->setToolTip(tr("Select the secondary division for this QSO"));
    entityNameComboBox->setToolTip(tr("Select the entity for this QSO"));
    propModeComboBox->setToolTip(tr("Select the propagation mode for this QSO"));
    iotaContinentComboBox->setToolTip(tr("Select the IOTA continent for this QSO"));
    iotaNumberLineEdit->setToolTip(tr("Select the IOTA reference number for this QSO"));

    entityPrimDivComboBox->setEnabled(false);
    entitySecDivComboBox->setEnabled(false);
    entityNameComboBox->setEnabled(true);
    propModeComboBox->setEnabled(true);

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
    tabLayout->addWidget(propModeComboBox, 4, 1, 1, 2);

    setLayout(tabLayout);

    if (entitiesList.size()>1)
    {
        entitiesList.prepend("00-Not Identified (000)");
        entityNameComboBox->addItems(entitiesList);
    }

    if (propModeList.size()>1)
    {
        propModeList.prepend("00 - Not - Not Identified");
        propModeComboBox->addItems(propModeList);
    }
/*

    QStringList continents;

    QSqlQuery query2("SELECT shortname FROM continent");
    while (query2.next()) {
        if (query2.isValid())
        {
            continents << query2.value(0).toString();
        }
    }
*/
    iotaContinentComboBox->addItems(dataProxy->getContinentShortNames());
    iotaNumberLineEdit->setInputMask("000");
    iotaNumberLineEdit->setText("000");
}

void MainWindowInputOthers::clear()
{
   //qDebug() << "MainWindowInputOthers::clear" << endl;
    entityNameComboBox->setCurrentIndex(0);
    propModeComboBox->setCurrentIndex(0);
    iotaContinentComboBox->setCurrentIndex(0);
    iotaNumberLineEdit->setText("000");
}

void MainWindowInputOthers::setEntitiesList(const QStringList _qs)
{
   //qDebug() << "MainWindowInputOthers::setEntitiesList: " << QString::number(_qs.length()) << endl;
    entitiesList.clear();
    entitiesList << _qs;
    if (entitiesList.size()>1)
    {
        entitiesList.prepend("00-Not Identified (000)");
        entityNameComboBox->addItems(entitiesList);
    }
}

void MainWindowInputOthers::setEntity(const int _ent)
{// Select the appropriate entity in the ComboBox
    //qDebug() << "MainWindowInputOthers::setEntity: " << QString::number(_ent) << endl;
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
    QString pref = QString();

    //pref = world->getEntityMainPrefix(_ent);
    //pref = world->getEntityName(_ent);
    pref = dataProxy->getEntityNameFromId(_ent);

    //int indexC = entityNameComboBox->findText(pref, Qt::MatchContains);
    //qDebug() << "MainWindow::selectCorrectEntity: aux to the findText: " << aux << endl;
    //int indexC = entityNameComboBox->findText("(" + aux + ")", Qt::MatchContains);
    int indexC = entityNameComboBox->findText("(" + aux + ")", Qt::MatchEndsWith);

    //qDebug() << "MainWindow::selectCorrectEntity: " << pref << "/" << QString::number(indexC) << endl;
    entityNameComboBox->setCurrentIndex(indexC);
    setIOTAContinentFromEntity(_ent);

}

QString MainWindowInputOthers::getEntityPrefix()
{
   //qDebug() << "MainWindowInputOthers::getEntityName" << endl;
    return (entityNameComboBox->currentText()).split('-').at(0);
    //qDebug() << "MainWindowInputOthers::getEntity: " << pref << "/" << QString::number(world->getQRZARRLId(pref))<< endl;
    //return world->getQRZARRLId(pref);
}

void MainWindowInputOthers::setPropMode(const QString _qs)
{
   //qDebug() << "MainWindowInputOthers::setPropMode: " << _qs << endl;
    if(( propModeComboBox->findText(_qs+" -", Qt::MatchContains))>0)
    {
        propModeComboBox->setCurrentIndex( propModeComboBox->findText(_qs+" -", Qt::MatchContains));
    }
    else
    {
        propModeComboBox->setCurrentIndex(0);
    }
}

QString MainWindowInputOthers::getPropModeFromComboBox()
{
    QString _pm = QString();
  //qDebug() << "MainWindow::getPropModeFromComboBox:" << propModeComboBox->currentText() << endl;
    _pm = (((propModeComboBox->currentText()).split('-')).at(1)).simplified();
  //qDebug() << "MainWindow::getPropModeFromComboBox: " << _pm << endl;
    if (_pm == "Not")
    {
        return QString();
    }
    return _pm;
}

void MainWindowInputOthers::clearIOTA()
{
    iotaContinentComboBox->setCurrentIndex(0);
    iotaNumberLineEdit->setText("000");
    iotaNumberLineEdit->setPalette(palBlack);
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
    return false;
}

void MainWindowInputOthers::setIOTA(const QString _qs, const bool _black)
{//TODO: Seems to be better to send the color info like in: (it is much more flexible as I can send any color!)

    //void MainWindowInputQSL::setQSLVia(const QString _qs, QColor qColor)
   //qDebug() << "MainWindow::setIOTA: " << _qs << endl;
    if ( (checkIfValidIOTA(_qs)).length() !=6 )
    {
        return;
    }
    else
    {
        QStringList values = _qs.split("-", QString::SkipEmptyParts);
       //qDebug() << "MainWindowInputOthers::setIOTA: IOTA " << _qs << endl;
        iotaContinentComboBox->setCurrentIndex( iotaContinentComboBox->findText(values.at(0) ) );
        iotaNumberLineEdit->setText(values.at(1));
    }
    if (_black)
    {
        iotaNumberLineEdit->setPalette(palBlack);
    }
    else
    {
        iotaNumberLineEdit->setPalette(palRed);
    }
}

QString MainWindowInputOthers::getIOTA()
{
    return (checkIfValidIOTA(iotaContinentComboBox->currentText() + "-" + iotaNumberLineEdit->text()));
}


void MainWindowInputOthers::setIOTAContinentFromEntity(const int _n)
{
   //qDebug() << "MainWindow::setIOTAContinentFromEntity:" << QString::number(_n) << endl;
    setIOTAContinent(dataProxy->getContinentShortNameFromEntity(_n)) ;
}

void MainWindowInputOthers::setIOTAContinent(const QString _qs)
{
    //qDebug() << "MainWindowInputOthers::setIOTAContinent: " << _qs << endl;
    //qDebug() << "MainWindowInputOthers::setIOTAContinent: setting to index(a): " << QString::number(iotaContinentComboBox->findText(_qs, Qt::MatchContains)) << endl;
    if(( iotaContinentComboBox->findText(_qs, Qt::MatchContains))>0)
    {
       //qDebug() << "MainWindowInputOthers::setIOTAContinent: setting to index: " << QString::number(iotaContinentComboBox->findText(_qs, Qt::MatchContains)) << endl;
        iotaContinentComboBox->setCurrentIndex( iotaContinentComboBox->findText(_qs, Qt::MatchContains));
    }
    else
    {
        //qDebug() << "MainWindowInputOthers::setIOTAContinent: setting to index: 00" << endl;
        iotaContinentComboBox->setCurrentIndex(0);
    }
}

QString MainWindowInputOthers::checkIfValidIOTA(const QString _tiota)
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
   //qDebug() << "MainWindowInputOthers::checkIfValidIOTA: " << _tiota << endl;
    bool _valid = false;
    QString _continent;
    QString _number;

    if (_tiota.count("-") == 1)
    {
        QStringList _values = _tiota.split("-", QString::SkipEmptyParts);
        _continent = _values.at(0);
        _number = _values.at(1);
    }
    else
    {
        return "";
    }

   //qDebug() << "MainWindowInputOthers::checkIfValidIOTA (cont) " << _continent << endl;
   //qDebug() << "MainWindowInputOthers::checkIfValidIOTA (numb): " << _number << endl;

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
    return QString();
}

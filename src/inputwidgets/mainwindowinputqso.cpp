/***************************************************************************
                          mainwindowinputqso.cpp  -  description
                             -------------------
    begin                : may 2021
    copyright            : (C) 2021 by Jaime Robles
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
//
// This class implement the tab of the mainwindow that supports the QSL options
//

#include "mainwindowinputqso.h"

MainWindowInputQSO::MainWindowInputQSO(DataProxy_SQLite *dp, QWidget *parent) :
    QWidget(parent)
{
       //qDebug() << "MainWindowInputQSO::MainWindowInputQSO"   << endl;
    locator = new Locator();

    nameLineEdit = new QLineEdit;
    qthLineEdit = new QLineEdit;
    locatorLineEdit = new QLineEdit;
    rstTXLineEdit = new QLineEdit;
    rstRXLineEdit = new QLineEdit;
    txFreqSpinBox = new QDoubleSpinBox;
    rxFreqSpinBox = new QDoubleSpinBox;
    splitCheckBox = new QCheckBox;
    rxPowerSpinBox = new QDoubleSpinBox;

    dataProxy = dp;


    createUI();
    setDefaultData();
    clear();
       //qDebug() << "MainWindowInputQSO::MainWindowInputQSO - END"   << endl;

}

MainWindowInputQSO::~MainWindowInputQSO(){}

void MainWindowInputQSO::createUI()
{
    // QSL Tab definition starts here

    connect(rstTXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(rstRXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(locatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(qthLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(nameLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );

    connect(locatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotLocatorTextChanged() ) );
}


void MainWindowInputQSO::setDefaultData()
{



}
void MainWindowInputQSO::clear()
{


}



void MainWindowInputQSO::slotReturnPressed()
{
    emit returnPressed();
}


void MainWindowInputQSO::slotLocatorTextChanged()
{//TO BE REMOVED ONCE InfoWidget is FINISHED - At least modified
    qDebug() << Q_FUNC_INFO << ": " << locatorLineEdit->text() << endl;
    int cursorP = locatorLineEdit->cursorPosition();

    locatorLineEdit->setText((locatorLineEdit->text()).toUpper());

    if ( locator->isValidLocator((locatorLineEdit->text()).toUpper()) )
    {
        qDebug() << Q_FUNC_INFO << ": VALID: " << locatorLineEdit->text() << endl;
        locatorLineEdit->setPalette(palBlack);
        emit dxLocatorChanged (locatorLineEdit->text());

        //dxLocator = (locatorLineEdit->text());
        //infoWidget->showDistanceAndBearing(myDataTabWidget->getMyLocator(), dxLocator);
        //satTabWidget->setLocator(dxLocator);
        locatorLineEdit->setToolTip(tr("DX QTH locator."));
        qDebug() << Q_FUNC_INFO << ": " << locator->getLat(locatorLineEdit->text()) << endl;
        qDebug() << Q_FUNC_INFO << ": LON: " << locator->getLon(locatorLineEdit->text()) << endl;

    }
    else
    {
        qDebug() << Q_FUNC_INFO << ": NOT VALID: " << locatorLineEdit->text() << endl;
        locatorLineEdit->setPalette(palRed);
        locatorLineEdit->setToolTip(tr("DX QTH locator. Format should be Maidenhead like IN70AA up to 10 characters."));
        locatorLineEdit->setCursorPosition(cursorP);

        return;
    }
    locatorLineEdit->setCursorPosition(cursorP);

}

QString MainWindowInputQSO::getDXLocator()
{
    return locatorLineEdit->text();
}
void MainWindowInputQSO::setDXLocator(const QString &_loc)
{
    if (locator->isValidLocator (_loc))
    {
        locatorLineEdit->setText (_loc.toUpper ());
    }
}

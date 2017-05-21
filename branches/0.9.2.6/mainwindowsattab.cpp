
/***************************************************************************
                          mainwindowsattab.cpp  -  description
                             -------------------
    begin                : Jul 2016
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
#include "mainwindowsattab.h"
/*
This class implements the Satellite TAB of the MainWindow
*/
MainWindowSatTab::MainWindowSatTab(QWidget *parent) :
    QWidget(parent)
{
    satNameComboBox = new QComboBox;
    satNameLineEdit = new QLineEdit;
    satModeLineEdit = new QLineEdit;
    satOtherLabel = new QLabel;

    keepThisDataForNextQSORadiobutton = new QRadioButton;

    dataProxy = new DataProxy_SQLite();

    createUI();
    populateSatComboBox();
    satNameLineEdit->setEnabled(false);
    satOtherLabel->setEnabled(false);
}

MainWindowSatTab::~MainWindowSatTab(){}

void MainWindowSatTab::createUI()
{
    connect(satNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSatNameTextChanged() ) );
    connect(satModeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSatModeTextChanged() ) );
    connect(satNameComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotSatNameComboBoxChanged() ) ) ;

    QLabel *keepLabel = new QLabel();
    keepLabel->setText(tr("Keep this data"));
    keepLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    keepLabel->setToolTip(tr("Data entered in this tab will be copied into the next QSO"));

    keepThisDataForNextQSORadiobutton->setToolTip(tr("Data entered in this tab will be copied into the next QSO"));
    QString othersat = tr("Other - Sat not in the list");
    QString aux;
    aux.clear();
    aux = QString(tr("Name of the Satellite if not in the list. Select: \"")) + othersat + QString(tr("\" to enable this box. (format like AO-51)"));
    satNameLineEdit->setToolTip(aux);
    //satNameLineEdit->setToolTip(tr("Name of the Satellite if not in the list. Select Other Sat (format like AO-51)"));
    satModeLineEdit->setToolTip(tr("Satellite mode used"));
    satNameComboBox->setToolTip(tr("Select the satellite you are using"));

    QLabel *satNameLabel = new QLabel();
    satNameLabel->setText(tr("Satellite"));
    satNameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *satModeLabel = new QLabel();
    satModeLabel->setText(tr("Mode"));
    satModeLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    //QLabel *satOtherLabel = new QLabel();
    satOtherLabel->setText(tr("Other"));
    satOtherLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QGridLayout *tabLayout = new QGridLayout;
    tabLayout->addWidget(satNameLabel, 0, 0);    
    tabLayout->addWidget(satNameComboBox, 0, 1);

    tabLayout->addWidget(satModeLabel, 1, 0);
    tabLayout->addWidget(satModeLineEdit, 1, 1);

    tabLayout->addWidget(satOtherLabel, 2, 0);
    tabLayout->addWidget(satNameLineEdit, 2, 1);

    tabLayout->addWidget(keepLabel, 3, 1);
    tabLayout->addWidget(keepThisDataForNextQSORadiobutton, 3, 2);

    setLayout(tabLayout);

}

void MainWindowSatTab::slotSatNameComboBoxChanged()
{
    int i = satNameComboBox->currentIndex();
   //qDebug() << "MainWindowSatTab::slotSatNameComboBoxChanged: " << QString::number(i) << endl;
    //QString _pm = (((satNameComboBox->currentText()).split(' ')).at(0)).simplified();

    satNameLineEdit->clear();

    if (i == 0)
    {
        emit setPropModeSat("Not");
        satNameLineEdit->setEnabled(false);
        satOtherLabel->setEnabled(false);

    }
    else if(i == 1)
    {
        emit setPropModeSat("SAT");
        satNameLineEdit->setEnabled(true);
        satOtherLabel->setEnabled(true);
    }
    else
    {
        emit setPropModeSat("SAT");
        satNameLineEdit->setEnabled(false);
        satOtherLabel->setEnabled(false);
    }

}

void MainWindowSatTab::slotSatNameTextChanged()
{
   //qDebug() << "MainWindowSatTab::slotSatNameTextChanged: " << satNameLineEdit->text() << endl;
    satNameLineEdit->setText((satNameLineEdit->text()).toUpper());

    if ((satNameLineEdit->text()).length()>0)
    {
        emit setPropModeSat("SAT");
    }
    else if ((satModeLineEdit->text()).length()<1)
    {
        emit setPropModeSat("Not");
    }

}


void MainWindowSatTab::slotSatModeTextChanged()
{
   //qDebug() << "MainWindowSatTab::slotSatModeTextChanged: " << satModeLineEdit->text() << endl;
/*
    satModeLineEdit->setText((satModeLineEdit->text()).toUpper());

    if ((satModeLineEdit->text()).length()>0)
    {
        emit setPropModeSat("SAT");
    }
    else if ((satNameLineEdit->text()).length()<1)
    {
        emit setPropModeSat("Not");
    }
*/

}

QString MainWindowSatTab::getSatName()
{
    // Sat name must follow the format CC-NN to make it compatible with LOTW
    // C = Character
    // N = Number
/*
    QString satName;
    satName = satNameLineEdit->text();
    //TODO: Check that the format is OK
    return satName;
 */
    QString _pm = QString();
    QString satName = QString();


   //qDebug() << "MainWindowSatTab::getSatName:" << satNameComboBox->currentText() << endl;
    _pm = (((satNameComboBox->currentText()).split(' ')).at(0)).simplified();

    //qDebug() << "MainWindowSatTab::satNameComboBox: " << _pm << endl;
    if (satNameComboBox->currentIndex() == 0)
    {
        return QString();
    }
    else if(satNameComboBox->currentIndex() == 1)
    {
        satName = satNameLineEdit->text();
        if (satName.length()>0)
        {
            return satName;
        }
        else
        {
            return QString();
        }
    }
    else
    {
        return _pm;
    }

}

void MainWindowSatTab::setSatName(const QString _t)
{
     //TODO: Check that the format is OK
    //satNameLineEdit->setText(_t);
    if (getSatIndex(_t) > 0)
    {
        setSatelliteCombo(_t);
    }
    else
    {
        satNameComboBox->setCurrentIndex(1);
        satNameLineEdit->setText(_t);
    }

}

QString MainWindowSatTab::getSatMode()
{
    return satModeLineEdit->text();
}

void MainWindowSatTab::setSatMode(const QString _t)
{

    if (_t == "-CLEAR-")
    {
        satModeLineEdit->clear();
    }
    else
    {
        satModeLineEdit->setText(_t);
    }

}

bool MainWindowSatTab::getRepeatThis()
{
    return keepThisDataForNextQSORadiobutton->isChecked();
}

void MainWindowSatTab::setRepeatThis(const bool _t)
{
    keepThisDataForNextQSORadiobutton->setChecked(_t);
}

void MainWindowSatTab::clear()
{
    if (keepThisDataForNextQSORadiobutton->isChecked())
    {
        return;
    }
    else
    {
        satModeLineEdit->clear();
        satNameComboBox->setCurrentIndex(0);
    }

}

void MainWindowSatTab::populateSatComboBox()
{
    //qDebug() << "MainWindowSatTab::populateSatComboBox: " << endl;

    QString nosat = tr("Not Sat QSO");
    QString othersat = tr("Other - Sat not in the list");
    satellitesList.clear();
    satellitesList = dataProxy->getSatellitesList();
    satellitesList.prepend(othersat);
    satellitesList.prepend("No-SAT - " + nosat);

    if (satellitesList.size()>1)
    {

        satNameComboBox->addItems(satellitesList);
    }
    else
    {
        //TODO: Check how to do it better... now I could simply remove the if
        satNameComboBox->addItems(satellitesList);
    }
}

void MainWindowSatTab::setSatelliteCombo(const QString _p)
{
    //qDebug() << "MainWindowsatTab::setSatelliteCombo: " << _p << endl;
    QString aux = QString();
    int indexC = getSatIndex(_p);
    //int indexC = satNameComboBox->findText(_p, Qt::MatchContains);
    //qDebug() << "MainWindowsatTab::setSatelliteCombo: N=" << QString::number(indexC) << endl;
    if (indexC>0)
    {
        satNameComboBox->setCurrentIndex(indexC);
    }
    else
    {
        satNameComboBox->setCurrentIndex(0);
        if (_p.length()>0)
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            aux = tr("A satellite name has been detected but is not on the KLog satellite's names table. Please check that the correct satellite is selected and if the satellite is not in the list, please contact the development team to include it.\n\n");
            msgBox.setText(aux + tr("The satellite you have in your QSO is: ") + _p + "\n\nPlease know that the satellite name will not be saved if it is not in the list so that information may be lost!");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();
            switch (ret)
            {
                case QMessageBox::Ok:
                break;
                default:
                // should never be reached
                break;
            }
        }
    }

}

void MainWindowSatTab::setOtherSatName(const QString _t)
{
    satNameLineEdit->setText(_t);
}

QString MainWindowSatTab::getOtherSatName()
{
    return QString();
}

int MainWindowSatTab::getSatIndex(const QString _p)
{
    return satNameComboBox->findText(_p, Qt::MatchContains);
}

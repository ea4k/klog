/***************************************************************************
                           mapwidget.cpp  -  description
                             -------------------
    begin                : May 2021
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/
#include "mapwindowwidget.h"

MapWindowWidget::MapWindowWidget(DataProxy_SQLite *dp, QWidget *parent)
{

    //qDebug() << Q_FUNC_INFO;
    dataProxy = dp;
    mapWidget = new MapWidget;

    propComboBox = new QComboBox;
    bandComboBox = new QComboBox;
    modeComboBox = new QComboBox;
    satNameComboBox = new QComboBox;
    confirmedCheckBox = new QCheckBox;


    //locatorsCheckBox = new QCheckBox;

    //qDebug() << Q_FUNC_INFO << " - END";
}

MapWindowWidget::~MapWindowWidget()
{
    delete(dataProxy);
    delete(mapWidget);
}

void MapWindowWidget::init()
{
    newOneColor = Qt::black;
    neededColor = Qt::black;
    workedColor = Qt::black;
    confirmedColor = Qt::black;
    defaultColor = Qt::black;
    createUI();
}

void MapWindowWidget::createUI()
{

    bandComboBox->setToolTip(tr("Select QSOs in this band."));
    modeComboBox->setToolTip(tr("Select QSOs in this mode."));
    propComboBox->setToolTip(tr("Select QSOs in this propagation mode."));
    satNameComboBox->setToolTip(tr("Select QSOs using this Satellite."));
    confirmedCheckBox->setText(tr("Only confirmed"));

    confirmedCheckBox->setToolTip(tr("Select only confirmed QSOs."));
    //locatorsCheckBox->setText(tr("Show Locators"));
    //locatorsCheckBox->setToolTip(tr("Show Locators or QSO positons."));

    QGridLayout *buttonsLayout = new QGridLayout;
    buttonsLayout->addWidget(bandComboBox, 0, 0);
    buttonsLayout->addWidget(modeComboBox, 0, 1);
    buttonsLayout->addWidget(propComboBox, 1, 0);
    buttonsLayout->addWidget(satNameComboBox, 1, 1);
    //buttonsLayout->addWidget(okButton, 1, 2);
    buttonsLayout->addWidget(confirmedCheckBox, 0, 2);
    //buttonsLayout->addWidget(locatorsCheckBox, 1, 2);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(buttonsLayout);
    layout->addWidget(mapWidget);
    setLayout (layout);

    setPropModes();
    setSatNames();

    mapWidget->init();

    connect(bandComboBox, SIGNAL(currentIndexChanged (QString)), this, SLOT(slotBandsComboBoxChanged()));
    connect(modeComboBox, SIGNAL(currentIndexChanged (QString)), this, SLOT(slotModesComboBoxChanged()));
    connect(propComboBox, SIGNAL(currentIndexChanged (QString)), this, SLOT(slotPropComboBoxChanged()));
    connect(satNameComboBox, SIGNAL(currentIndexChanged (QString)), this, SLOT(slotSatsComboBoxChanged()));
    connect(confirmedCheckBox, SIGNAL(clicked()), this, SLOT(slotConfirmedCheckBoxChanged()));
    //connect(locatorsCheckBox, SIGNAL(clicked()), this, SLOT(slotLocatorsCheckBoxChanged()));

    satNameComboBox->setEnabled(false); // Starts disable until propagation = SAT
}

void MapWindowWidget::setCenter(const Coordinate &_c)
{
    mapWidget->setCenter(_c);
}

void MapWindowWidget::setBands(const QStringList _bands)
{
    QStringList bands;
    bands.clear();
    bands = _bands;
    bands.removeDuplicates();
    //bands = dataProxy->sortBandNamesBottonUp(bands);
    bandComboBox->clear();
    bands.prepend("All - " + tr("All bands"));
    bandComboBox->addItems(bands);
}

void MapWindowWidget::setModes(const QStringList _modes)
{
    QStringList modes;
    modes.clear();
    modes = _modes;
    modes.removeDuplicates();
    modes.sort();
    modes.prepend("All - " + tr("All modes"));
    modeComboBox->clear();
    modeComboBox->addItems(modes);
}

void MapWindowWidget::setPropModes()
{
    QStringList propModeList;
    propModeList.clear();
    propModeList = dataProxy->getPropModeList();
        if (propModeList.size()>1)
        {
            propModeList.prepend("All - " + tr("All propagation modes"));
            //propModeList.prepend("01 - " + tr("Not - Not Identified"));
            propComboBox->addItems(propModeList);
        }
}

void MapWindowWidget::setSatNames()
{
    //qDebug() << Q_FUNC_INFO << QT_ENDL;

    QString nosat = tr("All satellites");
    //QString othersat = tr("Other - Sat not in the list");
    QStringList satellitesList;
    satellitesList.clear();
    satellitesList = dataProxy->getSatellitesList();
    //satellitesList.prepend(othersat);
    satellitesList.prepend("All - " + nosat);

    if (satellitesList.size()>1)
    {
        satNameComboBox->clear();
        satNameComboBox->addItems(satellitesList);
    }
    else
    {
        //TODO: Check how to do it better... now I could simply remove the if
        satNameComboBox->addItems(satellitesList);
    }
}

void MapWindowWidget::showFiltered()
{
    QStringList locators;
    locators.clear();

    QStringList shortLocators;
    shortLocators.clear();

    QColor color;
    if (confirmedCheckBox->isChecked())
    {
        color = confirmedColor;
    }
    else
    {
        color = workedColor;
    }
    color.setAlpha(127);    // Little Transparent

    QString satName = satNameComboBox->currentText();


    locators << dataProxy->getFilteredLocators(bandComboBox->currentText(), modeComboBox->currentText(), getPropModeFromComboBox(), satName.section(' ', 0, 0), confirmedCheckBox->isChecked());
    foreach(QString i, locators)
    {
        if (i.length() == 4)
        {
            shortLocators << i;
        }
        else if (i.length()>4)
        {
            QString a =  i;
            a.truncate(4);
            shortLocators << a;
        }
    }
    shortLocators << locators;
    shortLocators.removeDuplicates();
    shortLocators.sort();

    addLocators(shortLocators, color);
}

void MapWindowWidget::slotBandsComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO;
   showFiltered();
   bandComboBox->setFocus();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::slotModesComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO;
    showFiltered();
    modeComboBox->setFocus();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::slotPropComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO;

    if (getPropModeFromComboBox() == "SAT")
    {
        //qDebug() << Q_FUNC_INFO << ": SAT";
        satNameComboBox->setCurrentIndex(0);
        satNameComboBox->setEnabled(true);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": NO SAT";
        satNameComboBox->setCurrentIndex(0);
        satNameComboBox->setEnabled(false);
    }
    showFiltered();
    propComboBox->setFocus();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::slotSatsComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO;
    showFiltered();
    satNameComboBox->setFocus();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::slotConfirmedCheckBoxChanged()
{
    //qDebug() << Q_FUNC_INFO;
    showFiltered();
    //qDebug() << Q_FUNC_INFO << " - END";
}


void MapWindowWidget::addQSO(const QString &_loc)
{
    //qDebug() << Q_FUNC_INFO << ": " << _loc;
}

void MapWindowWidget::addLocator(const QString &_loc, const QColor &_color)
{
    //qDebug() << Q_FUNC_INFO << ": " << _loc;
    //if (!locator.isValidLocator(_loc))
    //{
    //    return;
    //}
    mapWidget->addLocator(_loc, _color);
}

void MapWindowWidget::addLocators(const QStringList &_locators, const QColor &_color)
{
    mapWidget->clearMap();
    foreach(QString i, _locators)
    {
        //mapWidget->addLocator(i, confirmedColor);
        mapWidget->addLocator(i, _color);
    }
}

QString MapWindowWidget::getPropModeFromComboBox()
{
    QString _pm = QString();
     //qDebug() << Q_FUNC_INFO << ": " << propComboBox->currentText() << QT_ENDL;
    _pm = (((propComboBox->currentText()).split('-')).at(1)).simplified();
    QString _n = (((propComboBox->currentText()).split('-')).at(0)).simplified();
    //qDebug() << Q_FUNC_INFO << ": " << _pm << QT_ENDL;

    if (_n == "00")
    {
        return QString();
    }
    return _pm;
}

void MapWindowWidget::paintGlobalGrid()
{

}

void MapWindowWidget::setColors (const QColor &_newOne, const QColor &_needed, const QColor &_worked, const QColor &_confirmed, const QColor &_default)
{
       //qDebug() << "Awards::setColors: " << _newOne << "/" << _needed << "/" << _worked << "/" << _confirmed << "/" << _default << QT_ENDL;
    defaultColor = _default;
    neededColor = _needed;
    workedColor = _worked;
    confirmedColor = _confirmed;
    newOneColor = _newOne;
}

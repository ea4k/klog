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
 *                                                                          *
 *****************************************************************************/
#include "mapwindowwidget.h"

MapWindowWidget::MapWindowWidget(DataProxy_SQLite *dp, QWidget *parent)
{
    Q_UNUSED(parent);
    //qDebug() << Q_FUNC_INFO;
    dataProxy = dp;
    //qDebug() << Q_FUNC_INFO << "1";
    mapWidget = new MapWidget(this);
    //qDebug() << Q_FUNC_INFO << "2";
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
    //qDebug() << Q_FUNC_INFO << " - Start";
    delete(dataProxy);
    delete(mapWidget);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::init()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    workedColor = Qt::black;
    confirmedColor = Qt::black;
    defaultColor = Qt::black;
    createUI();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::addMarker(const Coordinate _coord, const QString _loc)
{
    Q_UNUSED(_loc);
    //qDebug() << Q_FUNC_INFO << QString(" %1 = %2/%3(lat/lon)").arg(_loc).arg(_coord.lat).arg(_coord.lon);
    mapWidget->addMarker(_coord);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::createUI()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
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
    //qDebug() << Q_FUNC_INFO << "3";
    mapWidget->init();
    //qDebug() << Q_FUNC_INFO << "4";

    connect(bandComboBox, SIGNAL(currentTextChanged (QString)), this, SLOT(slotBandsComboBoxChanged()));
    connect(modeComboBox, SIGNAL(currentTextChanged (QString)), this, SLOT(slotModesComboBoxChanged()));
    connect(propComboBox, SIGNAL(currentTextChanged (QString)), this, SLOT(slotPropComboBoxChanged()));
    connect(satNameComboBox, SIGNAL(currentTextChanged (QString)), this, SLOT(slotSatsComboBoxChanged()));
    connect(confirmedCheckBox, SIGNAL(clicked()), this, SLOT(slotConfirmedCheckBoxChanged()));

    //connect(locatorsCheckBox, SIGNAL(clicked()), this, SLOT(slotLocatorsCheckBoxChanged()));

    satNameComboBox->setEnabled(false); // Starts disable until propagation = SAT
    //qDebug() << Q_FUNC_INFO << "-END";
}

void MapWindowWidget::setCenter(const Coordinate &_c)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    mapWidget->setCenter(_c);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::setBands(const QStringList _bands)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    QStringList bands;
    bands.clear();
    bands = _bands;
    bands.removeDuplicates();
    //bands = dataProxy->sortBandNamesBottonUp(bands);
    bandComboBox->clear();
    bands.prepend("All - " + tr("All bands"));
    bands.prepend("None - " + tr("Show nothing"));
    bandComboBox->addItems(bands);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::setModes(const QStringList _modes)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    QStringList modes;
    modes.clear();
    modes = _modes;
    modes.removeDuplicates();
    modes.sort();
    modes.prepend("All - " + tr("All modes"));
    modeComboBox->clear();
    modeComboBox->addItems(modes);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::setPropModes()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    QStringList propModeList;
    propModeList.clear();
    propModeList = dataProxy->getPropModeList();
        if (propModeList.size()>1)
        {
            propModeList.prepend("All - " + tr("All propagation modes"));
            //propModeList.prepend("01 - " + tr("Not - Not Identified"));
            propComboBox->addItems(propModeList);
        }
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::setSatNames()
{
    //qDebug() << Q_FUNC_INFO << " - Start";

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
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::showFiltered()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    if (bandComboBox->currentIndex () == 0)
    {
        mapWidget->clearMap();
        //qDebug() << Q_FUNC_INFO << " - END1";
        return;
    }
    QStringList confirmedLocators;

    QStringList shortLocators, confirmedShortLocators;
    shortLocators.clear();
    confirmedShortLocators.clear ();

    QString satName = satNameComboBox->currentText();


    // Get Confirmed Locators
    // Print Confirmed
        // !only confirmed
        // Get worked locators
        // Remove confirmed from worked
        // Print Worked

    //locators << dataProxy->getFilteredLocators(bandComboBox->currentText(), modeComboBox->currentText(), getPropModeFromComboBox(), satName.section(' ', 0, 0), confirmedCheckBox->isChecked());

    confirmedLocators << dataProxy->getFilteredLocators(bandComboBox->currentText(), modeComboBox->currentText(), getPropModeFromComboBox(), satName.section(' ', 0, 0), true);
    QColor color;
    Locator locator;
    confirmedShortLocators << locator.getShortLocators (confirmedLocators);

    confirmedShortLocators << confirmedLocators;
    confirmedShortLocators.removeDuplicates();
    confirmedShortLocators.sort();
    color = confirmedColor;
    color.setAlpha (127);
    addLocators(confirmedShortLocators, color);// The alpha gives some transparency

    if (!confirmedCheckBox->isChecked ())
    {
        QStringList wLocators;
        wLocators.clear ();
        wLocators << dataProxy->getFilteredLocators(bandComboBox->currentText(), modeComboBox->currentText(), getPropModeFromComboBox(), satName.section(' ', 0, 0), false);
        QStringList workedLocators;
        workedLocators.clear ();

        foreach (QString loc, wLocators)
        {
            if (!confirmedShortLocators.contains (loc))
            {
                workedLocators.append (loc);
            }
        }

        shortLocators.clear();
        shortLocators << locator.getShortLocators (workedLocators);
        shortLocators << workedLocators;
        shortLocators.removeDuplicates();
        workedLocators.clear ();
        foreach (QString loc, shortLocators)
        {
            if (!confirmedShortLocators.contains (loc))
            {
                workedLocators.append (loc);
            }
        }
        workedLocators.sort();
        color = workedColor;
        color.setAlpha (127);// The alpha gives some transparency
        appendLocators(workedLocators, color);
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}


void MapWindowWidget::slotBandsComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
   showFiltered();
   bandComboBox->setFocus();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::slotModesComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    showFiltered();
    modeComboBox->setFocus();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::slotPropComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO << " - Start";

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
    //qDebug() << Q_FUNC_INFO << " - Start";
    showFiltered();
    satNameComboBox->setFocus();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::slotConfirmedCheckBoxChanged()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    showFiltered();
    //qDebug() << Q_FUNC_INFO << " - END";
}


void MapWindowWidget::addQSO(const QString &_loc)
{
    Q_UNUSED(_loc);
    //qDebug() << Q_FUNC_INFO << ": " << _loc;
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::addLocator(const QString &_loc, const QColor &_color)
{
    //qDebug() << Q_FUNC_INFO << ": " << _loc;
    //if (!locator.isValidLocator(_loc))
    //{
    //    return;
    //}
    mapWidget->addLocator(_loc, _color);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::addLocators(const QStringList &_locators, const QColor &_color)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    mapWidget->clearMap();
    foreach(QString i, _locators)
    {
        if (i.contains ("IN99"))
        {
            //qDebug() << Q_FUNC_INFO << ": " << i;
        }
        mapWidget->addLocator(i, _color);
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::appendLocators(const QStringList &_locators, const QColor &_color)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    foreach(QString i, _locators)
    {
        //mapWidget->addLocator(i, confirmedColor);
        mapWidget->addLocator(i, _color);
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}

QString MapWindowWidget::getPropModeFromComboBox()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    QString _pm = QString();
     //qDebug() << Q_FUNC_INFO << ": " << propComboBox->currentText();
    _pm = (((propComboBox->currentText()).split('-')).at(1)).simplified();
    QString _n = (((propComboBox->currentText()).split('-')).at(0)).simplified();
    //qDebug() << Q_FUNC_INFO << ": " << _pm;

    if (_n == "00")
    {
        //qDebug() << Q_FUNC_INFO << " - END1";
        return QString();
    }
    //qDebug() << Q_FUNC_INFO << " - END";
    return _pm;
}

void MapWindowWidget::paintGlobalGrid()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWindowWidget::setColors (const QColor &_worked, const QColor &_confirmed, const QColor &_default)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    defaultColor = _default;
    workedColor = _worked;
    confirmedColor = _confirmed;
    //qDebug() << Q_FUNC_INFO << " - END";
}

/***************************************************************************
                          setuppagecolors.cpp  -  description
                             -------------------
    begin                : nov 2011
    copyright            : (C) 2011 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify        *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

//#include <QDebug>

#include "setuppagecolors.h"


SetupPageColors::SetupPageColors(QWidget *parent) : QWidget(parent)
{
       //qDebug() << "SetupPageColors::SetupPageColors";

    newOneColorButton = new QPushButton;
    neededColorButton = new QPushButton;
    workedColorButton = new QPushButton;
    confirmedColorButton = new QPushButton;
    defaultColorButton = new QPushButton;
    wsjtxColorButton = new QPushButton;
    klogColorButton = new QPushButton;
    darkModeButton = new QPushButton;

    newOneColorButton->setText(tr("New One"));
    neededColorButton->setText(tr("Needed in this band"));
    workedColorButton->setText(tr("Worked in this band"));
    confirmedColorButton->setText(tr("Confirmed in this band"));
    defaultColorButton->setText(tr("Default"));
    wsjtxColorButton->setText(tr("WSJT-X palette"));
    klogColorButton->setText(tr("Default palette"));
    darkModeButton->setText(tr("Dark Mode"));

    newOneColorButton->setToolTip(tr("Color when the DXCC is an ATNO (All Time New One)."));
    neededColorButton->setToolTip(tr("This DXCC was worked before in another band but not in the selected band. It may be needed due to the CQ, ITU, Grid, ..."));
    workedColorButton->setToolTip(tr("Worked DXCC, but not confirmed in this band."));
    confirmedColorButton->setToolTip(tr("DXCC is confirmed in this band."));
    defaultColorButton->setToolTip(tr("Default color."));
    wsjtxColorButton->setToolTip(tr("Sets a palette of colors similar to the one used in WSJT-X."));
    klogColorButton->setToolTip(tr("Sets the default palette."));
    darkModeButton->setToolTip(tr("Sets the Dark Mode"));

    newOneColorButton->setAutoFillBackground ( true );

    QVBoxLayout *buttonsLayout = new QVBoxLayout;

    buttonsLayout->addWidget(newOneColorButton);
    buttonsLayout->addWidget(neededColorButton);
    buttonsLayout->addWidget(workedColorButton);
    buttonsLayout->addWidget(confirmedColorButton);
    buttonsLayout->addWidget(defaultColorButton);

    QHBoxLayout *schemasLayout = new QHBoxLayout;
    schemasLayout->setSpacing(40);
    schemasLayout->addWidget(wsjtxColorButton);
    schemasLayout->addWidget(klogColorButton);
    schemasLayout->addWidget(darkModeButton);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(buttonsLayout, 0, 0);
    mainLayout->addLayout(schemasLayout, 1, 0);
    //setLayout(buttonsLayout);
    setLayout(mainLayout);

    connect(newOneColorButton, SIGNAL(clicked()), this, SLOT(slotNewOneColorButtonClicked()) );
    connect(neededColorButton, SIGNAL(clicked()), this, SLOT(slotNeededColorButtonClicked()) );
    connect(workedColorButton, SIGNAL(clicked()), this, SLOT(slotWorkedColorButtonClicked()) );
    connect(confirmedColorButton, SIGNAL(clicked()), this, SLOT(slotConfirmedColorButtonClicked()) );
    connect(defaultColorButton, SIGNAL(clicked()), this, SLOT(slotDefaultColorButtonClicked()) );
    connect(wsjtxColorButton, SIGNAL(clicked()), this, SLOT(slotWSJTXButtonClicked()) );
    connect(klogColorButton, SIGNAL(clicked()), this, SLOT(slotKLogButtonClicked()) );
    connect(darkModeButton, SIGNAL(clicked()), this, SLOT(slotSetDarkMode()) );

    setDefaultColors();

       //qDebug() << "SetupPageColors::SetupPageColors - END";
}

SetupPageColors::~SetupPageColors()
{;
}

void SetupPageColors::setDefaultColors()
{
    setNewOneColor("#FF0000");
    setNeededColor("#FF8C00");
    setWorkedColor("#FFD700");
    setConfirmedColor("#32CD32");
    setDefaultColor("#00BFFF");
}

void SetupPageColors::setWSJTXColors()
{
    setNewOneColor("#FF00FF");      // New DXCC
    setNeededColor("#FFAAFF");      // New DXCC on Band
    setWorkedColor("#00BFFF");      // Like Default, WSJT-X does not make any difference
    setConfirmedColor("#00BFFF");   // Like Default, WSJT-X does not make any difference
    setDefaultColor("#00BFFF");
}

void SetupPageColors::slotNewOneColorButtonClicked()
{
       //qDebug()  << "SetupPageColors::slotNewOneColorButtonClicked ";
    QString style = "* { background-color: ";
    style = style + (giveColor(newOneColorButton->palette().color(QPalette::Button))).name();
    style = style + "; }";
    newOneColorButton->setStyleSheet(style);
}

void SetupPageColors::slotNeededColorButtonClicked ()
{
       //qDebug()  << "SetupPageColors::slotNeededColorButtonClicked ";
    QString style = "* { background-color: ";
    style = style + (giveColor(neededColorButton->palette().color(QPalette::Button))).name();
    style = style + "; }";
    neededColorButton->setStyleSheet(style);

}

void SetupPageColors::slotWorkedColorButtonClicked ()
{
       //qDebug()  << "SetupPageColors::slotWorkedColorButtonClicked ";
    QString style = "* { background-color: ";
    style = style + (giveColor(workedColorButton->palette().color(QPalette::Button))).name();
    style = style + "; }";
    workedColorButton->setStyleSheet(style);

}

void SetupPageColors::slotConfirmedColorButtonClicked ()
{
       //qDebug()  << "SetupPageColors::slotNeededColorButtonClicked ";
    QString style = "* { background-color: ";
    style = style + (giveColor(confirmedColorButton->palette().color(QPalette::Button))).name();
    style = style + "; }";
    confirmedColorButton->setStyleSheet(style);
}

void SetupPageColors::slotDefaultColorButtonClicked()
{
       //qDebug()  << "SetupPageColors::slotDefaultColorButtonClicked ";
    QString style = "* { background-color: ";
    style = style + (giveColor(defaultColorButton->palette().color(QPalette::Button))).name();
    style = style + "; }";
    defaultColorButton->setStyleSheet(style);
}

QColor SetupPageColors::giveColor (QColor c)
{
  // Receives the actual color, shows the user a color picker and returns the color that the user selects.
  QColor colorb;
  color = c;
  colorb = color;
  color = QColorDialog::getColor (color, this, tr("Choose a color"));
  if (color.isValid ())
    {
            //qDebug()  << "SetupPageColors::giveColor valid color: " << color.name();
      return color;
    }
  else
    {
           //qDebug()  << "SetupPageColors::giveColor NOT valid color";
      return colorb;
    }

}

QString SetupPageColors::getNewOneColor()
{
       //qDebug()  << "SetupPageColors::getNewOneColor: " << (newOneColorButton->palette().color(QPalette::Button)).name();
    return (newOneColorButton->palette().color(QPalette::Button)).name();
}

QString SetupPageColors::getNeededColor()
{
    return (neededColorButton->palette().color(QPalette::Button)).name();
}

QString SetupPageColors::getWorkedColor()
{
    return (workedColorButton->palette().color(QPalette::Button)).name();
}

QString SetupPageColors::getConfirmedColor()
{
    return (confirmedColorButton->palette().color(QPalette::Button)).name();
}

QString SetupPageColors::getDefaultColor()
{
    return (defaultColorButton->palette().color(QPalette::Button)).name();
}

void SetupPageColors::setNewOneColor(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    QString style = "* { background-color: ";
    style = style + _c;
    style = style + "; }";
    newOneColorButton->setStyleSheet(style);
}

void SetupPageColors::setNeededColor(const QString &_c)
{
    QString style = "* { background-color: ";
    style = style + _c;
    style = style + "; }";
    neededColorButton->setStyleSheet(style);
}

void SetupPageColors::setWorkedColor(const QString &_c)
{
    QString style = "* { background-color: ";
    style = style + _c;
    style = style + "; }";
    workedColorButton->setStyleSheet(style);
}

void SetupPageColors::setConfirmedColor(const QString &_c)
{
    QString style = "* { background-color: ";
    style = style + _c;
    style = style + "; }";
    confirmedColorButton->setStyleSheet(style);
}

void SetupPageColors::setDefaultColor(const QString &_c)
{
    QString style = "* { background-color: ";
    style = style + _c;
    style = style + "; }";
    defaultColorButton->setStyleSheet(style);
}

void SetupPageColors::slotWSJTXButtonClicked()
{
    setWSJTXColors();
}

void SetupPageColors::slotKLogButtonClicked()
{
    setDefaultColors();
}

void SetupPageColors::loadDarkMode()
{// Reads the config to setup the DarkMode
    qDebug() << Q_FUNC_INFO;
    QSettings settings(util->getSetFile (), QSettings::IniFormat);
    settings.beginGroup ("Colors");
    setDarkMode (settings.value("DarkMode", false).toBool ());
    settings.endGroup ();
}

void SetupPageColors::slotSetDarkMode()
{
    setDarkMode (!darkMode);
}

QString SetupPageColors::getDarkMode(){

    qDebug() << Q_FUNC_INFO;
    return util->boolToQString(darkMode);
}

void SetupPageColors::setDarkMode(const bool _d)
{
    qDebug() << Q_FUNC_INFO << ": " << util->boolToQString (_d);
    darkMode = _d;
    if (darkMode)
    {
        QApplication::setStyle(QStyleFactory::create("Fusion"));
        QPalette p;
        p = qApp->palette();
        p.setColor(QPalette::Window, QColor(53,53,53));
        p.setColor(QPalette::Button, QColor(53,53,53));
        p.setColor(QPalette::Highlight, QColor(142,45,197));
        p.setColor(QPalette::ButtonText, QColor(255,255,255));
        p.setColor(QPalette::WindowText, QColor(255,255,255));
        p.setColor(QPalette::Text, Qt::white);
        p.setColor(QPalette::Base, QColor(100,100,100));
        qApp->setPalette(p);
        darkModeButton->setText(tr("Light Mode"));
        //darkMode = true;
    }
    else
    {
        QApplication::setStyle(QStyleFactory::create("Fusion"));
        QPalette p;
        p = qApp->palette();
        p.setColor(QPalette::Window, QColor(244,246,246));
        p.setColor(QPalette::Text, Qt::black);
        p.setColor(QPalette::Button, QColor(234,237,237));
        p.setColor(QPalette::Highlight, QColor(40,120,240));
        p.setColor(QPalette::ButtonText, Qt::black);
        p.setColor(QPalette::WindowText, QColor(33,47,60));
        p.setColor(QPalette::Base, Qt::white);
        p.setColor(QPalette::ToolTipBase, Qt::white);
        p.setColor(QPalette::ToolTipText, Qt::black);
        qApp->setPalette(p);
        darkModeButton->setText(tr("Dark Mode"));
        //darkMode = false;
    }
}

void SetupPageColors::saveSettings()
{
    //qDebug() << Q_FUNC_INFO ;
    QSettings settings(util->getSetFile (), QSettings::IniFormat);
    settings.beginGroup ("Colors");
    //qDebug() << Q_FUNC_INFO << ": Saving NewOneColor: " << (newOneColorButton->palette().color(QPalette::Button)).name();
    settings.setValue ("NewOneColor", (newOneColorButton->palette().color(QPalette::Button)).name());
    settings.setValue ("NeededColor", (neededColorButton->palette().color(QPalette::Button)).name());
    settings.setValue ("WorkedColor", (workedColorButton->palette().color(QPalette::Button)).name());
    settings.setValue ("ConfirmedColor", (confirmedColorButton->palette().color(QPalette::Button)).name());
    settings.setValue ("DefaultColor", (defaultColorButton->palette().color(QPalette::Button)).name());
    settings.setValue ("DarkMode", QVariant(darkMode));
    settings.endGroup ();
}

void SetupPageColors::loadSettings()
{
    qDebug() << Q_FUNC_INFO;
    QSettings settings(util->getSetFile (), QSettings::IniFormat);
    settings.beginGroup ("Colors");

    setNewOneColor (settings.value("NewOneColor", "#FF0000").toString ());
    //settings.value("interval").toInt();
    setNeededColor (settings.value("NeededColor", "#FF8C00").toString ());
    setWorkedColor (settings.value("WorkedColor", "#FFD700").toString ());
    setConfirmedColor (settings.value("ConfirmedColor", "#32CD32").toString ());
    setDefaultColor (settings.value("DefaultColor", "#00BFFF").toString ());
    setDarkMode (settings.value("DarkMode", false).toBool ());
    settings.endGroup ();
}

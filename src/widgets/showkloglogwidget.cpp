/***************************************************************************
                          showkloglogwidget.cpp  -  description
                             -------------------
    begin                : jan 2021
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

#include "showkloglogwidget.h"

ShowKLogLogWidget::ShowKLogLogWidget(QWidget *parent) : QWidget(parent)
{
    util = new Utilities(Q_FUNC_INFO);
    levelComboBox = new QComboBox;
    model = new QStringListModel();
    //QStringList list;
    list.clear();

    model->setStringList(list);
    logsView = new QListView;
    logsView->setModel(model);
    createUI();
}

ShowKLogLogWidget::~ShowKLogLogWidget()
{
    delete(util);
}

void ShowKLogLogWidget::createUI()
{
    levelComboBox->clear();
    levelComboBox->addItems(util->getDebugLevels());
    levelComboBox->setCurrentIndex(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(levelComboBox);
    layout->addWidget(logsView);
    setLayout(layout);

    connect(levelComboBox, SIGNAL(currentTextChanged (QString)), this, SLOT(slotLevelComboBoxChanged(QString) ) ) ;
}


void ShowKLogLogWidget::add(const QString &_func, QString const &_log, const DebugLogLevel _l)
{
    //qDebug() << "Debugging0: " << _func << "/" << _log << "/" << util->debugLevelToString(logLevel) << "/" << util->debugLevelToString(_l);
    if (logLevel >_l)
    {
        return;
    }
    QString msg;

    msg = QString("%1 %2 - %3 - %4").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss")).arg(util->debugLevelToString (_l)).arg(_func).arg(_log);

    //qDebug() << "Debugging1: " << msg;

    if(model->insertRow(0)) {
        QModelIndex index = model->index(0, 0);
        model->setData(index, msg);
    }
    return;
    // FILE
    debugFile = new QFile(util->getDebugLogFile());
    if (!debugFile->open(QIODevice::Append | QIODevice::Text)) /* Flawfinder: ignore */
    {
        //qDebug() << Q_FUNC_INFO << " Can't open the file to log - EXITING";
        return;
    }

    QTextStream out(debugFile);
    out << (QDateTime::currentDateTime()).toString("yyyyMMdd-hhmmsszzz") << " - " << _func << " - " << msg;
    //qDebug() << "Debugging2: " << out.string();
    debugFile->close();
}

void ShowKLogLogWidget::setLogLevel(const DebugLogLevel _l)
{
    //qDebug() << Q_FUNC_INFO << " - New log Level: " << util->debugLevelToString(_l);
    QString logString = util->debugLevelToString(_l);

    if (util->isValidLogLevel(logString))
        levelComboBox->setCurrentIndex(levelComboBox->findText(logString, Qt::MatchCaseSensitive));
    else
    {
        levelComboBox->setCurrentIndex(0);
        logLevel = util->stringToDebugLevel("None");
        return;
    }
    logLevel = _l;
}

void ShowKLogLogWidget::slotLevelComboBoxChanged(const QString &_l)
{
    emit newLogLevel(util->stringToDebugLevel(_l));
}

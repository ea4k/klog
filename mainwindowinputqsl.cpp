/***************************************************************************
                          mainwindowinputqsl.cpp  -  description
                             -------------------
    begin                : jun 2017
    copyright            : (C) 2017 by Jaime Robles
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
//
// This class implement the tab of the mainwindow that supports the QSL options
//

#include "mainwindowinputqsl.h"

MainWindowInputQSL::MainWindowInputQSL(QWidget *parent) :
    QWidget(parent)
{
    util = new Utilities;
    qslSentComboBox = new QComboBox;
    qslRecComboBox = new QComboBox;
    qslSentViaComboBox = new QComboBox;
    qslRecViaComboBox = new QComboBox;

    qslSentQDateEdit = new QDateEdit;
    qslRecQDateEdit = new QDateEdit;
    qslViaLineEdit = new QLineEdit;
    qslmsgTextEdit = new QTextEdit;

    dataProxy = new DataProxy_SQLite();







    createUI();
    setDefaultData();
    clear();

}

MainWindowInputQSL::~MainWindowInputQSL(){}

void MainWindowInputQSL::createUI()
{
    // QSL Tab definition starts here

    qslSentQDateEdit->setDisplayFormat("dd/MM/yyyy");
    qslRecQDateEdit->setDisplayFormat("dd/MM/yyyy");

    QLabel *QSLSentLabelN = new QLabel(tr("QSL Sent"));
    QSLSentLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *QSLRecLabelN = new QLabel(tr("QSL Rec"));
    QSLRecLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *QSLViaLabelN = new QLabel(tr("QSL Via"));
    QSLViaLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *QSLMsgLabelN = new QLabel(tr("QSL Msg"));
    QSLMsgLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    qslSentComboBox->setToolTip(tr("Status of the QSL sending."));
    qslRecComboBox->setToolTip(tr("Status of the QSL reception."));
    qslSentViaComboBox->setToolTip(tr("QSL sending information."));
    qslRecViaComboBox->setToolTip(tr("QSL reception information."));

    qslSentQDateEdit->setToolTip(tr("Date of the QSL sending."));
    qslRecQDateEdit->setToolTip(tr("Date of the QSL reception."));
    qslmsgTextEdit->setToolTip(tr("Message of the QSL."));
    qslViaLineEdit->setToolTip(tr("QSL via information."));

    QGridLayout *QSLLayout = new QGridLayout;
    QSLLayout->addWidget(QSLSentLabelN, 0, 0);
    QSLLayout->addWidget(QSLRecLabelN, 1, 0);
    QSLLayout->addWidget(QSLViaLabelN, 2, 0);
    QSLLayout->addWidget(QSLMsgLabelN, 3, 0);

    QSLLayout->addWidget(qslSentComboBox, 0, 1);
    QSLLayout->addWidget(qslRecComboBox, 1, 1);
    QSLLayout->addWidget(qslViaLineEdit, 2, 1, 1, -1);
    QSLLayout->addWidget(qslmsgTextEdit, 3, 1, 1, -1);

    QSLLayout->addWidget(qslSentQDateEdit, 0, 2);
    QSLLayout->addWidget(qslRecQDateEdit, 1, 2);
    QSLLayout->addWidget(qslSentViaComboBox, 0, 3);
    QSLLayout->addWidget(qslRecViaComboBox, 1, 3);

    setLayout(QSLLayout);

    connect(qslViaLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQSLViaTextChanged() ) )  ;
    connect(qslRecComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotQSLRecvComboBoxChanged() ) )  ;
    connect(qslSentComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotQSLSentComboBoxChanged() ) )  ;
    connect(qslViaLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQSLViaLineEditReturnPressed() ) );

}


void MainWindowInputQSL::setDefaultData()
{    

    qslSentStatusList.clear();
    qslRcvdStatusList.clear();
    qslViaList.clear();

    qslSentStatusList = dataProxy->getQSLSentList();
    qslRcvdStatusList = dataProxy->getQSLRcvdList();
    qslViaList = dataProxy->getQSLViaList();

    qslSentComboBox->addItems(qslSentStatusList);
    qslRecComboBox->addItems(qslRcvdStatusList);
    qslRecViaComboBox->addItems(qslViaList);
    qslSentViaComboBox->addItems(qslViaList);

    qslSentQDateEdit->setDate(util->getDefaultDate());
    qslRecQDateEdit->setDate(util->getDefaultDate());


}
void MainWindowInputQSL::clear()
{
    qslSentComboBox->setCurrentIndex(1);
    qslRecComboBox->setCurrentIndex(1);
    qslRecViaComboBox->setCurrentIndex(0);
    qslSentViaComboBox->setCurrentIndex(0);

    qslSentQDateEdit->setDate(util->getDefaultDate());
    qslRecQDateEdit->setDate(util->getDefaultDate());

    qslmsgTextEdit->clear();
    qslViaLineEdit->clear();

}

void MainWindowInputQSL::qslViaClear()
{
    qslViaLineEdit->clear();
}

QString MainWindowInputQSL::getQSLRecStatus()
{
    QString _pm = QString();
    _pm = (((qslRecComboBox->currentText()).split('-')).at(0)).simplified();
     return _pm;
}

QString MainWindowInputQSL::getQSLSenStatus()
{
    QString _pm = QString();
    _pm = (((qslSentComboBox->currentText()).split('-')).at(0)).simplified();
     return _pm;
}

QString MainWindowInputQSL::getSentVia()
{
    QString _pm = QString();
    _pm = (((qslSentViaComboBox->currentText()).split('-')).at(0)).simplified();
    qDebug() << "MainWindow::getSentVia: " << _pm << endl;
     return _pm;
}

QString MainWindowInputQSL::getRecVia()
{
    QString _pm = QString();
    _pm = (((qslRecViaComboBox->currentText()).split('-')).at(0)).simplified();
    qDebug() << "MainWindowInputQSL::getRecVia: " << _pm << endl;
     return _pm;
}

QString MainWindowInputQSL::getQSLVia()
{
    return qslViaLineEdit->text();
}

QString MainWindowInputQSL::getQSLMsg()
{
    return qslmsgTextEdit->toPlainText();
}


void MainWindowInputQSL::setQSLRecStatus(const QString _qs)
{
    if(( qslRecComboBox->findText(_qs+" -", Qt::MatchStartsWith))>=0)
    {
        qslRecComboBox->setCurrentIndex( qslRecComboBox->findText(_qs+" -", Qt::MatchStartsWith));
    }
    else
    {
        qslRecComboBox->setCurrentIndex(1);
    }
}

void MainWindowInputQSL::setQSLSenStatus(const QString _qs)
{
    if(( qslSentComboBox->findText(_qs+" -", Qt::MatchStartsWith))>=0)
    {
        qslSentComboBox->setCurrentIndex( qslSentComboBox->findText(_qs+" -", Qt::MatchStartsWith));
    }
    else
    {
        qslSentComboBox->setCurrentIndex(1);
    }
}

void MainWindowInputQSL::setQSLRecVia(const QString _qs)
{
    if(( qslRecViaComboBox->findText(_qs+" -", Qt::MatchStartsWith))>=0)
    {
        qslRecViaComboBox->setCurrentIndex( qslRecViaComboBox->findText(_qs+" -", Qt::MatchStartsWith));
    }
    else
    {
        qslRecViaComboBox->setCurrentIndex(0);  // bureau by default
    }
}

void MainWindowInputQSL::setQSLSenVia(const QString _qs)
{
    qDebug() << "MainWindowInputQSL::setQSLSenVia: " << _qs << endl;
    if(( qslSentViaComboBox->findText(_qs+" -", Qt::MatchStartsWith))>=0)
    {
        qslSentViaComboBox->setCurrentIndex( qslSentViaComboBox->findText(_qs+" -", Qt::MatchStartsWith));
    }
    else
    {
        qslSentViaComboBox->setCurrentIndex(0); // bureau by default
    }
}

void MainWindowInputQSL::setQSLVia(const QString _qs, QColor qColor)
{

    palette.setColor(QPalette::Text, qColor);
    qslViaLineEdit->setPalette(palette);

    if (_qs.length()>0)
    {
        qslViaLineEdit->setText(_qs);
    }
    else
    {
        qslViaLineEdit->clear();
    }

}

void MainWindowInputQSL::setQSLMsg(const QString _qs)
{
    if (_qs.length()>0)
    {
        qslmsgTextEdit->setText(_qs);
    }
    else
    {
        qslmsgTextEdit->clear();
    }
}


QDate MainWindowInputQSL::getQSLRecDate()
{
    return qslRecQDateEdit->date();

}

QDate MainWindowInputQSL::getQSLSenDate()
{
    return qslSentQDateEdit->date();
}

void MainWindowInputQSL::setQSLRecDate(const QDate _qs)
{
    if (_qs.isValid())
    {
        qslRecQDateEdit->setDate(_qs);
    }
    else
    {
        qslRecQDateEdit->clear();
    }
}

void MainWindowInputQSL::setQSLSenDate(const QDate _qs)
{
    if (_qs.isValid())
    {
        qslSentQDateEdit->setDate(_qs);
    }
    else
    {
        qslSentQDateEdit->clear();
    }
}

void MainWindowInputQSL::slotQSLViaTextChanged()
{
    //qDebug() << "MainWindow::slotQSLViaTextChanged: " << qslViaLineEdit->text() << " / Length: " << QString::number((qslViaLineEdit->text()).size()) << endl;
    qslViaLineEdit->setText((qslViaLineEdit->text()).toUpper());
}


void MainWindowInputQSL::slotQSLSentComboBoxChanged(){

    int i = qslSentComboBox->currentIndex();
//{Y, N, R, I, V}
    //(QSLSDATE is only valid if QSL_SENT is Y-0, Q-3, or I-4)
    // Y-Yes = 0
    // N-No = 1
    // R-Requested = 2
    // Q-Queued = 3
    // I-Ignore = 4
    switch (i)
    {
        case 0:
            qslSentQDateEdit->setEnabled(true);
            qslSentQDateEdit->setDate((QDateTime::currentDateTime()).date());

        break;
        case 2:
        break;
        case 3:
            qslSentQDateEdit->setEnabled(true);
        break;
        case 4:
            qslSentQDateEdit->setEnabled(true);
        break;

        default: //NO
            qslSentQDateEdit->setEnabled(false);
        break;
    }
}


void MainWindowInputQSL::slotQSLRecvComboBoxChanged(){

    int i = qslRecComboBox->currentIndex();
//{Y, N, R, I, V}
    //(QSLSDATE is only valid if QSL_SENT is Y-0, Q-3, or I-4)
    // Y-Yes = 0
    // N-No = 1
    // R-Requested = 2
    // Q-Queued = 3
    // I-Ignore = 4
    switch (i)
    {
        case 0:
            qslRecQDateEdit->setEnabled(true);
            qslRecQDateEdit->setDate((QDateTime::currentDateTime()).date());

        break;
        case 2:
        break;
        case 3:
            qslRecQDateEdit->setEnabled(true);
        break;
        case 4:
            qslRecQDateEdit->setEnabled(true);
        break;

        default: //NO
            qslRecQDateEdit->setEnabled(false);
        break;
    }
}


void MainWindowInputQSL::slotQSLViaLineEditReturnPressed()
{
    emit returnPressed();
}

/***************************************************************************
                          mainwindowinputeqsl.cpp  -  description
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

#include "mainwindowinputeqsl.h"

MainWindowInputEQSL::MainWindowInputEQSL(DataProxy_SQLite *dp, QWidget *parent) :
    QWidget(parent)
{
       //qDebug() << "MainWindowInputEQSL::MainWindowInputEQSL"  ;
    util = new Utilities(Q_FUNC_INFO);
    dataProxy = dp;

    eqslSentComboBox = new QComboBox;
    eqslRecComboBox = new QComboBox;
    lotwSentComboBox = new QComboBox;
    lotwRecComboBox = new QComboBox;
    clublogComboBox = new QComboBox;
    qrzcomComboBox = new QComboBox;

    eqslSentQDateEdit = new QDateEdit;
    eqslRecQDateEdit = new QDateEdit;
    lotwSentQDateEdit = new QDateEdit;
    lotwRecQDateEdit = new QDateEdit;
    clublogQDateEdit = new QDateEdit;
    qrzcomQDateEdit = new QDateEdit;

    createUI();
    setDefaultData();
    clear();
       //qDebug() << "MainWindowInputEQSL::MainWindowInputEQSL - END"  ;
}

MainWindowInputEQSL::~MainWindowInputEQSL()
{
    delete(util);
    //delete(dataProxy);
}

QSO MainWindowInputEQSL::getQSOData(QSO _qso)
{
    qDebug() << Q_FUNC_INFO;
    QSO qso = _qso;
    qso.setClubLogStatus(getClubLogStatus());
    qso.setEQSLQSL_RCVD(getEQSLRecStatus());
    qso.setEQSLQSL_SENT(getEQSLSenStatus());
    qso.setLoTWQSL_RCVD(getLOTWRecStatus());
    qso.setLoTWQSL_SENT(getLOTWSenStatus());
    qso.setQRZCOMStatus(getQRZCOMStatus());

    qso.setClubLogDate(getClubLogDate());
    qso.setEQSLQSLRDate(getEQSLRecDate());
    qso.setEQSLQSLSDate(getEQSLSenDate());
    qso.setLoTWQSLRDate(getLOTWRecDate());
    qso.setLoTWQSLSDate(getLOTWSenDate());
    qso.setQRZCOMDate(getQRZCOMDate());

    return qso;
}

void MainWindowInputEQSL::setQSOData(const QSO &_qso)
{
    QSO qso(_qso);
    // Define first the status so the slots are not overwriting the dates or set uo a semaphore

    qDebug() << Q_FUNC_INFO << (_qso.getEQSLQSLSDate()).toString("yyyy-MM-dd");
    qDebug() << Q_FUNC_INFO << (qso.getEQSLQSLSDate()).toString("yyyy-MM-dd");
    setClubLogStatus(qso.getClubLogStatus());
    setClubLogDate(qso.getClubLogDate());

    setEQSLRecStatus(qso.getEQSLQSL_RCVD());
    setEQSLSenStatus(qso.getEQSLQSL_SENT());
    setEQSLRecDate(qso.getEQSLQSLRDate());
    setEQSLSenDate(qso.getEQSLQSLSDate());

    setLOTWRecStatus(qso.getLoTWQSL_RCVD());
    setLOTWSenStatus(qso.getLoTWQSL_SENT());
    setLOTWRecDate(qso.getLoTWQSLRDate());
    setLOTWSenDate(qso.getLoTWQSLSDate());

    setQRZCOMStatus(qso.getQRZCOMStatus());
    setQRZCOMDate(qso.getQRZCOMDate());    
}

void MainWindowInputEQSL::createUI()
{
    qDebug() << Q_FUNC_INFO << " - Start";
    qslSentStatusList.clear();
    qslRcvdStatusList.clear();
    clubLogStatusList.clear();
    qslSentStatusList = dataProxy->getQSLSentList();
    qslRcvdStatusList = dataProxy->getQSLRcvdList();
    clubLogStatusList = dataProxy->getClubLogStatusList();

    eqslSentQDateEdit->setDisplayFormat("dd/MM/yyyy");
    eqslRecQDateEdit->setDisplayFormat("dd/MM/yyyy");
    lotwSentQDateEdit->setDisplayFormat("dd/MM/yyyy");
    lotwRecQDateEdit->setDisplayFormat("dd/MM/yyyy");
    clublogQDateEdit->setDisplayFormat("dd/MM/yyyy");
    qrzcomQDateEdit->setDisplayFormat("dd/MM/yyyy");

    clublogQDateEdit->setToolTip(tr("Date of the ClubLog upload."));
    qrzcomQDateEdit->setToolTip(tr("Date of the QRZ.com upload."));
    eqslSentQDateEdit->setToolTip(tr("Date of the eQSL sending."));
    eqslRecQDateEdit->setToolTip(tr("Date of the eQSL reception."));
    lotwSentQDateEdit->setToolTip(tr("Date of the LoTW sending."));
    lotwRecQDateEdit->setToolTip(tr("Date of the LoTW reception."));

    clublogComboBox->setToolTip(tr("Status on ClubLog."));
    qrzcomComboBox->setToolTip(tr("Status on QRZ.com."));
    eqslSentComboBox->setToolTip(tr("Status of the eQSL sending."));
    eqslRecComboBox->setToolTip(tr("Status of the eQSL reception."));
    lotwSentComboBox->setToolTip(tr("Status of the LoTW sending."));
    lotwRecComboBox->setToolTip(tr("Status of the LoTW reception."));

    // eQSL Tab definition starts here

    QLabel *clublogLabelN = new QLabel(tr("ClubLog"));
    clublogLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *qrzcomLabelN = new QLabel(tr("QRZ.com"));
    qrzcomLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *eQSLSentLabelN = new QLabel(tr("eQSL Sent"));
    eQSLSentLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *eQSLRecLabelN = new QLabel(tr("eQSL Rec"));
    eQSLRecLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *lotWSentLabelN = new QLabel(tr("LoTW Sent"));
    lotWSentLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *lotWRecLabelN = new QLabel(tr("LoTW Rec"));
    lotWRecLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);


    QGridLayout *eqslInputTabWidgetLayout = new QGridLayout;

    eqslInputTabWidgetLayout->addWidget(clublogLabelN, 0, 0);
    eqslInputTabWidgetLayout->addWidget(eQSLSentLabelN, 1, 0);
    eqslInputTabWidgetLayout->addWidget(eQSLRecLabelN, 2, 0);
    eqslInputTabWidgetLayout->addWidget(lotWSentLabelN, 3, 0);
    eqslInputTabWidgetLayout->addWidget(lotWRecLabelN, 4, 0);
    eqslInputTabWidgetLayout->addWidget(qrzcomLabelN, 5, 0);

    eqslInputTabWidgetLayout->addWidget(clublogComboBox, 0, 1);
    eqslInputTabWidgetLayout->addWidget(eqslSentComboBox, 1, 1);
    eqslInputTabWidgetLayout->addWidget(eqslRecComboBox, 2, 1);
    eqslInputTabWidgetLayout->addWidget(lotwSentComboBox, 3, 1);
    eqslInputTabWidgetLayout->addWidget(lotwRecComboBox, 4, 1);
    eqslInputTabWidgetLayout->addWidget(qrzcomComboBox, 5, 1);

    eqslInputTabWidgetLayout->addWidget(clublogQDateEdit, 0, 2);
    eqslInputTabWidgetLayout->addWidget(eqslSentQDateEdit, 1, 2);
    eqslInputTabWidgetLayout->addWidget(eqslRecQDateEdit, 2, 2);
    eqslInputTabWidgetLayout->addWidget(lotwSentQDateEdit, 3, 2);
    eqslInputTabWidgetLayout->addWidget(lotwRecQDateEdit, 4, 2);
    eqslInputTabWidgetLayout->addWidget(qrzcomQDateEdit, 5, 2);
    //eqslInputTabWidgetLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(eqslInputTabWidgetLayout);

    connect(clublogComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotClubLogComboBoxChanged() ) )  ;
    connect(eqslSentComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sloteQSLSentComboBoxChanged() ) )  ;
    connect(eqslRecComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sloteQSLRecvComboBoxChanged() ) ) ;
    connect(lotwSentComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLotwSentComboBoxChanged() ) )  ;
    connect(lotwRecComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLotwRecvComboBoxChanged() ) ) ;
    connect(qrzcomComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotQRZCOMComboBoxChanged() ) )  ;
}

void MainWindowInputEQSL::setDefaultData()
{
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Validated");

    //eqslRecComboBox->addItems(qsAux);
    //lotwRecComboBox->addItems(qsAux);
    eqslRecComboBox->addItems(qslRcvdStatusList);
    lotwRecComboBox->addItems(qslRcvdStatusList);

    //qsAux.clear();
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
    //eqslSentComboBox->addItems(qsAux);
    //lotwSentComboBox->addItems(qsAux);
    eqslSentComboBox->addItems(qslSentStatusList);
    lotwSentComboBox->addItems(qslSentStatusList);

    //QStringList qsAux;
    //qsAux.clear();
    //qsAux << tr("Y-Uploaded") << tr("N-Do not upload") << tr("M-Modified");
    clublogComboBox->addItems(clubLogStatusList);
    qrzcomComboBox->addItems(clubLogStatusList);
    queueSentByDefault = true;
}

void MainWindowInputEQSL::clear()
{
    qDebug() << Q_FUNC_INFO << " - Start";
    // Do not upload
    if (queueSentByDefault)
    {
        clublogComboBox->setCurrentIndex( clublogComboBox->findText("M", Qt::MatchStartsWith));
        eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText("Q", Qt::MatchStartsWith));
        lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText("Q", Qt::MatchStartsWith));
        qrzcomComboBox->setCurrentIndex( qrzcomComboBox->findText("M", Qt::MatchStartsWith));
    }
    else
    {
        clublogComboBox->setCurrentIndex( clublogComboBox->findText("N", Qt::MatchStartsWith));
        qrzcomComboBox->setCurrentIndex( qrzcomComboBox->findText("N", Qt::MatchStartsWith));
        eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText("N", Qt::MatchStartsWith));
        lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText("N", Qt::MatchStartsWith));
    }

    eqslRecComboBox->setCurrentIndex(eqslRecComboBox->findText("N", Qt::MatchStartsWith));
    lotwRecComboBox->setCurrentIndex(lotwRecComboBox->findText("N", Qt::MatchStartsWith));

    eqslSentQDateEdit->setDate(util->getDefaultDate());
    eqslRecQDateEdit->setDate(util->getDefaultDate());
    lotwSentQDateEdit->setDate(util->getDefaultDate());
    lotwRecQDateEdit->setDate(util->getDefaultDate());
    clublogQDateEdit->setDate(util->getDefaultDate());
    qrzcomQDateEdit->setDate(util->getDefaultDate());
}


QString MainWindowInputEQSL::getClubLogStatus()
{
    QString _pm = QString();
   //qDebug() << Q_FUNC_INFO << clublogComboBox->currentText();
     _pm = (((clublogComboBox->currentText()).split('-')).at(0)).simplified();
   //qDebug() << Q_FUNC_INFO << " - From UI: " << _pm;
     return _pm;
}

QString MainWindowInputEQSL::getQRZCOMStatus()
{
    QString _pm = QString();
    _pm = (((qrzcomComboBox->currentText()).split('-')).at(0)).simplified();
    //qDebug() << Q_FUNC_INFO << " - From UI: " << _pm;
     return _pm;
}

QString MainWindowInputEQSL::getEQSLRecStatus()
{
   QString _pm = QString();
    //qDebug() << "MainWindowInputEQSL::getEQSLRecStatus:" << eqslRecComboBox->currentText();
    _pm = (((eqslRecComboBox->currentText()).split('-')).at(0)).simplified();
    //qDebug() << "MainWindowInputEQSL::getEQSLRecStatus: " << _pm;
    //if (_pm == "Not")
    //{
    //    return QString();
    //}
    return _pm;
}

QString MainWindowInputEQSL::getEQSLSenStatus()
{
    QString _pm = QString();
     //qDebug() << "MainWindowInputEQSL::getEQSLSenStatus:" << eqslSentComboBox->currentText();
     _pm = (((eqslSentComboBox->currentText()).split('-')).at(0)).simplified();
     //qDebug() << "MainWindowInputEQSL::getEQSLSenStatus: " << _pm;
     //if (_pm == "Not")
     //{
     //    return QString();
     //}
     return _pm;
}

QString MainWindowInputEQSL::getLOTWRecStatus()
{
    QString _pm = QString();
     //qDebug() << "MainWindowInputEQSL::getLOTWRecStatus:" << lotwRecComboBox->currentText();
     _pm = (((lotwRecComboBox->currentText()).split('-')).at(0)).simplified();
     //qDebug() << "MainWindowInputEQSL::getLOTWRecStatus: " << _pm;
     //if (_pm == "Not")
     //{
     //    return QString();
     //}
     return _pm;
}

QString MainWindowInputEQSL::getLOTWSenStatus()
{
    QString _pm = QString();
     //qDebug() << "MainWindowInputEQSL::getLOTWSenStatus:" << lotwSentComboBox->currentText();
     _pm = (((lotwSentComboBox->currentText()).split('-')).at(0)).simplified();
     //qDebug() << "MainWindowInputEQSL::getLOTWSenStatus: " << _pm;
     //if (_pm == "Not")
     //{
     //    return QString();
     //}
     return _pm;
}


void MainWindowInputEQSL::setClubLogStatus(const QString &_qs)
{
       //qDebug() << "MainWindowInputEQSL::setClubLogStatus: " << _qs;
    if((( clublogComboBox->findText(_qs, Qt::MatchStartsWith))>=0) && (_qs.length()>0)    )
     {
         clublogComboBox->setCurrentIndex( clublogComboBox->findText(_qs+" -", Qt::MatchStartsWith));
     }
     else
     {
         clublogComboBox->setCurrentIndex( clublogComboBox->findText("N", Qt::MatchStartsWith));
     }
}

void MainWindowInputEQSL::setQRZCOMStatus(const QString &_qs)
{
       //qDebug() << "MainWindowInputEQSL::setClubLogStatus: " << _qs;
    if((( qrzcomComboBox->findText(_qs, Qt::MatchStartsWith))>=0) && (_qs.length()>0)    )
     {
         qrzcomComboBox->setCurrentIndex( qrzcomComboBox->findText(_qs+" -", Qt::MatchStartsWith));
     }
     else
     {
         qrzcomComboBox->setCurrentIndex( qrzcomComboBox->findText("N", Qt::MatchStartsWith));
     }
}

void MainWindowInputEQSL::setEQSLRecStatus(const QString &_qs)
{
       //qDebug() << "MainWindowInputEQSL::setEQSLRecStatus: " << _qs;
     //if(( eqslRecComboBox->findText(_qs+" -", Qt::MatchStartsWith))>=0)
    if((( eqslRecComboBox->findText(_qs, Qt::MatchStartsWith))>=0) && (_qs.length()>0)    )
     {
         eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(_qs+" -", Qt::MatchStartsWith));
     }
     else
     {
         eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText("N", Qt::MatchStartsWith));
     }
}

void MainWindowInputEQSL::setEQSLSenStatus(const QString &_qs)
{
    //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: #" << _qs+" -";

     if((( eqslSentComboBox->findText(_qs, Qt::MatchStartsWith))>=0) && (_qs.length()>0)    )
     {
         //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: found: " << _qs << " - Index: " << QString::number(eqslSentComboBox->findText(_qs, Qt::MatchStartsWith));
         eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(_qs, Qt::MatchStartsWith));
     }
     else
     {
        //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: NOT found";
         if (queueSentByDefault)
         {
             //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: NOT found - Q is defined by default";
              eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText("Q", Qt::MatchStartsWith));
         }
         else
         {
             //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: NOT found - Q is NOT defined by default";
             eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText("N", Qt::MatchStartsWith));
             eqslSentComboBox->setCurrentIndex(1);
         }

        //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: NOT found";
         //eqslSentComboBox->setCurrentIndex(1);
     }
     //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus - END";
}

void MainWindowInputEQSL::setLOTWRecStatus(const QString &_qs)
{
      //qDebug() << "MainWindowInputEQSL::setLOTWRecStatus: " << _qs;
    if((( lotwRecComboBox->findText(_qs, Qt::MatchStartsWith))>=0) && (_qs.length()>0)    )
     //if(( lotwRecComboBox->findText(_qs+" -", Qt::MatchStartsWith))>=0)
     {
         lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(_qs+" -", Qt::MatchStartsWith));
     }
     else
     {
         lotwRecComboBox->setCurrentIndex(lotwRecComboBox->findText("N", Qt::MatchStartsWith));
     }
}


void MainWindowInputEQSL::setLOTWSenStatus(const QString &_qs)
{
      //qDebug() << "MainWindowInputEQSL::setLOTWSenStatus: " << _qs;
    if((( lotwSentComboBox->findText(_qs, Qt::MatchStartsWith))>=0) && (_qs.length()>0)    )
     //if(( lotwSentComboBox->findText(_qs+" -", Qt::MatchStartsWith))>=0)
     {
         lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText(_qs+" -", Qt::MatchStartsWith));
     }
     else
     {
         if (queueSentByDefault)
         {
              lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText("Q", Qt::MatchStartsWith));
         }
         else
         {
             lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText("N", Qt::MatchStartsWith));
         }

         //lotwSentComboBox->setCurrentIndex(1);
     }
}


void MainWindowInputEQSL::slotLotwRecvComboBoxChanged(){
       //qDebug() << "MainWindowInputEQSL::slotLotwRecvComboBoxChanged";

//QSLRDATE (only valid if QSL_RCVD is Y-0, I-3, or V-4)
//Y-Yes-0
//N-No-1
//R-Requested-2
//I-Ignore-3
//V-Verified-4
    int i = lotwRecComboBox->currentIndex();

    switch (i)
    {
        case 0:
            lotwRecQDateEdit->setVisible(true);
            lotwRecQDateEdit->setEnabled(true);
            lotwRecQDateEdit->setDate((QDateTime::currentDateTime()).date());

        break;
        case 2:
            lotwRecQDateEdit->setVisible(true);
            lotwRecQDateEdit->setEnabled(true);
            lotwRecQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 3:
            lotwRecQDateEdit->setVisible(true);
            lotwRecQDateEdit->setEnabled(true);
        break;
        case 4:
            lotwRecQDateEdit->setVisible(true);
            lotwRecQDateEdit->setEnabled(true);
        break;
        default: //NO
            //lotwRecQDateEdit->setVisible(false);
            lotwRecQDateEdit->setEnabled(false);
        break;
    }
}


void MainWindowInputEQSL::slotLotwSentComboBoxChanged(){
       //qDebug() << "MainWindowInputEQSL::slotLotwSentComboBoxChanged";

    int i = lotwSentComboBox->currentIndex();
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
            lotwSentQDateEdit->setVisible(true);
            lotwSentQDateEdit->setEnabled(true);
            lotwSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 2:
            lotwSentQDateEdit->setVisible(true);
            lotwSentQDateEdit->setEnabled(true);
            lotwSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 3:
            lotwSentQDateEdit->setVisible(true);
            lotwSentQDateEdit->setEnabled(true);
            lotwSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 4:
            lotwSentQDateEdit->setVisible(true);
            lotwSentQDateEdit->setEnabled(true);
            lotwSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;

        default: //NO
            //lotwSentQDateEdit->setVisible(false);
            lotwSentQDateEdit->setEnabled(false);
        break;
    }
}


void MainWindowInputEQSL::sloteQSLRecvComboBoxChanged(){
       //qDebug() << "MainWindowInputEQSL::sloteQSLRecvComboBoxChanged";

//QSLRDATE (only valid if QSL_RCVD is Y-0, I-3, or V-4)
//Y-Yes-0
//N-No-1
//R-Requested-2
//I-Ignore-3
//V-Verified-4

    int i = eqslRecComboBox->currentIndex();

    switch (i)
    {
        case 0:
            eqslRecQDateEdit->setVisible(true);
            eqslRecQDateEdit->setEnabled(true);
            eqslRecQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 2:
            eqslRecQDateEdit->setVisible(true);
            eqslRecQDateEdit->setEnabled(true);
            eqslRecQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 3:
            eqslRecQDateEdit->setVisible(true);
            eqslRecQDateEdit->setEnabled(true);
            eqslRecQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 4:
            eqslRecQDateEdit->setVisible(true);
            eqslRecQDateEdit->setEnabled(true);
            eqslRecQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        default: //NO
            //eqslRecQDateEdit->setVisible(false);
            eqslRecQDateEdit->setEnabled(false);
        break;
    }
}


void MainWindowInputEQSL::sloteQSLSentComboBoxChanged()
{
    qDebug() << Q_FUNC_INFO << " - Start";

    int i = eqslSentComboBox->currentIndex();
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
            eqslSentQDateEdit->setVisible(true);
            eqslSentQDateEdit->setEnabled(true);
            eqslSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 2:
            eqslSentQDateEdit->setVisible(true);
            eqslSentQDateEdit->setEnabled(true);
            eqslSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 3:
            eqslSentQDateEdit->setVisible(true);
            eqslSentQDateEdit->setEnabled(true);
            eqslSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 4:
            eqslSentQDateEdit->setVisible(true);
            eqslSentQDateEdit->setEnabled(true);
            eqslSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;

        default: //NO
            //eqslSentQDateEdit->setVisible(false);
            eqslSentQDateEdit->setEnabled(false);
        break;
    }
}

void MainWindowInputEQSL::slotClubLogComboBoxChanged()
{
    int i = clublogComboBox->currentIndex();
    //{Y, N, M}
    // Y-Yes = 0
    // N-No = 1
    // M-Modified = 2

    switch (i)
    {
        case 0:
            clublogQDateEdit->setVisible(true);
            clublogQDateEdit->setEnabled(true);
            clublogQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 2:
            clublogQDateEdit->setVisible(true);
            clublogQDateEdit->setEnabled(true);
            clublogQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        default: //NO
            //clublogQDateEdit->setVisible(false);
            clublogQDateEdit->setEnabled(false);
        break;
    }
}

void MainWindowInputEQSL::setClubLogDate(const QDate _qs)
{
   //qDebug() << Q_FUNC_INFO << "Date: " << _qs.toString("dd/MM/yyyy");
    if (_qs.isValid())
    {
        clublogQDateEdit->setDate(_qs);
    }
    else
    {
        clublogQDateEdit->setDate(QDate::currentDate());
    }
}

void MainWindowInputEQSL::slotQRZCOMComboBoxChanged()
{
    int i = qrzcomComboBox->currentIndex();
    //{Y, N, M}
    // Y-Yes = 0
    // N-No = 1
    // M-Modified = 2

    switch (i)
    {
        case 0:
            qrzcomQDateEdit->setVisible(true);
            qrzcomQDateEdit->setEnabled(true);
            qrzcomQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 2:
            qrzcomQDateEdit->setVisible(true);
            qrzcomQDateEdit->setEnabled(true);
            qrzcomQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        default: //NO
            //qrzcomQDateEdit->setVisible(false);
            qrzcomQDateEdit->setEnabled(false);
        break;
    }
}

void MainWindowInputEQSL::setQRZCOMDate(const QDate _qs)
{
    if (_qs.isValid())
    {
        qrzcomQDateEdit->setDate(_qs);
    }
    else
    {
        qrzcomQDateEdit->setDate(QDate::currentDate());
    }
}

void MainWindowInputEQSL::setEQSLRecDate(const QDate _qs)
{
    if (_qs.isValid())
    {
        eqslRecQDateEdit->setDate(_qs);
    }
    else
    {
        eqslRecQDateEdit->setDate(QDate::currentDate());
    }
}

void MainWindowInputEQSL::setEQSLSenDate(const QDate _qs)
{
    qDebug() << Q_FUNC_INFO << (_qs).toString("yyyy-MM-dd");
    if (_qs.isValid())
    {
        qDebug() << Q_FUNC_INFO << " - Date valid";
        eqslSentQDateEdit->setDate(_qs);
    }
    else
    {
        qDebug() << Q_FUNC_INFO << " - Date not valid";
        eqslSentQDateEdit->setDate(QDate::currentDate());
    }
}

void MainWindowInputEQSL::setLOTWRecDate(const QDate _qs)
{
    if (_qs.isValid())
    {
        lotwRecQDateEdit->setDate(_qs);
    }
    else
    {
        lotwRecQDateEdit->setDate(QDate::currentDate());
    }
}

void MainWindowInputEQSL::setLOTWSenDate(const QDate _qs)
{
    if (_qs.isValid())
    {
        lotwSentQDateEdit->setDate(_qs);
    }
    else
    {
        lotwSentQDateEdit->setDate(QDate::currentDate());
    }
}

QDate MainWindowInputEQSL::getClubLogDate()
{
    return clublogQDateEdit->date();
}

QDate MainWindowInputEQSL::getQRZCOMDate()
{
    return qrzcomQDateEdit->date();
}

QDate MainWindowInputEQSL::getEQSLRecDate()
{
    return eqslRecQDateEdit->date();
}

QDate MainWindowInputEQSL::getEQSLSenDate()
{
    return eqslSentQDateEdit->date();
}

QDate MainWindowInputEQSL::getLOTWRecDate()
{
    return   lotwRecQDateEdit->date();
}

QDate MainWindowInputEQSL::getLOTWSenDate()
{
    return lotwSentQDateEdit->date();
}

void MainWindowInputEQSL::setQueueSentByDefault(const bool _b)
{
    queueSentByDefault = _b;
}

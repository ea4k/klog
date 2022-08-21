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
       //qDebug() << "MainWindowInputEQSL::MainWindowInputEQSL"   << QT_ENDL;
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
       //qDebug() << "MainWindowInputEQSL::MainWindowInputEQSL - END"   << QT_ENDL;
}

MainWindowInputEQSL::~MainWindowInputEQSL()
{
    delete(util);
    delete(dataProxy);
}

void MainWindowInputEQSL::createUI()
{
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
    isQRZSubscriber = false;

}
void MainWindowInputEQSL::clear()
{
      //qDebug() << "MainWindowInputEQSL::clear"  << QT_ENDL;
     // Do not upload
    if (queueSentByDefault)
    {
        clublogComboBox->setCurrentIndex( clublogComboBox->findText("M", Qt::MatchStartsWith));
        eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText("Q", Qt::MatchStartsWith));
        lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText("Q", Qt::MatchStartsWith));
    }
    else
    {
        clublogComboBox->setCurrentIndex( clublogComboBox->findText("N", Qt::MatchStartsWith));
        eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText("N", Qt::MatchStartsWith));
        lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText("N", Qt::MatchStartsWith));
    }

    if ((isQRZSubscriber) && (queueSentByDefault))
    {
        qrzcomComboBox->setCurrentIndex( qrzcomComboBox->findText("M", Qt::MatchStartsWith));
    }
    else
    {
        qrzcomComboBox->setCurrentIndex( qrzcomComboBox->findText("N", Qt::MatchStartsWith));
    }

    eqslRecComboBox->setCurrentIndex(eqslRecComboBox->findText("N", Qt::MatchStartsWith));
    eqslRecQDateEdit->setSpecialValueText( " " );
    eqslRecQDateEdit->setDate( QDate::fromString( "01/01/0001", "dd/MM/yyyy" ) );
    lotwRecComboBox->setCurrentIndex(lotwRecComboBox->findText("N", Qt::MatchStartsWith));
    lotwRecQDateEdit->setSpecialValueText( " " );
    lotwRecQDateEdit->setDate( QDate::fromString( "01/01/0001", "dd/MM/yyyy" ) );

//    eqslSentQDateEdit->setDate(util->getDefaultDate());
//    eqslRecQDateEdit->setDate(util->getDefaultDate());
//    lotwSentQDateEdit->setDate(util->getDefaultDate());
//    lotwRecQDateEdit->setDate(util->getDefaultDate());
//    clublogQDateEdit->setDate(util->getDefaultDate());
//    qrzcomQDateEdit->setDate(util->getDefaultDate());

}


QString MainWindowInputEQSL::getClubLogStatus()
{
    QString _pm = QString();
    //qDebug() << "MainWindowInputEQSL::getClubLogStatus:" << clublogComboBox->currentText() << QT_ENDL;
     _pm = (((clublogComboBox->currentText()).split('-')).at(0)).simplified();
     return _pm;
}

QString MainWindowInputEQSL::getQRZCOMStatus()
{
    QString _pm = QString();
     _pm = (((qrzcomComboBox->currentText()).split('-')).at(0)).simplified();
     return _pm;
}

QString MainWindowInputEQSL::getEQSLRecStatus()
{
   QString _pm = QString();
    //qDebug() << "MainWindowInputEQSL::getEQSLRecStatus:" << eqslRecComboBox->currentText() << QT_ENDL;
    _pm = (((eqslRecComboBox->currentText()).split('-')).at(0)).simplified();
    //qDebug() << "MainWindowInputEQSL::getEQSLRecStatus: " << _pm << QT_ENDL;
    //if (_pm == "Not")
    //{
    //    return QString();
    //}
    return _pm;

}

QString MainWindowInputEQSL::getEQSLSenStatus()
{
    QString _pm = QString();
     //qDebug() << "MainWindowInputEQSL::getEQSLSenStatus:" << eqslSentComboBox->currentText() << QT_ENDL;
     _pm = (((eqslSentComboBox->currentText()).split('-')).at(0)).simplified();
     //qDebug() << "MainWindowInputEQSL::getEQSLSenStatus: " << _pm << QT_ENDL;
     //if (_pm == "Not")
     //{
     //    return QString();
     //}
     return _pm;
}

QString MainWindowInputEQSL::getLOTWRecStatus()
{
    QString _pm = QString();
     //qDebug() << "MainWindowInputEQSL::getLOTWRecStatus:" << lotwRecComboBox->currentText() << QT_ENDL;
     _pm = (((lotwRecComboBox->currentText()).split('-')).at(0)).simplified();
     //qDebug() << "MainWindowInputEQSL::getLOTWRecStatus: " << _pm << QT_ENDL;
     //if (_pm == "Not")
     //{
     //    return QString();
     //}
     return _pm;
}

QString MainWindowInputEQSL::getLOTWSenStatus()
{
    QString _pm = QString();
     //qDebug() << "MainWindowInputEQSL::getLOTWSenStatus:" << lotwSentComboBox->currentText() << QT_ENDL;
     _pm = (((lotwSentComboBox->currentText()).split('-')).at(0)).simplified();
     //qDebug() << "MainWindowInputEQSL::getLOTWSenStatus: " << _pm << QT_ENDL;
     //if (_pm == "Not")
     //{
     //    return QString();
     //}
     return _pm;
}


void MainWindowInputEQSL::setClubLogStatus(const QString &_qs)
{
       //qDebug() << "MainWindowInputEQSL::setClubLogStatus: " << _qs << QT_ENDL;
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
       //qDebug() << "MainWindowInputEQSL::setClubLogStatus: " << _qs << QT_ENDL;
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
       //qDebug() << "MainWindowInputEQSL::setEQSLRecStatus: " << _qs << QT_ENDL;
     //if(( eqslRecComboBox->findText(_qs+" -", Qt::MatchStartsWith))>=0)
    if((( eqslRecComboBox->findText(_qs, Qt::MatchStartsWith))>=0) && (_qs.length()>0)    )
     {
         eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(_qs+" -", Qt::MatchStartsWith));
     }
     else
     {
         eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText("N", Qt::MatchStartsWith));
         eqslRecQDateEdit->setSpecialValueText( " " );
         eqslRecQDateEdit->setDate( QDate::fromString( "01/01/0001", "dd/MM/yyyy" ) );
     }
}

void MainWindowInputEQSL::setEQSLSenStatus(const QString &_qs)
{
    //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: #" << _qs+" -" << QT_ENDL;

     if((( eqslSentComboBox->findText(_qs, Qt::MatchStartsWith))>=0) && (_qs.length()>0)    )
     {
         //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: found: " << _qs << " - Index: " << QString::number(eqslSentComboBox->findText(_qs, Qt::MatchStartsWith)) << QT_ENDL;
         eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(_qs, Qt::MatchStartsWith));
     }
     else
     {
        //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: NOT found" << QT_ENDL;
         if (queueSentByDefault)
         {
             //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: NOT found - Q is defined by default" << QT_ENDL;
              eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText("Q", Qt::MatchStartsWith));
         }
         else
         {
             //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: NOT found - Q is NOT defined by default" << QT_ENDL;
             eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText("N", Qt::MatchStartsWith));
             eqslSentComboBox->setCurrentIndex(1);
         }

        //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: NOT found" << QT_ENDL;
         //eqslSentComboBox->setCurrentIndex(1);
     }
     //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus - END" << QT_ENDL;
}

void MainWindowInputEQSL::setLOTWRecStatus(const QString &_qs)
{
      //qDebug() << "MainWindowInputEQSL::setLOTWRecStatus: " << _qs << QT_ENDL;
    if((( lotwRecComboBox->findText(_qs, Qt::MatchStartsWith))>=0) && (_qs.length()>0)    )
     //if(( lotwRecComboBox->findText(_qs+" -", Qt::MatchStartsWith))>=0)
     {
         lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(_qs+" -", Qt::MatchStartsWith));
     }
     else
     {
         lotwRecComboBox->setCurrentIndex(lotwRecComboBox->findText("N", Qt::MatchStartsWith));
         lotwRecQDateEdit->setSpecialValueText( " " );
         lotwRecQDateEdit->setDate( QDate::fromString( "01/01/0001", "dd/MM/yyyy" ) );
     }
}


void MainWindowInputEQSL::setLOTWSenStatus(const QString &_qs)
{
      //qDebug() << "MainWindowInputEQSL::setLOTWSenStatus: " << _qs << QT_ENDL;
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
       //qDebug() << "MainWindowInputEQSL::slotLotwRecvComboBoxChanged" << QT_ENDL;

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
            lotwRecQDateEdit->setEnabled(true);
            lotwRecQDateEdit->setDate((QDateTime::currentDateTime()).date());

        break;
        case 2:
            lotwRecQDateEdit->setEnabled(true);
            lotwRecQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 3:
            lotwRecQDateEdit->setEnabled(true);
        break;
        case 4:
            lotwRecQDateEdit->setEnabled(true);
        break;
        default: //NO
            lotwRecQDateEdit->setSpecialValueText( " " );
            lotwRecQDateEdit->setDate( QDate::fromString( "01/01/0001", "dd/MM/yyyy" ) );
            lotwRecQDateEdit->setEnabled(false);
        break;
    }

}


void MainWindowInputEQSL::slotLotwSentComboBoxChanged(){
       //qDebug() << "MainWindowInputEQSL::slotLotwSentComboBoxChanged" << QT_ENDL;

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
            lotwSentQDateEdit->setEnabled(true);
            lotwSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 2:
            lotwSentQDateEdit->setEnabled(true);
            lotwSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 3:
            lotwSentQDateEdit->setEnabled(true);
            lotwSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 4:
            lotwSentQDateEdit->setEnabled(true);
            lotwSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;

        default: //NO
            lotwSentQDateEdit->setSpecialValueText( " " );
            lotwSentQDateEdit->setDate( QDate::fromString( "01/01/0001", "dd/MM/yyyy" ) );
            lotwSentQDateEdit->setEnabled(false);
        break;
    }
}


void MainWindowInputEQSL::sloteQSLRecvComboBoxChanged(){
       //qDebug() << "MainWindowInputEQSL::sloteQSLRecvComboBoxChanged" << QT_ENDL;

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
            eqslRecQDateEdit->setEnabled(true);
            eqslRecQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 2:
            eqslRecQDateEdit->setEnabled(true);
            eqslRecQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 3:
            eqslRecQDateEdit->setEnabled(true);
            eqslRecQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 4:
            eqslRecQDateEdit->setEnabled(true);
            eqslRecQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        default: //NO
            eqslRecQDateEdit->setSpecialValueText( " " );
            eqslRecQDateEdit->setDate( QDate::fromString( "01/01/0001", "dd/MM/yyyy" ) );
            eqslRecQDateEdit->setEnabled(false);
        break;


    }

}


void MainWindowInputEQSL::sloteQSLSentComboBoxChanged(){
       //qDebug() << "MainWindowInputEQSL::sloteQSLSentComboBoxChanged" << QT_ENDL;

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
            eqslSentQDateEdit->setEnabled(true);
            eqslSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 2:
            eqslSentQDateEdit->setEnabled(true);
            eqslSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 3:
            eqslSentQDateEdit->setEnabled(true);
            eqslSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 4:
            eqslSentQDateEdit->setEnabled(true);
            eqslSentQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;

        default: //NO
            eqslSentQDateEdit->setSpecialValueText( " " );
            eqslSentQDateEdit->setDate( QDate::fromString( "01/01/0001", "dd/MM/yyyy" ) );
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
            clublogQDateEdit->setEnabled(true);
            clublogQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 2:
            clublogQDateEdit->setEnabled(true);
            clublogQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        default: //NO
            clublogQDateEdit->setSpecialValueText( " " );
            clublogQDateEdit->setDate( QDate::fromString( "01/01/0001", "dd/MM/yyyy" ) );
            clublogQDateEdit->setEnabled(false);
        break;
    }
}

void MainWindowInputEQSL::setClubLogDate(const QDate _qs)
{
    if (_qs.isValid())
    {
        clublogQDateEdit->setDate(_qs);
    }
    else
    {
        clublogQDateEdit->clear();
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
            qrzcomQDateEdit->setEnabled(true);
            qrzcomQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        case 2:
            qrzcomQDateEdit->setEnabled(true);
            qrzcomQDateEdit->setDate((QDateTime::currentDateTime()).date());
        break;
        default: //NO
            qrzcomQDateEdit->setSpecialValueText( " " );
            qrzcomQDateEdit->setDate( QDate::fromString( "01/01/0001", "dd/MM/yyyy" ) );
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
        qrzcomQDateEdit->clear();
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
        eqslRecQDateEdit->clear();
    }
}

void MainWindowInputEQSL::setEQSLSenDate(const QDate _qs)
{
    if (_qs.isValid())
    {
        eqslSentQDateEdit->setDate(_qs);
    }
    else
    {
        eqslSentQDateEdit->clear();
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
        lotwRecQDateEdit->clear();
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
        lotwSentQDateEdit->clear();
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

void MainWindowInputEQSL::setSubscriber(const bool _b)
{
    isQRZSubscriber = _b;
}

//void MainWindowInputEQSL::clearDateEdit(QDateEdit &_c)
//{
//    _c.setSpecialValueText( " " );
//    _c.setDate( QDate::fromString( "01/01/0001", "dd/MM/yyyy" ) );
//}

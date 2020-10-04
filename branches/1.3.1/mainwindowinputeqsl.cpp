#include "mainwindowinputeqsl.h"

MainWindowInputEQSL::MainWindowInputEQSL(DataProxy_SQLite *dp, QWidget *parent) :
    QWidget(parent)
{
       //qDebug() << "MainWindowInputEQSL::MainWindowInputEQSL"   << endl;
    util = new Utilities;
    dataProxy = dp;

    eqslSentComboBox = new QComboBox;
    eqslRecComboBox = new QComboBox;
    lotwSentComboBox = new QComboBox;
    lotwRecComboBox = new QComboBox;
    clublogComboBox = new QComboBox;

    eqslSentQDateEdit = new QDateEdit;
    eqslRecQDateEdit = new QDateEdit;
    lotwSentQDateEdit = new QDateEdit;
    lotwRecQDateEdit = new QDateEdit;
    clublogQDateEdit = new QDateEdit;

    createUI();
    setDefaultData();
    clear();
       //qDebug() << "MainWindowInputEQSL::MainWindowInputEQSL - END"   << endl;
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

    clublogQDateEdit->setToolTip(tr("Date of the ClubLog upload."));
    eqslSentQDateEdit->setToolTip(tr("Date of the eQSL sending."));
    eqslRecQDateEdit->setToolTip(tr("Date of the eQSL reception."));
    lotwSentQDateEdit->setToolTip(tr("Date of the LoTW sending."));
    lotwRecQDateEdit->setToolTip(tr("Date of the LoTW reception."));

    clublogComboBox->setToolTip(tr("Status on ClubLog."));
    eqslSentComboBox->setToolTip(tr("Status of the eQSL sending."));
    eqslRecComboBox->setToolTip(tr("Status of the eQSL reception."));
    lotwSentComboBox->setToolTip(tr("Status of the LoTW sending."));
    lotwRecComboBox->setToolTip(tr("Status of the LoTW reception."));

    // eQSL Tab definition starts here

    QLabel *clublogLabelN = new QLabel(tr("ClubLog"));
    clublogLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

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

    eqslInputTabWidgetLayout->addWidget(clublogComboBox, 0, 1);
    eqslInputTabWidgetLayout->addWidget(eqslSentComboBox, 1, 1);
    eqslInputTabWidgetLayout->addWidget(eqslRecComboBox, 2, 1);
    eqslInputTabWidgetLayout->addWidget(lotwSentComboBox, 3, 1);
    eqslInputTabWidgetLayout->addWidget(lotwRecComboBox, 4, 1);

    eqslInputTabWidgetLayout->addWidget(clublogQDateEdit, 0, 2);
    eqslInputTabWidgetLayout->addWidget(eqslSentQDateEdit, 1, 2);
    eqslInputTabWidgetLayout->addWidget(eqslRecQDateEdit, 2, 2);
    eqslInputTabWidgetLayout->addWidget(lotwSentQDateEdit, 3, 2);
    eqslInputTabWidgetLayout->addWidget(lotwRecQDateEdit, 4, 2);

    setLayout(eqslInputTabWidgetLayout);

    connect(clublogComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotClubLogComboBoxChanged() ) )  ;
    connect(eqslSentComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(sloteQSLSentComboBoxChanged() ) )  ;
    connect(eqslRecComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(sloteQSLRecvComboBoxChanged() ) ) ;
    connect(lotwSentComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotLotwSentComboBoxChanged() ) )  ;
    connect(lotwRecComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotLotwRecvComboBoxChanged() ) ) ;

    //dxUpLeftTab->addTab(eqslInputTabWidget, tr("eQSL"));
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

    queueSentByDefault = true;

}
void MainWindowInputEQSL::clear()
{
      //qDebug() << "MainWindowInputEQSL::clear"  << endl;
    clublogComboBox->setCurrentIndex(1); // Do not upload
    if (queueSentByDefault)
    {
         eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText("Q", Qt::MatchStartsWith));
         lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText("Q", Qt::MatchStartsWith));
    }
    else
    {
        eqslSentComboBox->setCurrentIndex(1);
        lotwSentComboBox->setCurrentIndex(1);
    }

    eqslRecComboBox->setCurrentIndex(1);    
    lotwRecComboBox->setCurrentIndex(1);

    eqslSentQDateEdit->setDate(util->getDefaultDate());
    eqslRecQDateEdit->setDate(util->getDefaultDate());
    lotwSentQDateEdit->setDate(util->getDefaultDate());
    lotwRecQDateEdit->setDate(util->getDefaultDate());
    clublogQDateEdit->setDate(util->getDefaultDate());

}


QString MainWindowInputEQSL::getClubLogStatus()
{
    QString _pm = QString();
     //qDebug() << "MainWindowInputEQSL::getClubLogStatus:" << clublogComboBox->currentText() << endl;
     _pm = (((clublogComboBox->currentText()).split('-')).at(0)).simplified();
     //qDebug() << "MainWindowInputEQSL::getClubLogStatus: " << _pm << endl;
     //if (_pm == "Not")
     //{
     //    return QString();
     //}
     return _pm;
}

QString MainWindowInputEQSL::getEQSLRecStatus()
{
   QString _pm = QString();
    //qDebug() << "MainWindowInputEQSL::getEQSLRecStatus:" << eqslRecComboBox->currentText() << endl;
    _pm = (((eqslRecComboBox->currentText()).split('-')).at(0)).simplified();
    //qDebug() << "MainWindowInputEQSL::getEQSLRecStatus: " << _pm << endl;
    //if (_pm == "Not")
    //{
    //    return QString();
    //}
    return _pm;

}

QString MainWindowInputEQSL::getEQSLSenStatus()
{
    QString _pm = QString();
     //qDebug() << "MainWindowInputEQSL::getEQSLSenStatus:" << eqslSentComboBox->currentText() << endl;
     _pm = (((eqslSentComboBox->currentText()).split('-')).at(0)).simplified();
     //qDebug() << "MainWindowInputEQSL::getEQSLSenStatus: " << _pm << endl;
     //if (_pm == "Not")
     //{
     //    return QString();
     //}
     return _pm;
}

QString MainWindowInputEQSL::getLOTWRecStatus()
{
    QString _pm = QString();
     //qDebug() << "MainWindowInputEQSL::getLOTWRecStatus:" << lotwRecComboBox->currentText() << endl;
     _pm = (((lotwRecComboBox->currentText()).split('-')).at(0)).simplified();
     //qDebug() << "MainWindowInputEQSL::getLOTWRecStatus: " << _pm << endl;
     //if (_pm == "Not")
     //{
     //    return QString();
     //}
     return _pm;
}

QString MainWindowInputEQSL::getLOTWSenStatus()
{
    QString _pm = QString();
     //qDebug() << "MainWindowInputEQSL::getLOTWSenStatus:" << lotwSentComboBox->currentText() << endl;
     _pm = (((lotwSentComboBox->currentText()).split('-')).at(0)).simplified();
     //qDebug() << "MainWindowInputEQSL::getLOTWSenStatus: " << _pm << endl;
     //if (_pm == "Not")
     //{
     //    return QString();
     //}
     return _pm;
}


void MainWindowInputEQSL::setClubLogStatus(const QString _qs)
{
       //qDebug() << "MainWindowInputEQSL::setClubLogStatus: " << _qs << endl;
    if((( clublogComboBox->findText(_qs, Qt::MatchStartsWith))>=0) && (_qs.length()>0)    )
     //if(( clublogComboBox->findText(_qs+" -", Qt::MatchStartsWith))>=0)

     {
         clublogComboBox->setCurrentIndex( clublogComboBox->findText(_qs+" -", Qt::MatchStartsWith));
     }
     else
     {
         clublogComboBox->setCurrentIndex( clublogComboBox->findText("N", Qt::MatchStartsWith));
     }

}

void MainWindowInputEQSL::setEQSLRecStatus(const QString _qs)
{
       //qDebug() << "MainWindowInputEQSL::setEQSLRecStatus: " << _qs << endl;
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

void MainWindowInputEQSL::setEQSLSenStatus(const QString _qs)
{
    //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: #" << _qs+" -" << endl;

     if((( eqslSentComboBox->findText(_qs, Qt::MatchStartsWith))>=0) && (_qs.length()>0)    )
     {
         //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: found: " << _qs << " - Index: " << QString::number(eqslSentComboBox->findText(_qs, Qt::MatchStartsWith)) << endl;
         eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(_qs, Qt::MatchStartsWith));
     }
     else
     {
        //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: NOT found" << endl;
         if (queueSentByDefault)
         {
             //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: NOT found - Q is defined by default" << endl;
              eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText("Q", Qt::MatchStartsWith));
         }
         else
         {
             //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: NOT found - Q is NOT defined by default" << endl;
             eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText("N", Qt::MatchStartsWith));
             eqslSentComboBox->setCurrentIndex(1);
         }

        //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus: NOT found" << endl;
         //eqslSentComboBox->setCurrentIndex(1);
     }
     //qDebug() << "MainWindowInputEQSL::setEQSLSenStatus - END" << endl;
}

void MainWindowInputEQSL::setLOTWRecStatus(const QString _qs)
{
      //qDebug() << "MainWindowInputEQSL::setLOTWRecStatus: " << _qs << endl;
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


void MainWindowInputEQSL::setLOTWSenStatus(const QString _qs)
{
      //qDebug() << "MainWindowInputEQSL::setLOTWSenStatus: " << _qs << endl;
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
       //qDebug() << "MainWindowInputEQSL::slotLotwRecvComboBoxChanged" << endl;

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
            lotwRecQDateEdit->setEnabled(false);
        break;
    }

}


void MainWindowInputEQSL::slotLotwSentComboBoxChanged(){
       //qDebug() << "MainWindowInputEQSL::slotLotwSentComboBoxChanged" << endl;

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
            lotwSentQDateEdit->setEnabled(false);
        break;
    }
}


void MainWindowInputEQSL::sloteQSLRecvComboBoxChanged(){
       //qDebug() << "MainWindowInputEQSL::sloteQSLRecvComboBoxChanged" << endl;

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
            eqslRecQDateEdit->setEnabled(false);
        break;


    }

}


void MainWindowInputEQSL::sloteQSLSentComboBoxChanged(){
       //qDebug() << "MainWindowInputEQSL::sloteQSLSentComboBoxChanged" << endl;

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

#include "awardswidget.h"

AwardsWidget::AwardsWidget(DataProxy_SQLite *dp, QWidget *parent) :
    QWidget(parent)
{
    //qDebug() << "AwardsWidget::AwardsWidget"   << endl;
    manageDXMarathon = false;
    logSeverity = 7;  //7 Debug /0=emergency or no debug
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    currentLog = -1;

    dxccConfirmedQLCDNumber = new QLCDNumber;
    dxccWorkedQLCDNumber = new QLCDNumber;
    wazConfirmedQLCDNumber = new QLCDNumber;
    wazWorkedQLCDNumber = new QLCDNumber;
    //localConfirmedQLCDNumber = new QLCDNumber;
    //localWorkedQLCDNumber = new QLCDNumber;
    qsoConfirmedQLCDNumber = new QLCDNumber;
    qsoWorkedQLCDNumber = new QLCDNumber;
    yearlyQSOLCDNumber = new QLCDNumber;

    yearlyDXCCQLCDNumber = new QLCDNumber;
    yearlyCQQLCDNumber = new QLCDNumber;
    yearlyScoreQLCDNumber = new QLCDNumber;
    operatingYearsComboBox = new QComboBox;

    yearlyLabelN = new QLabel();
    yearlyScoreLabelN = new QLabel();

    recalculateAwardsButton = new QPushButton;
    operatingYearsComboBox = new QComboBox;
    dataProxy = dp;
    awards = new Awards(dataProxy, Q_FUNC_INFO);
    //awards = new Awards(dataProxy, Q_FUNC_INFO);
    //util = new Utilities;


    clear();
    createUI();
    emit debugLog (Q_FUNC_INFO, "End", logSeverity);
     //qDebug() << "AwardsWidget::AwardsWidget - END"   << endl;
}

AwardsWidget::~AwardsWidget()
{

}

void AwardsWidget::clear()
{
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    //qDebug() << "AwardsWidget::clear"   << endl;
    /*
    searchBoxLineEdit->clear();
    searchResultsTreeWidget->clear();
    qslingNeeded = false;
    searchSelectAllClicked = false;
    */

    emit debugLog (Q_FUNC_INFO, "End", logSeverity);
    //qDebug() << "AwardsWidget::clear-END"   << endl;
}


void AwardsWidget::createUI()
{
    //searchBoxClearButton->setToolTip(tr("Clear the searches."));
    //qDebug() << "AwardsWidget::createUI"   << endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);

    recalculateAwardsButton = new QPushButton(tr("Recalculate"), this);
    recalculateAwardsButton->setToolTip(tr("Click to recalculate the award status."));
    operatingYearsComboBox->setToolTip(tr("Select the year you want to check."));

    QLabel *yearlyQSOsLabelN = new QLabel(tr("QSOs"));
    QLabel *yearlyDXCCLabelN = new QLabel(tr("DXCC"));
    QLabel *yearlyCQLabelN = new QLabel(tr("CQ"));

    QLabel *awardLabelN = new QLabel(tr("Award"));
    awardLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    QLabel *confirmedLabelN = new QLabel(tr("Confirmed"));
    confirmedLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *workedLabelN = new QLabel(tr("Worked"));
    workedLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *dxccLabelN = new QLabel(tr("DXCC"));
    dxccLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *wazLabelN = new QLabel(tr("WAZ"));
    wazLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    //QLabel *localLabelN = new QLabel(tr("Local"));
    //localLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *qsoNLabelN = new QLabel(tr("QSOs"));
    qsoNLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    yearlyLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    yearlyQSOsLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    yearlyDXCCLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    yearlyQSOsLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    yearlyCQLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    yearlyScoreLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);


#ifdef Q_OS_WIN
        //qDebug() << "AwardsWidget::createUIDX - WINDOWS DETECTED!"  << endl;

    qsoNLabelN->setFrameShadow(QFrame::Raised);
    qsoNLabelN->setFrameStyle(QFrame::StyledPanel);
    wazLabelN->setFrameShadow(QFrame::Raised);
    wazLabelN->setFrameStyle(QFrame::StyledPanel);
    //localLabelN->setFrameShadow(QFrame::Raised);
    //localLabelN->setFrameStyle(QFrame::StyledPanel);
    dxccLabelN->setFrameShadow(QFrame::Raised);
    dxccLabelN->setFrameStyle(QFrame::StyledPanel);
    workedLabelN->setFrameShadow(QFrame::Raised);
    workedLabelN->setFrameStyle(QFrame::StyledPanel);
    confirmedLabelN->setFrameShadow(QFrame::Raised);
    confirmedLabelN->setFrameStyle(QFrame::StyledPanel);
    awardLabelN->setFrameShadow(QFrame::Raised);
    awardLabelN->setFrameStyle(QFrame::StyledPanel);

    yearlyQSOsLabelN->setFrameShadow(QFrame::Raised);
    yearlyDXCCLabelN->setFrameShadow(QFrame::Raised);
    yearlyQSOsLabelN->setFrameShadow(QFrame::Raised);
    yearlyScoreLabelN->setFrameShadow(QFrame::Raised);
    yearlyLabelN->setFrameShadow(QFrame::Raised);

    yearlyQSOsLabelN->setFrameStyle(QFrame::StyledPanel);
    yearlyDXCCLabelN->setFrameStyle(QFrame::StyledPanel);
    yearlyQSOsLabelN->setFrameStyle(QFrame::StyledPanel);
    yearlyScoreLabelN->setFrameStyle(QFrame::StyledPanel);
    yearlyLabelN->setFrameStyle(QFrame::StyledPanel);


#else
    //qDebug() << "AwardsWidget::createUIDX - NO WINDOWS DETECTED!"  << endl;

    //localLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    qsoNLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    wazLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    dxccLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    workedLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    confirmedLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    awardLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    yearlyQSOsLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    yearlyDXCCLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    yearlyQSOsLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    yearlyCQLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    yearlyScoreLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    yearlyLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

#endif

    //qDebug() << "AwardsWidget::createUI-125"   << endl;
    //reconfigureyearlyUI(manageyearly);
    yearlyScoreLabelN->setText(tr("Score"));
    yearlyLabelN->setText(tr("Annual"));

    //qDebug() << "AwardsWidget::createUI-135"   << endl;
    QGridLayout *yearlyDLayout = new QGridLayout;

    yearlyDLayout->addWidget(yearlyQSOsLabelN, 0, 0);
    yearlyDLayout->addWidget(yearlyDXCCLabelN, 0, 1);
    yearlyDLayout->addWidget(yearlyCQLabelN, 0, 2);
    yearlyDLayout->addWidget(yearlyQSOLCDNumber, 1, 0);
    yearlyDLayout->addWidget(yearlyDXCCQLCDNumber, 1, 1);
    yearlyDLayout->addWidget(yearlyCQQLCDNumber, 1, 2);

    //qDebug() << "AwardsWidget::createUI-145"   << endl;
    QVBoxLayout *yearlyTLayout = new QVBoxLayout;
    yearlyTLayout->addWidget(yearlyLabelN);
    yearlyTLayout->addWidget(operatingYearsComboBox);

    //qDebug() << "AwardsWidget::createUI-150"   << endl;
    QGridLayout *dxUpRightAwardsTabLayout = new QGridLayout;
    //qDebug() << "AwardsWidget::createUI-151"   << endl;
    dxUpRightAwardsTabLayout->addWidget(awardLabelN, 0, 0);
    //qDebug() << "AwardsWidget::createUI-152"   << endl;
    dxUpRightAwardsTabLayout->addWidget(workedLabelN, 0, 1);
    //qDebug() << "AwardsWidget::createUI-153"   << endl;
    dxUpRightAwardsTabLayout->addWidget(confirmedLabelN, 0, 2);
    //qDebug() << "AwardsWidget::createUI-154"   << endl;
    dxUpRightAwardsTabLayout->addWidget(dxccLabelN, 1, 0);
    //qDebug() << "AwardsWidget::createUI-155"   << endl;
    dxUpRightAwardsTabLayout->addWidget(dxccWorkedQLCDNumber, 1, 1);
    //qDebug() << "AwardsWidget::createUI-156"   << endl;
    dxUpRightAwardsTabLayout->addWidget(dxccConfirmedQLCDNumber, 1, 2);
    //qDebug() << "AwardsWidget::createUI-157"   << endl;
    dxUpRightAwardsTabLayout->addWidget(wazLabelN, 2, 0);
    //qDebug() << "AwardsWidget::createUI-158"   << endl;
    dxUpRightAwardsTabLayout->addWidget(wazWorkedQLCDNumber, 2, 1);
    //qDebug() << "AwardsWidget::createUI-159"   << endl;
    dxUpRightAwardsTabLayout->addWidget(wazConfirmedQLCDNumber, 2, 2);
    //qDebug() << "AwardsWidget::createUI-160"   << endl;
    //dxUpRightAwardsTabLayout->addWidget(localLabelN, 3, 0);
    //qDebug() << "AwardsWidget::createUI-161"   << endl;
    //dxUpRightAwardsTabLayout->addWidget(localWorkedQLCDNumber, 3, 1);
    //qDebug() << "AwardsWidget::createUI-162"   << endl;
    //dxUpRightAwardsTabLayout->addWidget(localConfirmedQLCDNumber, 3, 2);
    //qDebug() << "AwardsWidget::createUI-163"   << endl;
    dxUpRightAwardsTabLayout->addWidget(qsoNLabelN, 4, 0);
    //qDebug() << "AwardsWidget::createUI-164"   << endl;
    dxUpRightAwardsTabLayout->addWidget(qsoWorkedQLCDNumber, 4, 1);
    //qDebug() << "AwardsWidget::createUI-165"   << endl;
    dxUpRightAwardsTabLayout->addWidget(qsoConfirmedQLCDNumber, 4, 2);
    //qDebug() << "AwardsWidget::createUI-166"   << endl;
    dxUpRightAwardsTabLayout->addLayout(yearlyTLayout, 5, 0);
    //qDebug() << "AwardsWidget::createUI-167"   << endl;
    dxUpRightAwardsTabLayout->addLayout(yearlyDLayout, 5, 1, 1, -1);
    //qDebug() << "AwardsWidget::createUI-168"   << endl;
    dxUpRightAwardsTabLayout->addWidget(recalculateAwardsButton, 6, 1);

    //qDebug() << "AwardsWidget::createUI-200"   << endl;
    setLayout(dxUpRightAwardsTabLayout);

    //connect(dataProxy, SIGNAL(qsoFound(QStringList)), this, SLOT(slotQsoFound(QStringList)) );
    //connect(searchBoxExportButton, SIGNAL(clicked()), this, SLOT(slotSearchExportButtonClicked() ) );

    setToolTips();

    connect(operatingYearsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotOperatingYearComboBoxChanged() ) ) ;
    connect(recalculateAwardsButton, SIGNAL(clicked()), this, SLOT(slotRecalculateAwardsButtonClicked() ) );

    emit debugLog (Q_FUNC_INFO, "End", logSeverity);
    //qDebug() << "AwardsWidget::createUI-END"   << endl;
}

void AwardsWidget::setManageDXMarathon(const bool _dx)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    manageDXMarathon = _dx;
    reconfigureDXMarathonUI(manageDXMarathon);
    emit debugLog(Q_FUNC_INFO, "End", logSeverity);
}

/*
void AwardsWidget::slotSearchClearButtonClicked()
{
     //qDebug() << "SearchWidget::slotSearchClearButtonClicked: " << endl;
    searchResultsTreeWidget->clear();
    searchBoxLineEdit->clear();
    searchSelectAllClicked = false;
    qslingNeeded = false;
}
*/

void AwardsWidget::setToolTips()
{
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    dxccConfirmedQLCDNumber->setToolTip(tr("Number of confirmed DXCC entities."));
    dxccWorkedQLCDNumber->setToolTip(tr("Number of worked DXCC entities."));
    wazConfirmedQLCDNumber->setToolTip(tr("Number of confirmed WAZ zones."));
    wazWorkedQLCDNumber->setToolTip(tr("Number of worked WAZ zones."));
    //localConfirmedQLCDNumber->setToolTip(tr("Number of confirmed local references."));
    //localWorkedQLCDNumber->setToolTip(tr("Number of worked local references."));
    qsoConfirmedQLCDNumber->setToolTip(tr("Number of confirmed QSOs."));
    qsoWorkedQLCDNumber->setToolTip(tr("Number of worked QSOs."));

    yearlyQSOLCDNumber->setToolTip(tr("Number of QSOs worked on the selected year."));
    yearlyDXCCQLCDNumber->setToolTip(tr("Number of DXCC worked on the selected year."));
    yearlyCQQLCDNumber->setToolTip(tr("Number of CQ Zones worked on the selected year."));
    yearlyScoreQLCDNumber->setToolTip(tr("Score for the DXMarathon on the selected year."));
    operatingYearsComboBox->setToolTip(tr("Select the year you want to check."));

    emit debugLog (Q_FUNC_INFO, "End", logSeverity);
}

void AwardsWidget::slotRecalculateAwardsButtonClicked()
{
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    awards->recalculateAwards();
    //emit recalculateAwardsSignal();
    showAwards();
    emit debugLog (Q_FUNC_INFO, "End", logSeverity);
}

void AwardsWidget::slotOperatingYearComboBoxChanged()
{
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    selectedYear = (operatingYearsComboBox->currentText()).toInt();
    showDXMarathon(selectedYear);
    emit debugLog (Q_FUNC_INFO, "End", logSeverity);
}

void AwardsWidget::showAwards()
{ // Updates and show all the award status tab.
    //qDebug() << "AwardsWidget::showAwards" << endl;
/*
  WAZ
  Local
*/
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    checkIfValidLog();
    int _num = 0;
    //QSqlQuery query;
    //QString aux;

    _num = dataProxy->getHowManyQSOInLog(currentLog);
    if (_num>99999)
    {
        qsoWorkedQLCDNumber->setDigitCount((QString::number(_num)).size());
    }
    else
    {
         qsoWorkedQLCDNumber->setDigitCount(5);
    }
    qsoWorkedQLCDNumber->display(_num);


    _num = dataProxy->getHowManyConfirmedQSLInLog(currentLog);
    if (_num>99999)
    {
        qsoConfirmedQLCDNumber->setDigitCount((QString::number(_num)).size());
    }
    else
    {
         qsoConfirmedQLCDNumber->setDigitCount(5);
    }

    qsoConfirmedQLCDNumber->display(_num);
    _num = 0;

    dxccWorkedQLCDNumber->display(awards->getDXCCWorked(currentLog));
    _num = 0;

    dxccConfirmedQLCDNumber->display(awards->getDXCCConfirmed(currentLog));
    _num = 0;

    wazWorkedQLCDNumber->display(awards->getWAZWorked(currentLog));
    _num = 0;

    wazConfirmedQLCDNumber->display(awards->getWAZConfirmed(currentLog));

    showDXMarathon(selectedYear);
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
     //qDebug() << "AwardsWidget::showAwards - END" << endl;
}

void AwardsWidget::showDXMarathon(const int _year)
{
        //qDebug() << "AwardsWidget::AwardsWidget::showDXMarathon: Year: " << QString::number(_year) << endl;
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    int i = 0;

    i = awards->getDXMarathonQSO(_year, currentLog);
        //qDebug() << "AwardsWidget::AwardsWidget::showDXMarathon: QSO: " << QString::number(i) << endl;
    yearlyQSOLCDNumber->display(i);

    i = awards->getDXMarathonDXCC(_year, currentLog);
        //qDebug() << "AwardsWidget::AwardsWidget::showDXMarathon: DXCC: " << QString::number(i) << endl;
    yearlyDXCCQLCDNumber->display(i);

    i = awards->getDXMarathonCQ(_year, currentLog);
    yearlyCQQLCDNumber->display(i);
        //qDebug() << "AwardsWidget::AwardsWidget::showDXMarathon: CQ: " << QString::number(i) << endl;

    if (manageDXMarathon)
    {
        i = awards->getDXMarathonScore(_year, currentLog);
    }
    else
    {
        i = 0;
    }
    yearlyScoreQLCDNumber->display(i);
    emit debugLog(Q_FUNC_INFO, "End", logSeverity);
    //qDebug() << "AwardsWidget::AwardsWidget::showDXMarathon: Score: " << QString::number(i) << endl;
}

void AwardsWidget::reconfigureDXMarathonUI(const bool _dxM)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    //dxClusterWidget->setDXMarathon(_dxM);
    if (_dxM)
    {
        yearlyLabelN->setText(tr("DX-Marathon"));
        yearlyScoreLabelN->setEnabled(true);
        yearlyScoreQLCDNumber->setEnabled(true);
        yearlyScoreQLCDNumber->display(0);
    }
    else
    {
        yearlyLabelN->setText(tr("Annual"));
        yearlyScoreLabelN->setEnabled(false);
        yearlyScoreQLCDNumber->setEnabled(false);
        emit requireCurrentYearSignal();

        int i = awards->getDXMarathonScore(selectedYear, currentLog);
        if (i>0)
        {
            yearlyScoreQLCDNumber->display(i);
        }
        else
        {
            yearlyScoreQLCDNumber->display(0);
        }
    }
    emit debugLog(Q_FUNC_INFO, "End", logSeverity);
}

void AwardsWidget::checkIfValidLog()
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    if (currentLog < 1)
    {
        emit requireCurrentLogSignal();
    }
    emit debugLog(Q_FUNC_INFO, "End", logSeverity);
}

void AwardsWidget::setLog(const int _log)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    if (_log >= 1)
    {
        currentLog = _log;
    }

    //TODO: Define an action when the log received is NOK
    emit debugLog(Q_FUNC_INFO, "End", logSeverity);
}


void AwardsWidget::setYear(const int _year)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    if (_year >= 1800)
    {
        selectedYear = _year;

        if (operatingYearsComboBox->findText(QString::number(selectedYear), Qt::MatchCaseSensitive)>=0)
        {
            operatingYearsComboBox->setCurrentIndex(operatingYearsComboBox->findText(QString::number(selectedYear), Qt::MatchCaseSensitive));
        }
        else
        {
        //TODO: Define what to do when the year is NOK
        }
    }
    emit debugLog(Q_FUNC_INFO, "End", logSeverity);
}

void AwardsWidget::fillOperatingYears()
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    emit requireCurrentLogSignal();
    emit requireCurrentYearSignal();
    operatingYearsComboBox->clear();

    if (dataProxy->getLastQSOid()<=1)
    {
        //qDebug() << "AwardsWidget::AwardsWidget: 18.4" << endl;
        operatingYearsComboBox->addItem(QString::number(selectedYear));
    }
    else
    {
         //qDebug() << "AwardsWidget::AwardsWidget: 18.5 - currentLog: " << QString::number(currentLog) << endl;
        operatingYearsComboBox->addItems(dataProxy->getOperatingYears(currentLog));
         //qDebug() << "AwardsWidget::AwardsWidget: 18.5.1 - currentLog: " << QString::number(currentLog) << endl;

        QStringList a;
        a.clear();
        a << dataProxy->getOperatingYears(currentLog);
         //qDebug() << "AwardsWidget::AwardsWidget: 18.5.1.1 - currentLog: " << QString::number(currentLog) << endl;
        if (!a.isEmpty())
        {
             //qDebug() << "AwardsWidget::AwardsWidget: 18.5.1.2 - currentLog: " << QString::number(currentLog) << endl;
            operatingYearsComboBox->setCurrentIndex(operatingYearsComboBox->findText(a.last(), Qt::MatchCaseSensitive));
             //qDebug() << "AwardsWidget::AwardsWidget: 18.5.1.3 - currentLog: " << QString::number(currentLog) << endl;
        }

         //qDebug() << "AwardsWidget::AwardsWidget: 18.5.2" << endl;
    }
    emit debugLog(Q_FUNC_INFO, "End", logSeverity);
}


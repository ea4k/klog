/***************************************************************************
                          awardswidget.cpp  -  description
                             -------------------
    begin                : nov 2019
    copyright            : (C) 2019 by Jaime Robles
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

#include "awardswidget.h"

AwardsWidget::AwardsWidget(DataProxy_SQLite *dp, World *injectedWorld, QWidget *parent) :
    QWidget(parent)
{
      //qDebug() << "AwardsWidget::AwardsWidget"  ;
    manageDXMarathon = false;
    logLevel = Info;  //7 Debug /0=emergency or no debug
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
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
    includeModeForNeededCheckBox = new QCheckBox;
    dataProxy = dp;
    world = injectedWorld;
    awards = new Awards(dataProxy, world, Q_FUNC_INFO);
    currentMode = -1;
    //awards = new Awards(dataProxy, Q_FUNC_INFO);
    //util = new Utilities(Q_FUNC_INFO);


    clear();
    createUI();
    emit debugLog (Q_FUNC_INFO, "END", Debug);
       //qDebug() << "AwardsWidget::AwardsWidget - END"  ;
}

AwardsWidget::~AwardsWidget()
{
    delete(awards);
}

void AwardsWidget::clear()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
      //qDebug() << "AwardsWidget::clear"  ;
    /*
    searchBoxLineEdit->clear();
    searchResultsTreeWidget->clear();
    qslingNeeded = false;
    searchSelectAllClicked = false;
    */

    emit debugLog (Q_FUNC_INFO, "END", Debug);
      //qDebug() << "AwardsWidget::clear-END"  ;
}


void AwardsWidget::createUI()
{
    //searchBoxClearButton->setToolTip(tr("Clear the searches."));
      //qDebug() << "AwardsWidget::createUI"  ;
    emit debugLog (Q_FUNC_INFO, "Start", Debug);

    recalculateAwardsButton = new QPushButton(tr("Recalculate"), this);
    recalculateAwardsButton->setToolTip(tr("Click to recalculate the award status."));
    includeModeForNeededCheckBox->setText(tr("Check band && mode for needed"));
    includeModeForNeededCheckBox->setChecked(false);
    includeModeForNeededCheckBox->setToolTip(tr("If checked, KLog considers both band and mode when evaluating if a QSO is needed or confirmed."));
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

    // qLabel *localLabelN = new QLabel(tr("Local"));
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
          //qDebug() << "AwardsWidget::createUIDX - WINDOWS DETECTED!" ;

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
    yearlyCQLabelN->setFrameShadow(QFrame::Raised);

    yearlyQSOsLabelN->setFrameStyle(QFrame::StyledPanel);
    yearlyDXCCLabelN->setFrameStyle(QFrame::StyledPanel);
    yearlyQSOsLabelN->setFrameStyle(QFrame::StyledPanel);
    yearlyScoreLabelN->setFrameStyle(QFrame::StyledPanel);
    yearlyLabelN->setFrameStyle(QFrame::StyledPanel);
    yearlyCQLabelN->setFrameStyle(QFrame::StyledPanel);


#else
      //qDebug() << "AwardsWidget::createUIDX - NO WINDOWS DETECTED!" ;

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

      //qDebug() << "AwardsWidget::createUI-125"  ;
    //reconfigureyearlyUI(manageyearly);
    yearlyScoreLabelN->setText(tr("Score"));
    yearlyLabelN->setText(tr("Annual"));

      //qDebug() << "AwardsWidget::createUI-135"  ;
    QGridLayout *yearlyDLayout = new QGridLayout;

    yearlyDLayout->addWidget(yearlyQSOsLabelN, 0, 0);
    yearlyDLayout->addWidget(yearlyDXCCLabelN, 0, 1);
    yearlyDLayout->addWidget(yearlyCQLabelN, 0, 2);
    yearlyDLayout->addWidget(yearlyQSOLCDNumber, 1, 0);
    yearlyDLayout->addWidget(yearlyDXCCQLCDNumber, 1, 1);
    yearlyDLayout->addWidget(yearlyCQQLCDNumber, 1, 2);

      //qDebug() << "AwardsWidget::createUI-145"  ;
    QVBoxLayout *yearlyTLayout = new QVBoxLayout;
    yearlyTLayout->addWidget(yearlyLabelN);
    yearlyTLayout->addWidget(operatingYearsComboBox);

      //qDebug() << "AwardsWidget::createUI-150"  ;
    QGridLayout *dxUpRightAwardsTabLayout = new QGridLayout;
      //qDebug() << "AwardsWidget::createUI-151"  ;
    dxUpRightAwardsTabLayout->addWidget(awardLabelN, 0, 0);
      //qDebug() << "AwardsWidget::createUI-152"  ;
    dxUpRightAwardsTabLayout->addWidget(workedLabelN, 0, 1);
      //qDebug() << "AwardsWidget::createUI-153"  ;
    dxUpRightAwardsTabLayout->addWidget(confirmedLabelN, 0, 2);
      //qDebug() << "AwardsWidget::createUI-154"  ;
    dxUpRightAwardsTabLayout->addWidget(dxccLabelN, 1, 0);
      //qDebug() << "AwardsWidget::createUI-155"  ;
    dxUpRightAwardsTabLayout->addWidget(dxccWorkedQLCDNumber, 1, 1);
      //qDebug() << "AwardsWidget::createUI-156"  ;
    dxUpRightAwardsTabLayout->addWidget(dxccConfirmedQLCDNumber, 1, 2);
      //qDebug() << "AwardsWidget::createUI-157"  ;
    dxUpRightAwardsTabLayout->addWidget(wazLabelN, 2, 0);
      //qDebug() << "AwardsWidget::createUI-158"  ;
    dxUpRightAwardsTabLayout->addWidget(wazWorkedQLCDNumber, 2, 1);
      //qDebug() << "AwardsWidget::createUI-159"  ;
    dxUpRightAwardsTabLayout->addWidget(wazConfirmedQLCDNumber, 2, 2);
      //qDebug() << "AwardsWidget::createUI-160"  ;
    //dxUpRightAwardsTabLayout->addWidget(localLabelN, 3, 0);
      //qDebug() << "AwardsWidget::createUI-161"  ;
    //dxUpRightAwardsTabLayout->addWidget(localWorkedQLCDNumber, 3, 1);
      //qDebug() << "AwardsWidget::createUI-162"  ;
    //dxUpRightAwardsTabLayout->addWidget(localConfirmedQLCDNumber, 3, 2);
      //qDebug() << "AwardsWidget::createUI-163"  ;
    dxUpRightAwardsTabLayout->addWidget(qsoNLabelN, 4, 0);
      //qDebug() << "AwardsWidget::createUI-164"  ;
    dxUpRightAwardsTabLayout->addWidget(qsoWorkedQLCDNumber, 4, 1);
      //qDebug() << "AwardsWidget::createUI-165"  ;
    dxUpRightAwardsTabLayout->addWidget(qsoConfirmedQLCDNumber, 4, 2);
      //qDebug() << "AwardsWidget::createUI-166"  ;
    dxUpRightAwardsTabLayout->addLayout(yearlyTLayout, 5, 0);
      //qDebug() << "AwardsWidget::createUI-167"  ;
    dxUpRightAwardsTabLayout->addLayout(yearlyDLayout, 5, 1, 1, -1);
      //qDebug() << "AwardsWidget::createUI-168"  ;
    dxUpRightAwardsTabLayout->addWidget(includeModeForNeededCheckBox, 6, 0);
    dxUpRightAwardsTabLayout->addWidget(recalculateAwardsButton, 6, 1);

      //qDebug() << "AwardsWidget::createUI-200"  ;
    setLayout(dxUpRightAwardsTabLayout);

    //connect(dataProxy, SIGNAL(qsoFound(QStringList)), this, SLOT(slotQsoFound(QStringList)) );
    //connect(searchBoxExportButton, SIGNAL(clicked()), this, SLOT(slotSearchExportButtonClicked() ) );

    setToolTips();

    connect(operatingYearsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotOperatingYearComboBoxChanged() ) ) ;
    connect(recalculateAwardsButton, SIGNAL(clicked()), this, SLOT(slotRecalculateAwardsButtonClicked() ) );
    connect(dataProxy, &DataProxy_SQLite::logChanged, this, &AwardsWidget::slotRefreshYearsComboBox);
    connect(includeModeForNeededCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slotIncludeModeForNeededChanged(int)));
    emit debugLog (Q_FUNC_INFO, "END", Debug);
      //qDebug() << "AwardsWidget::createUI-END"  ;
}

void AwardsWidget::setManageDXMarathon(const bool _dx)
{
    emit debugLog(Q_FUNC_INFO, "Start", Devel);
    manageDXMarathon = _dx;
    reconfigureDXMarathonUI(manageDXMarathon);
    emit debugLog(Q_FUNC_INFO, "End", Devel);
}

/*
void AwardsWidget::slotSearchClearButtonClicked()
{
       //qDebug() << "SearchWidget::slotSearchClearButtonClicked: ";
    searchResultsTreeWidget->clear();
    searchBoxLineEdit->clear();
    searchSelectAllClicked = false;
    qslingNeeded = false;
}
*/

void AwardsWidget::setToolTips()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    dxccConfirmedQLCDNumber->setToolTip(tr("Number of confirmed DXCC entities."));
    dxccWorkedQLCDNumber->setToolTip(tr("Number of worked DXCC entities."));
    wazConfirmedQLCDNumber->setToolTip(tr("Number of confirmed WAZ zones."));
    wazWorkedQLCDNumber->setToolTip(tr("Number of worked WAZ zones."));
    //localConfirmedQLCDNumber->setToolTip(tr("Number of confirmed local references."));
    //localWorkedQLCDNumber->setToolTip(tr("Number of worked local references."));
    qsoConfirmedQLCDNumber->setToolTip(tr("Number of confirmed QSOs."));
    qsoWorkedQLCDNumber->setToolTip(tr("Number of worked QSOs."));

    yearlyQSOLCDNumber->setToolTip(tr("Number of QSOs worked in the selected year."));
    yearlyDXCCQLCDNumber->setToolTip(tr("Number of DXCCs worked in the selected year."));
    yearlyCQQLCDNumber->setToolTip(tr("Number of CQ Zones worked in the selected year."));
    yearlyScoreQLCDNumber->setToolTip(tr("Score for the DXMarathon in the selected year."));
    operatingYearsComboBox->setToolTip(tr("Select the year you want to check."));

    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void AwardsWidget::slotRecalculateAwardsButtonClicked()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    awards->recalculateAwards();
    //emit recalculateAwardsSignal();
    showAwards();
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void AwardsWidget::slotOperatingYearComboBoxChanged()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    selectedYear = (operatingYearsComboBox->currentText()).toInt();
    showDXMarathon(selectedYear);
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void AwardsWidget::showAwards()
{ // Updates and show all the award status tab.
      //qDebug() << Q_FUNC_INFO << " - Start";
    emit debugLog(Q_FUNC_INFO, "Start", Devel);
    checkIfValidLog();
    int _num = 0;

    const QList<int> modeFilter = (includeModeForNeededCheckBox->isChecked() && currentMode >= 0)
        ? dataProxy->getModeGroupIds(currentMode)
        : QList<int>();

    _num = dataProxy->getHowManyQSOInLog(currentLog, modeFilter);
    if (_num>99999)
    {
        qsoWorkedQLCDNumber->setDigitCount((QString::number(_num)).size());
    }
    else
    {
         qsoWorkedQLCDNumber->setDigitCount(5);
    }
    qsoWorkedQLCDNumber->display(_num);

    _num = dataProxy->getHowManyConfirmedQSLInLog(currentLog, modeFilter);
    if (_num>99999)
    {
        qsoConfirmedQLCDNumber->setDigitCount((QString::number(_num)).size());
    }
    else
    {
         qsoConfirmedQLCDNumber->setDigitCount(5);
    }
    qsoConfirmedQLCDNumber->display(_num);

    dxccWorkedQLCDNumber->display(dataProxy->getFieldInBand(DXCC, "ALL", false, "ALL", currentLog, modeFilter));
    dxccConfirmedQLCDNumber->display(dataProxy->getFieldInBand(DXCC, "ALL", true, "ALL", currentLog, modeFilter));
    wazWorkedQLCDNumber->display(awards->getWAZWorked(currentLog, modeFilter));
    wazConfirmedQLCDNumber->display(awards->getWAZConfirmed(currentLog, modeFilter));

    showDXMarathon(selectedYear);
    emit debugLog(Q_FUNC_INFO, "End", Devel);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void AwardsWidget::showDXMarathon(const int _year)
{
          //qDebug() << "AwardsWidget::AwardsWidget::showDXMarathon: Year: " << QString::number(_year);
    emit debugLog(Q_FUNC_INFO, "Start", Devel);
    int i = 0;

    const QList<int> modeFilter = (includeModeForNeededCheckBox->isChecked() && currentMode >= 0)
        ? dataProxy->getModeGroupIds(currentMode)
        : QList<int>();

    i = dataProxy->getQSOonYear(_year, currentLog, modeFilter);
          //qDebug() << "AwardsWidget::AwardsWidget::showDXMarathon: QSO: " << QString::number(i);
    yearlyQSOLCDNumber->display(i);

    i = dataProxy->getDXCConYear(_year, currentLog, modeFilter);
          //qDebug() << "AwardsWidget::AwardsWidget::showDXMarathon: DXCC: " << QString::number(i);
    yearlyDXCCQLCDNumber->display(i);

    i = dataProxy->getCQzonYear(_year, currentLog, modeFilter);
    yearlyCQQLCDNumber->display(i);
          //qDebug() << "AwardsWidget::AwardsWidget::showDXMarathon: CQ: " << QString::number(i);

    if (manageDXMarathon)
    {
        // Score = DXCC + CQ (same formula as DXMarathon, now mode-filtered)
        i = dataProxy->getDXCConYear(_year, currentLog, modeFilter)
          + dataProxy->getCQzonYear(_year, currentLog, modeFilter);
    }
    else
    {
        i = 0;
    }
    yearlyScoreQLCDNumber->display(i);
    emit debugLog(Q_FUNC_INFO, "End", Devel);
      //qDebug() << "AwardsWidget::AwardsWidget::showDXMarathon: Score: " << QString::number(i);
}

void AwardsWidget::reconfigureDXMarathonUI(const bool _dxM)
{
    emit debugLog(Q_FUNC_INFO, "Start", Devel);
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
    emit debugLog(Q_FUNC_INFO, "End", Devel);
}

void AwardsWidget::checkIfValidLog()
{
    emit debugLog(Q_FUNC_INFO, "Start", Devel);
    if (currentLog < 1)
    {
        emit requireCurrentLogSignal();
    }
    emit debugLog(Q_FUNC_INFO, "End", Devel);
}

void AwardsWidget::setLog(const int _log)
{
    emit debugLog(Q_FUNC_INFO, "Start", Devel);
    if (_log >= 1)
    {
        currentLog = _log;
    }

    //TODO: Define an action when the log received is NOK
    emit debugLog(Q_FUNC_INFO, "End", Devel);
}


void AwardsWidget::setYear(const int _year)
{
    emit debugLog(Q_FUNC_INFO, "Start", Devel);
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
    emit debugLog(Q_FUNC_INFO, "End", Devel);
}

void AwardsWidget::fillOperatingYears()
{

    emit requireCurrentLogSignal();
    emit requireCurrentYearSignal();
    operatingYearsComboBox->clear();

    if (dataProxy->getLastQSOid()<=1)
    {
          //qDebug() << "AwardsWidget::AwardsWidget: 18.4";
        operatingYearsComboBox->addItem(QString::number(selectedYear));
    }
    else
    {
           //qDebug() << "AwardsWidget::AwardsWidget: 18.5 - currentLog: " << QString::number(currentLog);
        operatingYearsComboBox->addItems(dataProxy->getOperatingYears(currentLog));
           //qDebug() << "AwardsWidget::AwardsWidget: 18.5.1 - currentLog: " << QString::number(currentLog);

        QStringList a;
        a.clear();
        a << dataProxy->getOperatingYears(currentLog);
           //qDebug() << "AwardsWidget::AwardsWidget: 18.5.1.1 - currentLog: " << QString::number(currentLog);
        if (!a.isEmpty())
        {
               //qDebug() << "AwardsWidget::AwardsWidget: 18.5.1.2 - currentLog: " << QString::number(currentLog);
            operatingYearsComboBox->setCurrentIndex(operatingYearsComboBox->findText(a.last(), Qt::MatchCaseSensitive));
               //qDebug() << "AwardsWidget::AwardsWidget: 18.5.1.3 - currentLog: " << QString::number(currentLog);
        }

           //qDebug() << "AwardsWidget::AwardsWidget: 18.5.2";
    }
    emit debugLog(Q_FUNC_INFO, "End", Devel);
}

void AwardsWidget::slotRefreshYearsComboBox()
{
    emit debugLog(Q_FUNC_INFO, "Start", Devel);
    fillOperatingYears();  // reutiliza la lógica correcta y consistente
    showAwards();          // refresca los LCD (contadores de QSOs, etc.)
    emit debugLog(Q_FUNC_INFO, "End", Devel);
}

void AwardsWidget::slotIncludeModeForNeededChanged(int state)
{
    emit debugLog(Q_FUNC_INFO, "Start", Devel);
    emit includeModeForNeededChanged(state != 0);
    emit debugLog(Q_FUNC_INFO, "End", Devel);
}

void AwardsWidget::setIncludeModeForNeeded(const bool _include)
{
    emit debugLog(Q_FUNC_INFO, "Start", Devel);
    includeModeForNeededCheckBox->blockSignals(true);
    includeModeForNeededCheckBox->setChecked(_include);
    includeModeForNeededCheckBox->blockSignals(false);
    emit debugLog(Q_FUNC_INFO, "End", Devel);
}

bool AwardsWidget::getIncludeModeForNeeded()
{
    return includeModeForNeededCheckBox->isChecked();
}

void AwardsWidget::setCurrentMode(const int _modeId)
{
    currentMode = _modeId;
}

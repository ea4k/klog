#include "tipsdialog.h"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

TipsDialog::TipsDialog(QWidget *parent): QDialog(parent)
{
      //qDebug() << "TipsDialog::TipsDialog" << endl;
    logSeverity = Info;  //7 Debug /0=emergency or no debug
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    tipTextQLabel = new QLabel;
    //tipTextEdit = new QTextEdit;
    //tipTextEdit->setReadOnly(true);
    //tipTextEdit->setWordWrapMode(QTextOption::WordWrap);
    tipId = 1;
    tipMax = 20;

    //QPixmap pixmap(":/img/klog_256x256.png");


    setWindowTitle(tr("KLog tips"));
    setWindowFlags(windowFlags() & Qt::WindowContextHelpButtonHint);


    //description = tr("KLog is a free logbook for hamradio operators.") ;
    //tipTextEdit->setHtml(description);
    //tipTextQLabel->setText(description);
    tipTextQLabel->setWordWrap(true);
    tipTextQLabel->setOpenExternalLinks(false);
    tipTextQLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    tipTextQLabel->setTextFormat(Qt::RichText);
    setTip(1);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    QPushButton *closeButton = buttonBox->button(QDialogButtonBox::Close);
    QPushButton *nextButton = new QPushButton;
    QPushButton *prevButton = new QPushButton;
    nextButton->setText(tr("Next"));
    prevButton->setText(tr("Previous"));

    buttonBox->addButton(closeButton, QDialogButtonBox::ButtonRole(QDialogButtonBox::RejectRole | QDialogButtonBox::AcceptRole));
    buttonBox->addButton(prevButton, QDialogButtonBox::ButtonRole(QDialogButtonBox::ActionRole));
    buttonBox->addButton(nextButton, QDialogButtonBox::ButtonRole(QDialogButtonBox::ActionRole));

    connect(buttonBox , &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(prevButton, SIGNAL(clicked()), this, SLOT(slotPrevButtonClicked() ) );
    connect(nextButton, SIGNAL(clicked()), this, SLOT(slotNextButtonClicked() ) );
    connect(tipTextQLabel, SIGNAL(linkActivated(QString)), this, SLOT(slotLinkActivated(QString)));

    //QLabel *logoLabel = new QLabel;
    //logoLabel->setPixmap(pixmap);

    tip = new QWidget;

    QGridLayout *layout1 = new QGridLayout;
    //layout1->addWidget(logoLabel , 0, 0, 1, 1);
    layout1->addWidget(tipTextQLabel, 0, 1, 4, 4);
    tip->setLayout(layout1);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tip);
    layout->addWidget(buttonBox);
    //layout->setSizeConstraint(QLayout::SetNoConstraint);
    setLayout(layout);


       //qDebug() << "TipsDialog::TipsDialog - END" << endl;

    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

TipsDialog::~TipsDialog(){}


void TipsDialog::slotPrevButtonClicked()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);


    if (tipId>1)
    {
        tipId--;
    }
    else
    {
        tipId = tipMax;
    }
    setTip(tipId);

    emit debugLog (Q_FUNC_INFO, "END", Debug);

}

void TipsDialog::slotNextButtonClicked()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);

    if (tipId<tipMax)
    {
        tipId++;
    }
    else
    {
        tipId = 1;
    }
    setTip(tipId);

    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void TipsDialog::setSeverity(const DebugLogLevel _sev)
{
    logSeverity = _sev;
}

void TipsDialog::setTip(const int _t)
{
    //QSize _size = tipTextQLabel->sizeHint();
    //qDebug() << "TipsDialog::setTip: Height: " << QString::number(_t) <<endl;
    emit debugLog (Q_FUNC_INFO, "Start", Debug);

    switch (_t) {

    case 1:
        //: Translator: Please make sure that the name of the link is coherent with the menu Tools->Fill in QSO data
        description = tr("<b>Tip #1:</b><br>Do you know...<br>You can use <a href=\"#ToolsFillInQSO\">Tools->Fill in QSO data</a> to automatically read the full log to fill the DXCC, CQ, ITU zones and continent?");
    break;
    case 2:
        //: Translator: Please make sure that the name of the link is coherent with the menu Tools->Fill in DXCC data
        description = tr("<b>Tip #2:</b><br>Do you know...<br>You can use <a href=\"#ToolsFillInDXCC\">Tools->Fill in DXCC data</a> to automatically read the full log to fill the DXCC QSO data?");
    break;
    case 3:
        //: Translator: Please make sure that the name of the link is coherent with the menu Tools->QSL tools...->Find QSO to QSL
        description = tr("<b>Tip #3:</b><br>Do you know...<br>You can use <a href=\"#ToolsFindQSO2QSL\">Tools->QSL tools...->Find QSO to QSL</a> to look for all those QSO that you should send your QSL because you still need to confirm that DXCC and you have still not send your QSL card?");
    break;
    case 4:
        //: Translator: Please make sure that the name of the link is coherent with the menu File->Export Requested QSL to ADIF...
        description = tr("<b>Tip #4:</b><br>Do you know...<br>You can export your QSO marked as QSL requested with <a href=\"#FileExportQSLADIFToPrint\">File->Export Requested QSL to ADIF...</a> to create an ADIF file that you will be able to import into a QSL tag creation program to print tags for your QSL cards?");
    break;
    case 5:
        description = tr("<b>Tip #5:</b><br>Do you know...<br>You can enter a '*' in the search box, in the search window to search for all the QSOs done with one specific station callsign?");
    break;
    case 6:
        //: Translator: Please make sure that the name of the link is coherent with the menu File->KLog folder
        description = tr("<b>Tip #6:</b><br>Do you know...<br>You can find the file containing all your log and other information in the logbook.dat "
                         "file and the klogrc file, containing the KLog config file in the KLog folder by opening the"
                         " <a href=\"#FileOpenKLogFolder\">File->KLog folder</a> menu?");
    break;
    case 7:
        //: Translator: Please make sure that the name of the link is coherent with the menu QSL tools...->Find My-QSLs pending to send
        description = tr("<b>Tip #7:</b><br>Do you know...<br>You can find the QSLs that you still need to send with <a href=\"#ToolsSendPendingQSL\">Tools->QSL tools...->Find My-QSLs pending to send</a>.<bR>This tool will list you in the search box all the QSOs with the QSL-Sent marked as <i>Requested</i>.");
    break;
    case 8:
        //: Translator: Please make sure that the name of the link is coherent with the menu QSL tools...->Find DX-QSLs pending to receive
        description = tr("<b>Tip #8:</b><br>Do you know...<br>You can find the QSLs that you are still waiting for with <a href=\"#ToolsReceivePendingQSL\">Tools->QSL tools...->Find DX-QSLs pending to receive</a>.<bR>This tool will list you in the search box all the QSOs with the QSL-Sent marked as <i>Sent</i> but you have still not received the QSL card from the DX.");
    break;
    case 9:
        //: Translator: Please make sure that the name of the link is coherent with the menu QSL tools...->Find requested pending to receive
        description = tr("<b>Tip #9:</b><br>Do you know...<br>You can find the QSLs that you are still waiting for with <a href=\"#ToolsReceiveRecPendingQSL\">Tools->QSL tools...->Find requested pending to receive</a>.<bR>This tool will list you in the search box all the QSOs with the QSL-Rec marked as <i>Requested</i> but you have still not received the QSL card from the DX.");
    break;
    case 10:
        description = tr("<b>Tip #10:</b><br>Do you know...<br>You can subscribe to the <a href=https://t.me/klogchat>English KLog Telegram group</a> to discuss about KLog in English?");
    break;
    case 11:
        description = tr("<b>Tip #11:</b><br>Do you know...<br>You can subscribe to the <a href=https://t.me/KLogES>Spanish Telegram group</a> to discuss about KLog in Spanish?");
    break;
    case 12:
        description = tr("<b>Tip #12:</b><br>Do you know...<br>You can subscribe to <a href=https://lists.nongnu.org/mailman/listinfo/klog-users>KLog-users mailing list</a> to discuss via email about KLog in English?");
    break;
    case 13:
        description = tr("<b>Tip #13:</b><br>Do you know...<br>You can <a href=https://twitter.com/_ea4k>follow EA4K on twitter</a> to get updates about KLog?");
    break;
    case 14:
        description = tr("<b>Tip #14:</b><br>Do you know...<br>You can write your own <a href=https://www.eham.net/reviews/detail/3118>review in eHam.net about KLog</a> to help other users to decide to use KLog?");
    break;
    case 15:
        description = tr("<b>Tip #15:</b><br>Do you know...<br>You can join the development team by simply <a href=https://www.klog.xyz/contact>Contacting us</a>?");
    break;
    case 16:
        description = tr("<b>Tip #16:</b><br>Do you know...<br>That there are many ways to contribute to KLog and some of them are listed in the <a href=https://www.klog.xyz/contrib>KLog Contribute</a> page?");
    break;
    case 17:
        description = tr("<b>Tip #17:</b><br>Do you know...<br>You can support translating KLog into your language? Please check <a href=https://www.klog.xyz/contrib/translations>KLog Translations</a> page.");
    break;
    case 18:
        description = tr("<b>Tip #18:</b><br>Do you know...<br>You can double-click on an entity name in the DXCC table and all the QSOs with that DXCC Entity will be shown in the search box?");
    break;
    case 19:
        description = tr("<b>Tip #19:</b><br>Do you know...<br>You can right-click on a QSO and select <i>Check in QRZ.com</i> to check that callsign in QRZ.com?");
    break;
    case 20:
        description = tr("<b>Tip #20:</b><br>Do you know...<br>You can see the QSO that confirms one specific DXCC entity in one specific band by poiting your mouse over that band in the DXCC widget?");
    break;
    case 21:
        //: Translator: Please make sure that the name of the link is coherent with the menu File->Export ADIF for LoTW...
        description = tr("<b>Tip #21:</b><br>Do you know...<br>You can upload your QSO marked as queued to LoTW via TQSL with <a href=\"#ToolsUploadLoTW\">Tools->Upload to LoTW...</a> ?<br><br>You have to configure TQSL in the preferences to be able to use this functionality.");
    break;
    default:
        //description = tr("TIP-Default: Text");
        description = "";
    break;
    }
    //tipTextEdit->setHtml(description);

    tipTextQLabel->setText(description);

    emit debugLog (Q_FUNC_INFO, "END", Debug);
      //qDebug() << "TipsDialog::setTip: END"  << endl;
}

void TipsDialog::slotLinkActivated(const QString &_link)
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);

      //qDebug() << "TipsDialog::slotLinkActivated: " << _link << endl;
    //Comprobar el enalce y activar el menu correspondiente
    if (_link == "#ToolsFillInQSO")
    {
        emit fillInQSOSignal();
    }
    else if (_link == "#ToolsFindQSO2QSL")
    {
        emit fillInDXCCSignal();
    }
    else if (_link == "#ToolsFillInDXCC")
    {
        emit findQSL2QSOSignal();
    }
    else if (_link == "#FileExportQSLADIFToPrint")
    {
        emit fileExportToPrintSignal();
    }
    else if (_link == "#FileExportLoTWADIF")
    {
        //emit fileExportForLoTWSignal();
    }
    else if (_link == "#FileOpenKLogFolder")
    {
        emit fileOpenKLogFolderSignal();
    }
    else if (_link == "#ToolsSendPendingQSL")
    {
        emit toolSendPendingQSLSignal();
    }
    else if (_link == "#ToolsReceivePendingQSL")
    {
        emit toolRecPendingQSLSignal();
    }
    else if (_link == "#ToolsReceiveRecPendingQSL")
    {
        emit toolRecRecPendingQSLSignal();
    }
    else if (_link == "#ToolsUploadLoTW")
    {
        emit toolsUploadLoTWSignal();
    }

    emit debugLog (Q_FUNC_INFO, "END", Debug);
}


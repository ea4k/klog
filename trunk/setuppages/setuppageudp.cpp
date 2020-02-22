#include "setuppages/setuppageudp.h"

SetupPageUDP::SetupPageUDP(QWidget *parent) : QWidget(parent)
{
    
    logFromWSJTXCheckbox = new QCheckBox(this);
    realDataFromWSJTXCheckbox  = new QCheckBox(this);
    logAutomaticallyWSJTXCheckbox  = new QCheckBox(this);

    UDPServerCheckBox = new QCheckBox(tr("Start UDP Server"), this);
    UDPServerPortSpinBox = new QSpinBox;
    miliSecsSpinBox = new QSpinBox;
    
    //wsjtxIPAddress = new QLineEdit(this);
    //wsjtxPortNumber = new QLineEdit(this);
    //logFromWSJTx = false;
    //autoLogFromWSJTx = false;
    //realDataFromWSJTx = false;
    
    defaultport = 2237;     // Default WSJTX port
    defaultTimer = 2000;  // 2 secs

    createUI();
    createActions();
}

SetupPageUDP::~SetupPageUDP(){
     //qDebug() << "SetupPageUDP::~SetupPageUDP" << endl;
}

void SetupPageUDP::createUI()
{
    QString msgAuto = QString(tr("Log automatically QSOs from WSJT-X"));
    logFromWSJTXCheckbox->setText(tr("Allow WSJT-X to send logged QSO to KLog"));
    logFromWSJTXCheckbox->setToolTip(tr("QSO logged in WSJT-X will be sent to KLog and KLog will ask before logging into KLog unless \"%1\" is selected").arg(msgAuto));

    logAutomaticallyWSJTXCheckbox->setText(msgAuto);
    logAutomaticallyWSJTXCheckbox->setToolTip(tr("KLog will log automatically any QSO coming from WSJT-X without any manual confirmation."));


    realDataFromWSJTXCheckbox->setText(tr("Receive and Update QSO data to KLog"));
    realDataFromWSJTXCheckbox->setToolTip(tr("KLog will automatically show and update data coming from WSJT-X (DX callsign, locator, RPT, ...)"));
    
    UDPServerCheckBox->setChecked(false);
    logFromWSJTXCheckbox->setChecked(false);
    logAutomaticallyWSJTXCheckbox->setChecked(false);
    realDataFromWSJTXCheckbox->setChecked(false);

    logFromWSJTXCheckbox->setEnabled(false);
    logAutomaticallyWSJTXCheckbox->setEnabled(false);
    realDataFromWSJTXCheckbox->setEnabled(false);

    QVBoxLayout *checkLayout = new QVBoxLayout;
    checkLayout->addWidget(logFromWSJTXCheckbox);
    checkLayout->addWidget(logAutomaticallyWSJTXCheckbox);
    checkLayout->addWidget(realDataFromWSJTXCheckbox);

    QString labelTip = tr("UDP port number where the UDP Server will listen for packets.") + "\n" + tr("Make sure it is the same port where the other programs are sending the data to. Default port is 2237.");
    UDPServerCheckBox->setToolTip(tr("UDP Server will receive QSOs sent from other programs like WSJT-X allowing you to log in KLog automatically from those programs."));
    UDPServerPortSpinBox->setToolTip(labelTip);

    UDPServerPortSpinBox->setMinimum(0);
    UDPServerPortSpinBox->setMaximum(65535);
    UDPServerPortSpinBox->setValue(defaultport);
    QLabel *udpPortLabel = new QLabel(tr("UDP Port"));
    udpPortLabel->setBuddy(UDPServerPortSpinBox);
    udpPortLabel->setToolTip(labelTip);
    udpPortLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    udpPortLabel->setEnabled(true);

    QHBoxLayout *UDPLayout1 = new QHBoxLayout;
    UDPLayout1->addWidget(udpPortLabel);
    UDPLayout1->addWidget(UDPServerPortSpinBox);

    miliSecsSpinBox->setMinimum(0);
    miliSecsSpinBox->setMaximum(30000);
    miliSecsSpinBox->setValue(defaultTimer);
    QLabel *miliSecsSpinBoxLabel = new QLabel(tr("QSO notification timeout (milisecs)"));
    miliSecsSpinBox->setToolTip(tr("Milliseconds that the notification of QSO received from WSJTX will be shown."));
    miliSecsSpinBoxLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    miliSecsSpinBoxLabel->setEnabled(true);
    miliSecsSpinBox->setEnabled(false);


    QHBoxLayout *UDPLayout = new QHBoxLayout;
    UDPLayout->addWidget(UDPServerCheckBox);
    UDPLayout->addLayout(UDPLayout1);
    UDPLayout->addWidget(miliSecsSpinBoxLabel);
    UDPLayout->addWidget(miliSecsSpinBox);



    //QLabel *IPAddressLabel = new QLabel(tr("WSJT-x IP address"));
    //QLabel *portLabel = new QLabel(tr("UDP port to listen"));

    //IPAddressLabel->setBuddy(wsjtxIPAddress);
    //portLabel->setBuddy(wsjtxPortNumber);
    
    //QHBoxLayout *IPALayout = new QHBoxLayout;
    //IPALayout->addWidget(wsjtxIPAddress);
    //IPALayout->addWidget(IPAddressLabel);

    //QHBoxLayout *portLayout = new QHBoxLayout;
    //portLayout->addWidget(wsjtxPortNumber);
    //portLayout->addWidget(portLabel);


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(UDPLayout);
    layout->addLayout(checkLayout);

    setLayout(layout);
}

void SetupPageUDP::createActions()
{
    connect(logFromWSJTXCheckbox, SIGNAL(clicked () ), this, SLOT(slotLogFromWSJTCheckBoxClicked() ) );
    //connect(logAutomaticallyWSJTXCheckbox, SIGNAL(clicked () ), this, SLOT(slotAutoLogFromWSJTCheckBoxClicked() ) );
    //connect(realDataFromWSJTXCheckbox, SIGNAL(clicked () ), this, SLOT(slotRealFromWSJTCheckBoxClicked() ) );
    connect(UDPServerCheckBox, SIGNAL(clicked () ), this, SLOT(slotUDPServerCheckBoxClicked() ) );

}

void SetupPageUDP::slotUDPServerCheckBoxClicked()
 {
     //qDebug() << "SetupPageUDP::slotUDPServerCheckBoxClicked" << endl;

    if (UDPServerCheckBox->isChecked())
    {
        logFromWSJTXCheckbox->setEnabled(true);
        logFromWSJTXCheckbox->setChecked(true);
        logAutomaticallyWSJTXCheckbox->setEnabled(true);
        realDataFromWSJTXCheckbox->setEnabled(true);
        miliSecsSpinBox->setEnabled(true);
    }
    else
    {
        logFromWSJTXCheckbox->setChecked(false);
        logAutomaticallyWSJTXCheckbox->setChecked(false);
        realDataFromWSJTXCheckbox->setChecked(false);
        logFromWSJTXCheckbox->setEnabled(false);
        logAutomaticallyWSJTXCheckbox->setEnabled(false);
        realDataFromWSJTXCheckbox->setEnabled(false);
        miliSecsSpinBox->setEnabled(false);
    }
 }

void SetupPageUDP::slotLogFromWSJTCheckBoxClicked()
{
     //qDebug() << "SetupPageUDP::slotLogFromWSJTCheckBoxClicked" << endl;
    if (logFromWSJTXCheckbox->isChecked())
    {
        logAutomaticallyWSJTXCheckbox->setEnabled(true);
    }
    else
    {
        logAutomaticallyWSJTXCheckbox->setEnabled(false);
        logAutomaticallyWSJTXCheckbox->setChecked(false);
    }
}


QString SetupPageUDP::getUDPServer()
{
    if (UDPServerCheckBox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

void SetupPageUDP::setUDPServer(const QString t)
{
     //qDebug() << "SetupPageUDP::setUDPServer: "  << t << endl;
    if ( (t.toUpper()) == "FALSE")
    {
        UDPServerCheckBox->setChecked(false);
    }
    else
    {
        UDPServerCheckBox->setChecked(true);
    }
    slotUDPServerCheckBoxClicked();

}

void SetupPageUDP::setUDPServerPort(const QString t)
{
    if (  (t.toInt()>=0) && (t.toInt()<=65535))
    {
        UDPServerPortSpinBox->setValue(t.toInt());
    }
    else
    {
        UDPServerPortSpinBox->setValue(defaultport);
    }
}

QString SetupPageUDP::getUDPServerPort()
{
    int t = UDPServerPortSpinBox->value();

    if (  (t>=0) && (t<=65535) )
    {
        return QString::number(t);
    }
    else
    {
        return QString::number(defaultport);
    }
}

void SetupPageUDP::setTimeout(const QString t)
{
    if (  (t.toInt()>=0) && (t.toInt()<=30000))
    {
        miliSecsSpinBox->setValue(t.toInt());
    }
    else
    {
        miliSecsSpinBox->setValue(defaultTimer);
    }

}

QString SetupPageUDP::getTimeout()
{
    int t = miliSecsSpinBox->value();

    if (  (t>=0) && (t<=30000) )
    {
        return QString::number(t);
    }
    else
    {
        return QString::number(defaultTimer);
    }
}

QString SetupPageUDP::getLogFromWSJTx()
{
    if (logFromWSJTXCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

QString SetupPageUDP::getAutoLogFromWSJTx()
{

    if (logAutomaticallyWSJTXCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

QString SetupPageUDP::getReaDataFromWSJTx()
{
    if (realDataFromWSJTXCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

void SetupPageUDP::setLogFromWSJTx(const QString t)
{
    if (t.toUpper() == "TRUE")
    {
        logFromWSJTXCheckbox->setChecked(true);
    }
    else
    {
        logFromWSJTXCheckbox->setChecked(false);
    }
    slotLogFromWSJTCheckBoxClicked();
}

void SetupPageUDP::setAutoLogFromWSJTx(const QString t)
{
    if (t.toUpper() == "TRUE")
    {
        logAutomaticallyWSJTXCheckbox->setChecked(true);
    }
    else
    {
        logAutomaticallyWSJTXCheckbox->setChecked(false);
    }
}

void SetupPageUDP::setReaDataFromWSJTx(const QString t)
{
    if (t.toUpper() == "TRUE")
    {
       realDataFromWSJTXCheckbox->setChecked(true);
    }
    else
    {
        realDataFromWSJTXCheckbox->setChecked(false);
    }
}

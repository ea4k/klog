#include "setuppages/setuppageinterfaceswindows.h"

SetupPageInterfacesWindows::SetupPageInterfacesWindows(QWidget *parent) : QWidget(parent)
{
    
    sendToPSTRotatorCheckBox = new QCheckBox(this);
    PSTRotatorUDPServerPortSpinBox = new QSpinBox;
    PSTRotatorServerLineEdit = new QLineEdit;
    PSTRotatorDefaultport = 12040;

    createUI();
    createActions();
}

SetupPageInterfacesWindows::~SetupPageInterfacesWindows(){
     //qDebug() << "SetupPageInterfacesWindows::~SetupPageInterfacesWindows" << endl;
}

void SetupPageInterfacesWindows::createUI()
{
    PSTRotatorServer = "localhost";
    setPSTRotatorUDPServer(PSTRotatorServer);

    QString labelTip = tr("UDP port number where the PSTRotator Server will listen for packets.") + "\n" + tr("Default port is 12040.");
    PSTRotatorUDPServerPortSpinBox->setToolTip(labelTip);

    PSTRotatorUDPServerPortSpinBox->setMinimum(0);
    PSTRotatorUDPServerPortSpinBox->setMaximum(65535);
    PSTRotatorUDPServerPortSpinBox->setValue(PSTRotatorDefaultport);

    QLabel *udpPortLabel = new QLabel(tr("PST Rotator UDP Port"));
    udpPortLabel->setBuddy(PSTRotatorUDPServerPortSpinBox);
    udpPortLabel->setToolTip(labelTip);
    udpPortLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    udpPortLabel->setEnabled(true);

    labelTip = tr("Hostname or IP address of the computer running PSTRotator. Leave it as localhost if you are running KLog and PSTRotator in the same computer.");
    PSTRotatorServerLineEdit->setToolTip(labelTip);
    QLabel *udpServerLabel = new QLabel(tr("PST Rotator host"));
    udpServerLabel->setBuddy(PSTRotatorServerLineEdit);
    udpServerLabel->setToolTip(labelTip);
    udpServerLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    udpServerLabel->setEnabled(true);

    QHBoxLayout *UDPLayout1 = new QHBoxLayout;
    UDPLayout1->addWidget(udpServerLabel);
    UDPLayout1->addWidget(PSTRotatorServerLineEdit);
    UDPLayout1->addWidget(udpPortLabel);
    UDPLayout1->addWidget(PSTRotatorUDPServerPortSpinBox);

    sendToPSTRotatorCheckBox->setText(tr("Send antenna bearing to PST Rotator"));
    sendToPSTRotatorCheckBox->setChecked(false);

    //PSTRotatorServerLineEdit->clear();
    //PSTRotatorServerLineEdit->setText(tr("Enter the hostname of the machine running PSTRotator. Enter localhost if it is the same machine."));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(sendToPSTRotatorCheckBox);
    layout->addLayout(UDPLayout1);

    setLayout(layout);
}

void SetupPageInterfacesWindows::createActions()
{
    connect(sendToPSTRotatorCheckBox, SIGNAL(clicked () ), this, SLOT(slotSendToPSTRotatorCheckBoxClicked() ) );
}

void SetupPageInterfacesWindows::setSendToPSTRotator(const QString &_t)
{
     //qDebug() << "SetupPageInterfacesWindows::setUDPServer: "  << t << endl;
    if ( (_t.toUpper()) == "FALSE")
    {
        sendToPSTRotatorCheckBox->setChecked(false);
    }
    else
    {
        sendToPSTRotatorCheckBox->setChecked(true);
    }
    slotSendToPSTRotatorCheckBoxClicked();
}

void SetupPageInterfacesWindows::setPSTRotatorUDPServerPort(const QString &_t)
{
    if (  (_t.toInt()>=0) && (_t.toInt()<=65535))
    {
        PSTRotatorUDPServerPortSpinBox->setValue(_t.toInt());
    }
    else
    {
        PSTRotatorUDPServerPortSpinBox->setValue(PSTRotatorDefaultport);
    }
}

void SetupPageInterfacesWindows::setPSTRotatorUDPServer(const QString &_t)
{
    PSTRotatorServer = _t;
    PSTRotatorServerLineEdit->setText(PSTRotatorServer);

}

QString SetupPageInterfacesWindows::getSendToPSTRotator()
{
    if (sendToPSTRotatorCheckBox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

QString SetupPageInterfacesWindows::getPSTRotatorUDPServerPort()
{
    int t = PSTRotatorUDPServerPortSpinBox->value();

    if (  (t>=0) && (t<=65535) )
    {
        return QString::number(t);
    }
    else
    {
        return QString::number(PSTRotatorDefaultport);
    }
}

QString SetupPageInterfacesWindows::getPSTRotatorUDPServer()
{
    return PSTRotatorServer;
}


void SetupPageInterfacesWindows::slotSendToPSTRotatorCheckBoxClicked()
{
     //qDebug() << "SetupPageInterfacesWindows::slotLogFromWSJTCheckBoxClicked" << endl;
    if (sendToPSTRotatorCheckBox->isChecked())
    {
        PSTRotatorUDPServerPortSpinBox->setEnabled(true);
        PSTRotatorServerLineEdit->setEnabled(true);
    }
    else
    {
        PSTRotatorUDPServerPortSpinBox->setEnabled(false);
        PSTRotatorServerLineEdit->setEnabled(false);
    }
}


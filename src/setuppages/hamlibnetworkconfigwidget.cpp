#include "hamlibnetworkconfigwidget.h"

HamLibNetworkConfigWidget::HamLibNetworkConfigWidget(QWidget *parent) : QWidget(parent)
{
    qDebug() << Q_FUNC_INFO;
    hostAddressLineEdit = new QLineEdit();
    portQSpinBox = new QSpinBox;
    qDebug() << Q_FUNC_INFO << " - END";

}

void HamLibNetworkConfigWidget::createUI()
{
    portQSpinBox->setMaximum (65535);
    portQSpinBox->setMinimum (0);
    hostAddressLineEdit->setToolTip (tr("Enter the hostname or address of the radio."));
    portQSpinBox->setToolTip (tr("Set de network port of the radio."));

    //portQSpinBox->setValue (rigctlport);
    QLabel *hostAddLabel = new QLabel(tr("Host/Address"));
    hostAddLabel->setBuddy(hostAddressLineEdit);
    hostAddLabel->setToolTip(tr("Enter the hostname or address of the radio."));
    hostAddLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    hostAddLabel->setEnabled(true);

    QLabel *portNetLabel = new QLabel(tr("Port"));
    portNetLabel->setBuddy(portQSpinBox);
    portNetLabel->setToolTip(tr("Enter the port of the radio."));
    portNetLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    portNetLabel->setEnabled(true);

    QGridLayout *networkLayout = new QGridLayout;
    //qDebug() << Q_FUNC_INFO << " - 120";
    networkLayout->addWidget (hostAddLabel, 0, 0);
    networkLayout->addWidget (hostAddressLineEdit, 0, 1);
    networkLayout->addWidget (portNetLabel, 1, 0);
    networkLayout->addWidget (portQSpinBox, 1, 1);

    //networkGroup->setLayout (networkLayout);
    setLayout (networkLayout);
    qDebug() << Q_FUNC_INFO << " - END";
}

int HamLibNetworkConfigWidget::getPort()
{
    return portQSpinBox->value ();
}

void HamLibNetworkConfigWidget::setPort (const int _b)
{//TODO: Check that
    portQSpinBox->setValue (_b);
}

QString HamLibNetworkConfigWidget::getAddress()
{
    return hostAddressLineEdit->text ();
}

void HamLibNetworkConfigWidget::setAddress(const QString &_st)
{ //TODO: Check that what is arriving is a hostname or IP address
    hostAddressLineEdit->setText (_st);
}

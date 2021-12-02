#ifndef HAMLIBSERIALCONFIGWIDGET_H
#define HAMLIBSERIALCONFIGWIDGET_H

#include <QtWidgets>
#include <QSerialPortInfo>
#include "klogdefinitions.h"

class HamLibSerialConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HamLibSerialConfigWidget(QWidget *parent = nullptr);
    void createUI();
    //void setRigList(const QStringList &_rigs);
    int getDataBits();
    void setDataBits (const int _b);
    QString getFlowControl();
    void setFlowControl(const QString &_st);
    void setParity(const QString &_st);
    QString getParity();
    void setStopBits(const QString &_st);
    int getStopBits();
    bool setSerialSpeed(const int _speed );
    bool setSerialPort(const QString &_port);

    QString getSerialPort();
    int getSerialBauds();

signals:

private slots:
    void slotScanPorts();

private:

    void setRig();
    void setDefaults();
    QStringList getAvailableSerialPorts();
    void fillSerialPortsComboBox();

    QPushButton *scanSerialPortButton;
    QComboBox *serialBaudsComboBox, *serialPortComboBox, *dataBitsComboBox, *flowControlComboBox, *parityComboBox, *stopBitsComboBox;
    QLineEdit *dataFromRigLineEdit;
    QLineEdit *serialPort;
    QStringList serialPorts;

};

#endif // HAMLIBSERIALCONFIGWIDGET_H

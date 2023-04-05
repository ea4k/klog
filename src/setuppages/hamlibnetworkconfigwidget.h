#ifndef HAMLIBNETWORKCONFIGWIDGET_H
#define HAMLIBNETWORKCONFIGWIDGET_H

#include <QtWidgets>
#include "../klogdefinitions.h"
#include "../utilities.h"

class HamLibNetworkConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HamLibNetworkConfigWidget(QWidget *parent = nullptr);
    void createUI();
    int getPort();
    void setPort (const int _b);
    QString getAddress();
    void setAddress(const QString &_st);

private:
    QLineEdit*hostAddressLineEdit;
    QSpinBox *portQSpinBox;
};

#endif // HAMLIBNETWORKCONFIGWIDGET_H

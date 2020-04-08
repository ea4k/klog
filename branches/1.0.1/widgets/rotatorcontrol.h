#ifndef ROTATORCONTROLWIDGET_H
#define ROTATORCONTROLWIDGET_H

#include <QWidget>
#include "rotator.h"


class RotatorControlWidget : public QWidget
{
    Q_OBJECT

public:
    RotatorControlWidget(QWidget *parent = nullptr);


protected:
    //void paintEvent(QPaintEvent *event) override;
    //virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
private:
    RotatorWidget *rotatorWidget;
};


#endif

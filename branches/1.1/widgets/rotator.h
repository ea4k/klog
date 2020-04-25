#ifndef ROTATORWIDGET_H
#define ROTATORWIDGET_H

#include <QWidget>
#include <QResizeEvent>


class RotatorWidget : public QWidget
{
    Q_OBJECT

public:
    RotatorWidget(QWidget *parent = nullptr);


protected:
    void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

};


#endif

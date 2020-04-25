#include <QtWidgets>
#include <QRect>

#include "rotator.h"


RotatorWidget::RotatorWidget(QWidget *parent) : QWidget(parent)

{
    //qDebug() << "RotatorWidget::RotatorWidget" << endl;
    QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    //timer->start(1000);
    setWindowTitle(tr("Rotator"));
    resize(200, 200);
}

void RotatorWidget::paintEvent(QPaintEvent *)
{
    //qDebug() << "RotatorWidget::paintEvent" << endl;
    static const QPoint hourHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -40)
    };
    static const QPoint minuteHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };

    QColor hourColor(127, 0, 127);
    QColor minuteColor(0, 127, 127, 191);

    int side = qMin(width(), height());
    QTime time = QTime::currentTime();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);

    // Paints the hour hand
    painter.save();
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();

    painter.setPen(hourColor);

    // Paints the hours lines
    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(minuteColor);

    painter.save();
    //Paints the minute hand
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();

    painter.setPen(minuteColor);
    painter.save();
    // Paints the minutes
    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0) // Skips the hour lines
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }
    painter.restore();
    QPointF point;
     //qDebug() << "RotatorWidget::paintEvent width: " << QString::number(width()) << endl;
          //qDebug() << "RotatorWidget::paintEvent height: " << QString::number(height()) << endl;
    point.setX(20);
    point.setY(20);
    painter.save();
    //painter.translate(width()/2, 8);
    painter.drawText(point, "0");

    painter.restore();

}

void RotatorWidget::resizeEvent(QResizeEvent *event)
{
    //qDebug() << "RotatorWidget::resizeEvent" << endl;
    event->accept();

    if(event->size().width() > event->size().height()){
        QWidget::resize(event->size().height(),event->size().height());
    }else{
        QWidget::resize(event->size().width(),event->size().width());
    }
}

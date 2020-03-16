/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QRect>

#include "rotator.h"


RotatorWidget::RotatorWidget(QWidget *parent) : QWidget(parent)

{
    QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    //timer->start(1000);
    setWindowTitle(tr("Rotator"));
    resize(200, 200);
}

void RotatorWidget::paintEvent(QPaintEvent *)

{
    if (width() != height())
    {
        return;
    }
    static const QPoint antennaDirection[3] = {
        QPoint(3, 4),
        QPoint(-3, 4),
        QPoint(0, -90)
    };

    QColor dirColor(Qt::blue);
    QColor antennaColor(Qt::red);

    int side = qMin(width(), height());
    QTime time = QTime::currentTime();
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    qDebug() << "RotatorWidget::paintEvent: " << QString::number(width()) << "/" << QString::number(height()) << endl;
    painter.scale(side / 200.0, side / 200.0);
    qDebug() << "RotatorWidget::paintEvent: scaled: " << QString::number(width()) << "/" << QString::number(height()) << endl;
    // Antenna
    painter.setPen(Qt::NoPen);
    painter.setBrush(antennaColor);

    painter.save();
    painter.rotate(6.0 * (time.second() / 60.0*60.0));
    painter.drawConvexPolygon(antennaDirection, 3);
    painter.restore();

    painter.setPen(dirColor);
    painter.save();
    for (int j = 0; j < 60; ++j) {

        if ((j % 5) != 0)
        {
            painter.drawLine(92, 0, 96, 0);
        }
        painter.rotate(6.0);
    }
    painter.restore();

    painter.save();

    for (int i = 0; i < 12; i++)
    {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }

    //painter.restore();
    painter.resetTransform();

    int x = (width()/4);// - (width()/3);
    int y = height()/4;


    painter.drawText(x, height()/2, "270" );
    //painter.drawText((width()/2)-(width()/3), 0, "90" );
    painter.drawText(width()/2, y , "0" );
    painter.drawText(width() /2, height()-y, "180" );
    painter.drawText(width() - x, height()/2, "90" );

  for (int i = 0; i< 4; i++)
  {

      painter.save();
      painter.rotate((i * 90) - 90);
      //painter.rotate(i * (270/4) + 135);
      painter.drawText(((width() -50)/2)-30, 0, QString::number((i * 90) ));
      painter.restore();
  }
}

void RotatorWidget::resizeEvent(QResizeEvent *event)
{
    event->accept();

    if(event->size().width() > event->size().height()){
        QWidget::resize(event->size().height(),event->size().height());
    }else{
        QWidget::resize(event->size().width(),event->size().width());
    }
}

/***************************************************************************
                          worldmapwidget.cpp  -  description
                             -------------------
    begin                : oct 2019
    copyright            : (C) 2019 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

#include "worldmapwidget.h"

WorldMapWidget::WorldMapWidget() // : mapLabel (new QLabel), scrollArea (new QScrollArea), scaleFactor(1)
{
       //qDebug() << "WorldMapWidget::WorldMapWidget"  << QT_ENDL;
    scrollArea = new QScrollArea;
    mapQImage = new QImage;

    setWindowTitle(tr("World map"));
    //setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    width = 0;
    height = 0;
    createActions();

    mapLabel = new QLabel();
    mapLabel->setBackgroundRole(QPalette::Base);
    mapLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    mapLabel->setScaledContents(true);

    //mapLabel->setPixmap(QPixmap::fromImage(*mapQImage));
    mapLabel->setWordWrap(true);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(mapLabel);
    scrollArea->setVisible(false);

    setCentralWidget(scrollArea);
    loadMap();
    //drawLocators();

    //QVBoxLayout *layout = new QVBoxLayout(this);
    //layout->setSizeConstraint(QLayout::SetFixedSize);
    //layout->addWidget(mapLabel);
    //setLayout(layout);

      //qDebug() << "WorldMapWidget::WorldMapWidget - END"  << QT_ENDL;
}

WorldMapWidget::~WorldMapWidget()
{
       //qDebug() << "WorldMapWidget::~WorldMapWidget"  << QT_ENDL;
}

void WorldMapWidget::loadMap()
{
      //qDebug() << "WorldMapWidget::loadMap"  << QT_ENDL;

    mapQImage->load(":/img/worldmap.j");
    *mapQImage = mapQImage->convertToFormat(QImage::Format_RGB32);
       //qDebug() << "WorldMapWidget::loadMap-10"  << QT_ENDL;

    //mapLabel->setPixmap(QPixmap::fromImage(*mapQImage));
    //mapLabel->setWordWrap(true);
      //qDebug() << "WorldMapWidget::loadMap-11"  << QT_ENDL;
    scaleFactor = 1.0;
      //qDebug() << "WorldMapWidget::loadMap-12"  << QT_ENDL;

    fitToWindowAct->setEnabled(true);
      //qDebug() << "WorldMapWidget::loadMap-13"  << QT_ENDL;
    updateActions();
      //qDebug() << "WorldMapWidget::loadMap-14"  << QT_ENDL;
   // mapNormalizeSize();

    if (!fitToWindowAct->isChecked())
    {
        mapLabel->adjustSize();
    }
      //qDebug() << "WorldMapWidget::loadMap-15"  << QT_ENDL;

    width = mapQImage->width();
    height = mapQImage->height();

    setImage(*mapQImage);
      //qDebug() << "WorldMapWidget::loadMap-Size: " << QString::number(mapQImage->width()) <<"/" << QString::number(mapQImage->height()) << QT_ENDL;




    //*mapQImage = mapQImage->scaledToWidth(1260);

     //qDebug() << "WorldMapWidget::loadMap-reSize: " << QString::number(mapQImage->width()) <<"/" << QString::number(mapQImage->height()) << QT_ENDL;

}

void WorldMapWidget::drawLocator(const int _x, const int _y, const int _width, const int _height, const bool _confirmed)
{
      //qDebug() << "WorldMapWidget::drawLocator: " << QString::number(_x) << QString::number(_y) << QString::number(_width) << QString::number(_height)  << QT_ENDL;
    QPainter qPainter(mapQImage);
    //qPainter.setBrush(Qt::Dense4Pattern);

    qPainter.setBrush(Qt::NoBrush);
    if (_confirmed)
    {
        qPainter.setPen(Qt::green);
    }
    else
    {
        qPainter.setPen(Qt::red);
    }
    QPen pen(Qt::green);
    pen.setWidth(1);
    qPainter.setPen(pen);

    qPainter.drawRect(_x,_y,_width,_height);

    qPainter.end();
    setImage(*mapQImage);
      //qDebug() << "WorldMapWidget::drawLocator-END"  << QT_ENDL;
}

void WorldMapWidget::drawLocatorText (const int _x, const int _y, const QString &loc, const bool _confirmed)
{
      //qDebug() << "WorldMapWidget::Text"  << QT_ENDL;
    QPainter qPainter(mapQImage);
    qPainter.setFont(QFont("Arial", 12, QFont::Bold));
    if (_confirmed)
    {
        qPainter.setPen(Qt::green);
    }
    else
    {
        qPainter.setPen(Qt::red);
    }

    qPainter.drawText(_x+2,_y-2, loc);
    qPainter.end();
    setImage(*mapQImage);
      //qDebug() << "WorldMapWidget::Text-END"  << QT_ENDL;

}

void WorldMapWidget::drawLocators()
{
      //qDebug() << "WorldMapWidget::drawLocators"  << QT_ENDL;
    if (width < 18 || height < 18)
    {
        return;
    }

    int x1, y1;

    for (int i = 0; i < 180; i++)
    {
        for (int j = 0; j < 180; j++)
        {
            x1 = (i * (width / 180));
            y1 = height - ((1+j) * (height / 180));
              //qDebug() << "WorldMapWidget::drawLocators: " << QString::number(x1) << "/" << QString::number(y1) << "/" <<QString::number(width/18) << "/" << QString::number(height/18)  << QT_ENDL;
            //drawLocator(x1, y1, width/180, height/180, false);
            drawLines(x1, y1);
            //drawLocatorText(x1, y1+(height/18), QString(QChar(i+'A')) + QString(QChar(j+'A')), false);
        }
    }
    setImage(*mapQImage);
      //qDebug() << "WorldMapWidget::drawLocators-END"  << QT_ENDL;
}

void WorldMapWidget::drawLines(const int _x, const int _y)
{
    QPainter qPainter(mapQImage);
    //qPainter.setBrush(Qt::Dense4Pattern);

    qPainter.setBrush(Qt::NoBrush);

    QPen pen(Qt::green, 1, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
    //pen.setWidth(1);
    qPainter.setPen(pen);
    // Horizontal
    qPainter.drawLine(0, _y, width, _y);
    qPainter.drawLine(_x, 0, _x, height);
    //qPainter.drawRect(_x,_y,_width,_height);

    qPainter.end();
    //setImage(*mapQImage);
}

void WorldMapWidget::slotZoomIn()
{
    scaleImage(1.25);
}

void WorldMapWidget::slotZoomOut()
{
    scaleImage(0.8);
}

void WorldMapWidget::slotFitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if(fitToWindow)
        slotNormalSize();
    updateActions();
}

void WorldMapWidget::slotNormalSize()
{
    mapLabel->adjustSize();
    scaleFactor = 1.0;
}

void WorldMapWidget::createActions()
{
    QMenu *viewMenu = menuBar()->addMenu(tr("View"));
    zoomInAct = viewMenu->addAction(tr("Zoom In(25%)"), this, &WorldMapWidget::slotZoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);

    zoomOutAct = viewMenu->addAction(tr("Zoom Out(25%)"), this, &WorldMapWidget::slotZoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);

    normalSizeAct = viewMenu->addAction(tr("Normal Size"), this, &WorldMapWidget::slotNormalSize);
    normalSizeAct->setEnabled(false);

    fitToWindowAct = viewMenu->addAction(tr("Fit to window"), this, &WorldMapWidget::slotFitToWindow);
    fitToWindowAct->setEnabled(false);

}

void WorldMapWidget::setImage (const QImage &newImage)
{
    *mapQImage = newImage;
    mapLabel->setPixmap(QPixmap::fromImage(*mapQImage));
    scaleFactor = 1.0;

    scrollArea->setVisible(true);
    fitToWindowAct->setEnabled(true);
    updateActions();
    if (!fitToWindowAct->isChecked())
        mapLabel->adjustSize();
}

void WorldMapWidget::updateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}


void WorldMapWidget::scaleImage(double factor)
{
    //Q_ASSERT(mapLabel->pixmap(Qt::ReturnByValue));
    scaleFactor *= factor;

    #if (QT_VERSION>=0x051202)
    mapLabel->resize(scaleFactor * mapLabel->pixmap(Qt::ReturnByValue).size());
    #else
    mapLabel->resize(scaleFactor * mapLabel->pixmap()->size());
    #endif

    //mapLabel->resize(scaleFactor * mapLabel->pixmap(QT_RETURNBYVALUE)->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);
    zoomInAct->setEnabled(scaleFactor < 15.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void WorldMapWidget::adjustScrollBar(QScrollBar *scrollbar, double factor)
{
    scrollbar->setValue(int(factor * scrollbar->value() + ((factor -1) * scrollbar->pageStep()/2)));
}

QRect WorldMapWidget::getScreenResolution()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    return screen->geometry();
}

void WorldMapWidget::mapNormalizeSize()
{
    int _xMax = (getScreenResolution()).height();

    int mapWidth;
    /*
    if (_xMax < 640)
    {
        mapWidth = 360;
    }
    else if ((_xMax >= 640) && (_xMax < 1024))
    {
        mapWidth = 450;
    }
    else if ((_xMax >= 1024) && (_xMax < 1440))
    {
        mapWidth = 810;
    }
    else if ((_xMax >= 1440) && (_xMax < 2048))
    {
        mapWidth = 1080;
    }
    else
    {
        mapWidth = 1350;
    }
    */

    if (_xMax < 600)
    {
        mapWidth = 360;
    }
    else if ((_xMax >= 600) && (_xMax < 960))
    {
        mapWidth = 450;
    }
    else if ((_xMax >= 960) && (_xMax < 1080))
    {
        mapWidth = 810;
    }
    else if ((_xMax >= 1080) && (_xMax < 1536))
    {
        mapWidth = 1080;
    }
    else
    {
        mapWidth = 1350;
    }

    *mapQImage = mapQImage->scaledToHeight(mapWidth);

      //qDebug() << "WorldMapWidget::mapNormalizeSize: Screen: " << QString::number(_xMax)  << QT_ENDL;
      //qDebug() << "WorldMapWidget::mapNormalizeSize: Map: " << QString::number(mapWidth)  << QT_ENDL;
}

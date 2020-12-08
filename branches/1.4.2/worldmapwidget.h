#ifndef WORLDMAPWIDGET_H
#define WORLDMAPWIDGET_H

/***************************************************************************
                          worldmapwidget.h  -  description
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/
#include <QtWidgets>
#include <QWidget>
//#include <QTextBrowser>

class QMenu;
class QScrollArea;
class QScrollBar;
class QAction;
class QLabel;

class WorldMapWidget: public QMainWindow
{
    Q_OBJECT

public:
    WorldMapWidget();
    ~WorldMapWidget();
    void loadMap();
    //void setText(const QString txt);

private slots:
    //void slotAcceptButtonClicked();
    void slotZoomIn();
    void slotZoomOut();
    void slotNormalSize();
    void slotFitToWindow();

private:
    //void loadMap();
    void drawLocators();
    void drawLocator(const int _x, const int _y, const int _width,const int _height, const bool _confirmed);
    void drawLocatorText (const int _x, const int _y, const QString &loc, const bool _confirmed);
    void drawLines(const int _x, const int _y);
    void setImage (const QImage &newImage);
    void updateActions();
    void createActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollbar, double factor);
    QRect getScreenResolution();
    void mapNormalizeSize();
    //void keyPressEvent(QKeyEvent *event);
     //QTextBrowser *textBrowser;
     //QString text;
    QScrollArea *scrollArea;
    QLabel *mapLabel;
    QImage *mapQImage;
    int width, height;
    double scaleFactor;

    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *fitToWindowAct;
    QAction *normalSizeAct;


};


#endif // WorldMapWidget_H

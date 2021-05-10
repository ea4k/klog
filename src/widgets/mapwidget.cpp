/***************************************************************************
                           mapwidget.cpp  -  description
                             -------------------
    begin                : May 2021
    copyright            : (C) 2021 by Jaime Robles
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
#include "mapwidget.h"
#include <QQuickView>

MapWidget::MapWidget()
{
    testButton = new QPushButton;
    testButton->setText ("Push");


    qmlView.setSource(QUrl(QStringLiteral("qrc:qml/mapqmlfile.qml")));
    qmlView.setResizeMode(QQuickView::SizeRootObjectToView);
    QWidget *container = QWidget::createWindowContainer(&qmlView, this);

    //setGeometry (container->geometry ());

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget (testButton);
    layout->addWidget(container);
    setLayout (layout);

    setMinimumSize (200, 200); //This minimum size may be relative to another widget... (maybe the mainwindow?)
    connect(testButton, SIGNAL(clicked()), this, SLOT(slotButtonClicked() ) );
}

void MapWidget::setCenter(const double lat, const double lon)
{

    QObject *object = qmlView.rootObject ();
    //object->setProperty ("zoom", 1);
    object->setProperty ("lat", lat);
    object->setProperty ("lon", lon);

}

void MapWidget::addLocator(const double lat1, const double lon1, const double lat2, const double lon2)
{
    QObject *object = qmlView.rootObject ();
    //MapRectangle
    object->setProperty ("locLat1", lat1);
    object->setProperty ("locLon1", lon1);
    object->setProperty ("locLat2", lat2);
    object->setProperty ("locLon2", lon2);
}

void MapWidget::slotButtonClicked ()
{
    qDebug() << "MapWidget::slotButtonClicked " << endl;

    double latitude = 43.2;
    double longitude = -4.816669;

    QVariant returnedValue;
    QObject *object = qmlView.rootObject ();
    QMetaObject::invokeMethod(object, "addLoc",
           Q_RETURN_ARG(QVariant, returnedValue),
           Q_ARG(QVariant, latitude), Q_ARG(QVariant, longitude));
       qDebug() << "QML function returned:" << returnedValue.toString();


}

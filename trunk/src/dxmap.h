#ifndef DXMAP_H
#define DXMAP_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QHash>
#include <QMap>

#include "dxspot.h"

typedef QHash<int, DxSpot> DXSpotList;

#define SPOTS_TO_DISPLAY 50

class DXMap : public QWidget
{
   Q_OBJECT
public:
   DXMap( QWidget *parent=0 );

public slots:
   void plotSpot(DxSpot);
   void plot();

protected:
   void paintEvent( QPaintEvent * );
   QColor getFreqColour(qreal);

private:
//   QPainter painter;
   int numberOfSpots; // a count of spots in the hash
   QPixmap mapImage;
   // Number of spots to display on the map at ny one time
   int spotsToDisplay;
   // The size of the window
   qreal imageX;
   qreal imageY;
   bool allSpots;
   // A list of the coordinates
   DXSpotList spotList, *dxSpotList;
   QHash<int, DxSpot> rspot;
   QMap<int, QString> xxx;

   int displayType;
   DXMap *dxMap;
   // The high and low values for a frequency to be displayed on the map
   qreal spotFrequencyLow;
   qreal spotFrequencyHigh;

};
#endif

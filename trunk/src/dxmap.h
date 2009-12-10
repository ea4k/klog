#ifndef DXMAP_H
#define DXMAP_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QProgressDialog>
#include "dxspot.h"

#define SPOTS_TO_DISPLAY 50
typedef QHash<int, DxSpot> DXSpotList;

class DXMap : public QWidget
{
   Q_OBJECT
public:
   DXMap( QWidget *parent=0 );

public slots:
   void plotSpot(DXSpotList &);
   void plotSpot(DXSpotList *);
   void SetSpotsToDisplay(int);
   void AllSpotsDisplay(bool);
   void ToSpots(QStringList &);
   void FromSpots(QStringList &);

protected:
   void paintEvent( QPaintEvent * );
   QColor getFreqColour(qreal);

private:
//   QPainter painter;
   QPixmap mapImage;
   // Number of spots to display on the map at ny one time
   int spotsToDisplay;
   // The size of the window
   qreal imageX;
   qreal imageY;
   bool allSpots;
   // A list of the coordinates
   DXSpotList *spotList;
   // The list of locations we want to view spots from / to
   QStringList *toSpots;
   QStringList *fromSpots;
};
#endif

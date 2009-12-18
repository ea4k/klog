#ifndef DXMAP_H
#define DXMAP_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QProgressDialog>
#include "world.h"
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
   void plotSpot(QString, QString, QString);
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
   World *world;
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

   bool LOADING; // used for loading data from file

   Entity spotEntity;
   Entity loggingEntity;
   QString myQRA;
   int displayType;
   DXMap *dxMap;
   QFile logfile;
   QString QRA;
   QString userName;
   // Spots in the 'View Spots To' list
   DXSpotList *dxSpotList;
   // test a new way of handling to and from spots, we'll work with to spots first.
   DXSpotList *dxToFromSpots;
   // a count of how many spots there are
   int dxLineCount;
   // The high and low values for a frequency to be displayed on the map
   qreal spotFrequencyLow;
   qreal spotFrequencyHigh;

};
#endif

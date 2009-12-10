#include "dxmap.h"
#define DEBUG 1
#ifdef DEBUG
#include <QDebug>
#endif

DXMap::DXMap( QWidget *parent ) : QWidget( parent ) {
   // this is connected from the parent widget which will emit a signal whenever the
   // to/from list is changed.
   setAttribute(Qt::WA_StaticContents);
   spotsToDisplay = SPOTS_TO_DISPLAY;
   mapImage = QPixmap(":/images/map.png");
   allSpots = FALSE;
   spotList = new DXSpotList;
   toSpots = new QStringList;
   fromSpots = new QStringList;
}

void DXMap::paintEvent( QPaintEvent * ) {
   QPainter painter( this );
   QPen pen(Qt::gray, 1);
   int cnt, spots, n;

   painter.setRenderHint(QPainter::Antialiasing);
   painter.drawPixmap(painter.window(), mapImage);
   painter.translate(width() / 2, height() / 2);
   imageX = painter.window().size().width();
   imageY = painter.window().size().height();

   // if sort out how many spots to display and where to start the list
   if(spotList->size() <= spotsToDisplay || allSpots){
      spots = 0;
   } else {
      spots = spotList->size() - spotsToDisplay;
   }
   for(cnt=spots ; cnt<spotList->size() ; cnt++) {
      DxSpot list = spotList->value(cnt);
/*      qreal spotX = list.SLong() * imageX / 2 / 180;
      qreal spotY = list.SLat() * imageY / 2 / 90;
      qreal logX = list.LLong() * imageX / 2 / 180;
      qreal logY = list.LLat() * imageY / 2 / 90;
      QPointF spotpoint(-spotX,-spotY);
      QPointF logpoint(-logX,-logY);
      pen.setColor(getFreqColour(list.Frequency()));
      painter.setPen(pen);
      painter.drawLine(spotpoint,logpoint);
 */
   }
}

void DXMap::plotSpot(DXSpotList &slist){
   spotList = &slist;
   update();
}

void DXMap::plotSpot(DXSpotList *slist){
   spotList = slist;
   update();
}

void DXMap::SetSpotsToDisplay(int spots){
   spotsToDisplay = spots;
   update();
}

void DXMap::AllSpotsDisplay(bool state){
   allSpots = state;
   update();
}

void DXMap::ToSpots(QStringList &list){
   toSpots = &list;
   update();
}

void DXMap::FromSpots(QStringList &list){
   fromSpots = &list;
   update();
}

QColor DXMap::getFreqColour(qreal frequency){
/*   // 160M
   if(frequency >= LOW_160M && frequency <= HIGH_160M)
      return Qt::white;
   // 80M
   if(frequency >= LOW_80M && frequency <= HIGH_80M)
      return Qt::green;
   // 40M
   if(frequency >= LOW_40M && frequency <= HIGH_40M)
      return Qt::blue;
   // 30M
   if(frequency >= LOW_30M && frequency <= HIGH_30M)
      return Qt::magenta;
   // 20M
   if(frequency >= LOW_20M && frequency <= HIGH_20M)
      return Qt::red;
   // 17M
   if(frequency >= LOW_17M && frequency <= HIGH_17M)
      return Qt::yellow;
   // 15M
   if(frequency >= LOW_15M && frequency <= HIGH_15M)
      return Qt::cyan;
   // 12M
   if(frequency >= LOW_12M && frequency <= HIGH_12M)
      return Qt::darkYellow;
   // 10M
   if(frequency >= LOW_10M && frequency <= HIGH_10M)
      return Qt::darkCyan;
   // 6M
   if(frequency >= LOW_6M && frequency <= HIGH_6M)
      return Qt::darkMagenta;
   // 2M
   if(frequency >= LOW_2M && frequency <= HIGH_2M)
      return Qt::darkBlue;
*/
   return Qt::black;
}

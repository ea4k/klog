/***************************************************************************
                          infowidget.h  -  description
                             -------------------
    begin                : ago 2017
    copyright            : (C) 2017 by Jaime Robles
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

#include "infowidget.h"

InfoWidget::InfoWidget(DataProxy_SQLite *dp, QWidget *parent) :
    QWidget(parent)
{
       //qDebug() << "InfoWidget::InfoWidget: "  ;
    dataProxy = dp;
    awards = new Awards(dataProxy, Q_FUNC_INFO); //Just to know colors

    locator = new Locator();
    world = new World(dataProxy, Q_FUNC_INFO);

    bandLabel1 = new QLabel;
    bandLabel2 = new QLabel;
    bandLabel3 = new QLabel;
    bandLabel4 = new QLabel;
    bandLabel5 = new QLabel;
    bandLabel6 = new QLabel;
    bandLabel7 = new QLabel;
    bandLabel8 = new QLabel;
    bandLabel9 = new QLabel;
    bandLabel10 = new QLabel;
    bandLabel11 = new QLabel;
    bandLabel12 = new QLabel;

    continentLabel = new QLabel;
    prefixLabel = new QLabel;
    cqzLabel = new QLabel;
    ituzLabel = new QLabel;
    gradShortLabel = new QLabel;
    distShortLabel = new QLabel;
    gradLongLabel = new QLabel;
    distLongLabel = new QLabel;

    distShortLabelN = new QLabel;
    distLongLabelN = new QLabel;

    imperialSystem=false;
    dxLocator.clear();

    createUI();
    clearBandLabels();
       //qDebug() << "InfoWidget::InfoWidget: - END"  ;
}

InfoWidget::~InfoWidget()
{
    delete(awards);
    delete(locator);
    delete(world);
}

void InfoWidget::createUI()
{
    bandLabel1->setText(tr("10M"));
    bandLabel2->setText(tr("15M"));
    bandLabel3->setText(tr("20M"));
    bandLabel4->setText(tr("40M"));
    bandLabel5->setText(tr("80M"));
    bandLabel6->setText(tr("160M"));
    bandLabel7->setText(tr("2M"));
    bandLabel8->setText(tr("6M"));
    bandLabel9->setText(tr("12M"));
    bandLabel10->setText(tr("17M"));
    bandLabel11->setText(tr("30M"));
    bandLabel12->setText(tr("70CM"));

    bandLabel1->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel2->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel3->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel4->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel5->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel6->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel7->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel8->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel9->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel10->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel11->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel12->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *continentLabelN = new QLabel(tr("Continent"));
    continentLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    continentLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *prefixLabelN = new QLabel(tr("Prefix"));
    prefixLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    prefixLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *cqzLabelN = new QLabel(tr("CQ"));
    cqzLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    cqzLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *ituzLabelN = new QLabel(tr("ITU"));
    ituzLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    ituzLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *shortLabelN = new QLabel(tr("Short Path"));
    shortLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *longLabelN = new QLabel(tr("Long Path"));
    longLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *gradShortLabelN = new QLabel(tr("Deg"));
    gradShortLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    gradShortLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    distShortLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *gradLongLabelN = new QLabel(tr("Deg"));
    gradLongLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    gradLongLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    distShortLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    distLongLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    distLongLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    clear();


    //continentLabel->setText("--");
    //prefixLabel->setText("--");
    //cqzLabel->setText("0");
    //ituzLabel->setText("0");
    //gradShortLabel->setText("0");
    //gradLongLabel->setText("0");
    //distLongLabel->setText("0");
    //distShortLabel->setText("0");

    QGridLayout *infoLayout1 = new QGridLayout;
    infoLayout1->addWidget(continentLabelN, 0, 0);
    infoLayout1->addWidget(continentLabel, 1, 0);
    infoLayout1->addWidget(prefixLabelN, 0, 1);
    infoLayout1->addWidget(prefixLabel, 1, 1);
    infoLayout1->addWidget(cqzLabelN, 0, 2);
    infoLayout1->addWidget(cqzLabel, 1, 2);
    infoLayout1->addWidget(ituzLabelN, 0, 3);
    infoLayout1->addWidget(ituzLabel, 1, 3);

    QGridLayout *shortPathLayout = new QGridLayout;
    shortPathLayout->addWidget(shortLabelN, 0, 0, 1, 0);
    shortPathLayout->addWidget(gradShortLabelN, 1, 0);
    shortPathLayout->addWidget(gradShortLabel, 1, 1);
    shortPathLayout->addWidget(distShortLabelN, 1, 2);
    shortPathLayout->addWidget(distShortLabel, 1, 3);

    QGridLayout *longPathLayout = new QGridLayout;
    longPathLayout->addWidget(longLabelN, 0, 0, 1, 0);
    longPathLayout->addWidget(gradLongLabelN, 1, 0);
    longPathLayout->addWidget(gradLongLabel, 1, 1);
    longPathLayout->addWidget(distLongLabelN, 1, 2);
    longPathLayout->addWidget(distLongLabel, 1, 3);

    QHBoxLayout *pathsLayout = new QHBoxLayout;
    pathsLayout->addLayout(shortPathLayout);
    pathsLayout->addLayout(longPathLayout);

    QGridLayout *bandsLayout = new QGridLayout;
    bandsLayout->addWidget(bandLabel1, 0, 0);
    bandsLayout->addWidget(bandLabel2, 0, 1);
    bandsLayout->addWidget(bandLabel3, 0, 2);
    bandsLayout->addWidget(bandLabel4, 0, 3);
    bandsLayout->addWidget(bandLabel5, 0, 4);
    bandsLayout->addWidget(bandLabel6, 0, 5);
    bandsLayout->addWidget(bandLabel7, 1, 0);
    bandsLayout->addWidget(bandLabel8, 1, 1);
    bandsLayout->addWidget(bandLabel9, 1, 2);
    bandsLayout->addWidget(bandLabel10, 1, 3);
    bandsLayout->addWidget(bandLabel11, 1, 4);
    bandsLayout->addWidget(bandLabel12, 1, 5);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(bandsLayout);
    mainLayout->addLayout(infoLayout1);
    mainLayout->addLayout(pathsLayout);

#ifdef Q_OS_WIN
    continentLabel->setFrameShadow(QFrame::Raised);
    continentLabel->setFrameStyle(QFrame::StyledPanel);
    continentLabelN->setFrameShadow(QFrame::Raised);
    continentLabelN->setFrameStyle(QFrame::StyledPanel);
    prefixLabelN->setFrameShadow(QFrame::Raised);
    prefixLabelN->setFrameStyle(QFrame::StyledPanel);
    prefixLabel->setFrameShadow(QFrame::Raised);
    prefixLabel->setFrameStyle(QFrame::StyledPanel);
    cqzLabelN->setFrameShadow(QFrame::Raised);
    cqzLabelN->setFrameStyle(QFrame::StyledPanel);
    cqzLabel->setFrameShadow(QFrame::Raised);
    cqzLabel->setFrameStyle(QFrame::StyledPanel);
    ituzLabel->setFrameShadow(QFrame::Raised);
    ituzLabel->setFrameStyle(QFrame::StyledPanel);
    ituzLabelN->setFrameShadow(QFrame::Raised);
    ituzLabelN->setFrameStyle(QFrame::StyledPanel);
    shortLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    shortLabelN->setFrameShadow(QFrame::Raised);
    shortLabelN->setFrameStyle(QFrame::StyledPanel);

    longLabelN->setFrameShadow(QFrame::Raised);
    longLabelN->setFrameStyle(QFrame::StyledPanel);
    gradShortLabelN->setFrameShadow(QFrame::Raised);
    gradShortLabelN->setFrameStyle(QFrame::StyledPanel);
    gradShortLabel->setFrameShadow(QFrame::Raised);
    gradShortLabel->setFrameStyle(QFrame::StyledPanel);
    distShortLabelN->setFrameShadow(QFrame::Raised);
    distShortLabelN->setFrameStyle(QFrame::StyledPanel);
    distShortLabel->setFrameShadow(QFrame::Raised);
    distShortLabel->setFrameStyle(QFrame::StyledPanel);
    gradLongLabelN->setFrameShadow(QFrame::Raised);
    gradLongLabelN->setFrameStyle(QFrame::StyledPanel);
    gradLongLabel->setFrameShadow(QFrame::Raised);
    gradLongLabel->setFrameStyle(QFrame::StyledPanel);
    distLongLabelN->setFrameShadow(QFrame::Raised);
    distLongLabelN->setFrameStyle(QFrame::StyledPanel);
    distLongLabel->setFrameShadow(QFrame::Raised);
    distLongLabel->setFrameStyle(QFrame::StyledPanel);


    bandLabel1->setFrameShadow(QFrame::Raised);
    bandLabel1->setFrameStyle(QFrame::StyledPanel);
    bandLabel2->setFrameShadow(QFrame::Raised);
    bandLabel2->setFrameStyle(QFrame::StyledPanel);
    bandLabel3->setFrameShadow(QFrame::Raised);
    bandLabel3->setFrameStyle(QFrame::StyledPanel);
    bandLabel4->setFrameShadow(QFrame::Raised);
    bandLabel4->setFrameStyle(QFrame::StyledPanel);
    bandLabel5->setFrameShadow(QFrame::Raised);
    bandLabel5->setFrameStyle(QFrame::StyledPanel);
    bandLabel6->setFrameShadow(QFrame::Raised);
    bandLabel6->setFrameStyle(QFrame::StyledPanel);

    bandLabel7->setFrameShadow(QFrame::Raised);
    bandLabel7->setFrameStyle(QFrame::StyledPanel);
    bandLabel8->setFrameShadow(QFrame::Raised);
    bandLabel8->setFrameStyle(QFrame::StyledPanel);
    bandLabel9->setFrameShadow(QFrame::Raised);
    bandLabel9->setFrameStyle(QFrame::StyledPanel);
    bandLabel10->setFrameShadow(QFrame::Raised);
    bandLabel10->setFrameStyle(QFrame::StyledPanel);
    bandLabel11->setFrameShadow(QFrame::Raised);
    bandLabel11->setFrameStyle(QFrame::StyledPanel);
    bandLabel12->setFrameShadow(QFrame::Raised);
    bandLabel12->setFrameStyle(QFrame::StyledPanel);

#else
    continentLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    continentLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    prefixLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    prefixLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    cqzLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    cqzLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    ituzLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    ituzLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    shortLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    longLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    gradShortLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    gradShortLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    distShortLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    distShortLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    gradLongLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    distLongLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    distLongLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    gradLongLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

    bandLabel1->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel3->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel4->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel5->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel6->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel7->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel8->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel9->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel10->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel11->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel12->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
#endif

    setLayout(mainLayout);
}

void InfoWidget::clearBandLabels()
{
    QString defaultColorName = (awards->getDefaultColor()).name(QColor::HexRgb);
    bandLabel1->setStyleSheet("* { background-color: " + defaultColorName + "; }");
    bandLabel2->setStyleSheet("* { background-color: " + defaultColorName + "; }");
    bandLabel3->setStyleSheet("* { background-color: " + defaultColorName + "; }");
    bandLabel4->setStyleSheet("* { background-color: " + defaultColorName + "; }");
    bandLabel5->setStyleSheet("* { background-color: " + defaultColorName + "; }");
    bandLabel6->setStyleSheet("* { background-color: " + defaultColorName + "; }");
    bandLabel7->setStyleSheet("* { background-color: " + defaultColorName + "; }");
    bandLabel8->setStyleSheet("* { background-color: " + defaultColorName + "; }");
    bandLabel9->setStyleSheet("* { background-color: " + defaultColorName + "; }");
    bandLabel10->setStyleSheet("* { background-color: " + defaultColorName + "; }");
    bandLabel11->setStyleSheet("* { background-color: " + defaultColorName + "; }");
    bandLabel12->setStyleSheet("* { background-color: " + defaultColorName + "; }");
}


void InfoWidget::clearInfoFromLocators()
{
       //qDebug() << "InfoWidget::clearInfoFromLocators";
    gradShortLabel->setText( "0" );
    gradLongLabel->setText( "0" );
    distShortLabel->setText( "0" );
    distLongLabel->setText( "0" );
    cqzLabel->setText("0");
    ituzLabel->setText("0");
}

void InfoWidget::clear()
{
    continentLabel->setText("--");
    prefixLabel->setText("--");
    clearBandLabels();
    clearInfoFromLocators();
}

void InfoWidget::setColors (const QString &_newOne, const QString &_needed, const QString &_worked, const QString &_confirmed, const QString &_default)
{
    awards->setColors (_newOne, _needed, _worked, _confirmed, _default);
    clearBandLabels();
}

void InfoWidget::setCurrentLog(const int _log)
{
    currentLog = _log;
}

void InfoWidget::setImperialSystem (const  bool _imp)
{
    imperialSystem = _imp;

    if (imperialSystem)
    {
        distShortLabelN->setText(tr("Miles"));
        distLongLabelN->setText(tr("Miles"));
        //distShortLabel->setText( QString::number( Km2Mile(imperialSystem, (distShortLabel->text()).toInt() )) );
        //distLongLabel->setText( QString::number(Km2Mile(imperialSystem, (distLongLabel->text()).toInt()) ) );
    }
    else
    {
        distShortLabelN->setText(tr("Km"));
        distLongLabelN->setText(tr("Km"));
    }
}

QString InfoWidget::getStyleColorToLabelFromBand(const int _bandId, const int _entityId)
{ // Receives band name, Entity number (as a String)
   //qDebug() << Q_FUNC_INFO << ": " << _b << "/" << _q;
    EntityStatus _entityStatus;
    _entityStatus.entityId  = _entityId;
    _entityStatus.bandId    = _bandId;
    _entityStatus.modeId    = -1;
    _entityStatus.log       = currentLog;

    //TODO: Check if we can know the mode and replace the -1
    //qDebug() << Q_FUNC_INFO << ": (Band/background-color): " << _b << (awards->getQRZDXStatusColor(_qs)).name(QColor::HexRgb) ;
    return "* { background-color: " + (awards->getQRZDXStatusColor(_entityStatus)).name(QColor::HexRgb) + "; }";
}


//void InfoWidget::showInfo(const int _entity, const int _bandid, const int _modeid, const int _log)
void InfoWidget::showInfo(const int _entity)
{ // Default values of _modeid & _log = -1
    //qDebug() << Q_FUNC_INFO << " - Start";
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_entity);
    //QColor getQRZDXStatusColor(const QStringList _qs); // Receives Entity, band, mode & log
    if (_entity <1)
    {
        //qDebug() << Q_FUNC_INFO << ": entity <1";
        clearBandLabels();
    }

    bandLabel1->setStyleSheet(getStyleColorToLabelFromBand(dataProxy->getIdFromBandName(bandLabel1->text()), _entity));
    bandLabel2->setStyleSheet(getStyleColorToLabelFromBand(dataProxy->getIdFromBandName(bandLabel2->text()), _entity));
    bandLabel3->setStyleSheet(getStyleColorToLabelFromBand(dataProxy->getIdFromBandName(bandLabel3->text()), _entity));
    bandLabel4->setStyleSheet(getStyleColorToLabelFromBand(dataProxy->getIdFromBandName(bandLabel4->text()), _entity));
    bandLabel5->setStyleSheet(getStyleColorToLabelFromBand(dataProxy->getIdFromBandName(bandLabel5->text()), _entity));
    bandLabel6->setStyleSheet(getStyleColorToLabelFromBand(dataProxy->getIdFromBandName(bandLabel6->text()), _entity));
    bandLabel7->setStyleSheet(getStyleColorToLabelFromBand(dataProxy->getIdFromBandName(bandLabel7->text()), _entity));
    bandLabel8->setStyleSheet(getStyleColorToLabelFromBand(dataProxy->getIdFromBandName(bandLabel8->text()), _entity));
    bandLabel9->setStyleSheet(getStyleColorToLabelFromBand(dataProxy->getIdFromBandName(bandLabel9->text()), _entity));
    bandLabel10->setStyleSheet(getStyleColorToLabelFromBand(dataProxy->getIdFromBandName(bandLabel10->text()), _entity));
    bandLabel11->setStyleSheet(getStyleColorToLabelFromBand(dataProxy->getIdFromBandName(bandLabel11->text()), _entity));
    bandLabel12->setStyleSheet(getStyleColorToLabelFromBand(dataProxy->getIdFromBandName(bandLabel12->text()), _entity));
    //qDebug() << Q_FUNC_INFO << " - END";
}

void InfoWidget::showEntityInfo(const int _enti, int _cq, int _itu)
{
    //qDebug() << "InfoWidget::showEntityInfo" << QString::number(_enti);

    if (_enti<=0)
    {
        return;
    }

/* TO paint a flag of the Worked entity
    QString flagSt;
    flagSt.clear();
    QString aux;
    aux = dataProxy->getISOName(_enti);
    if (aux.length()>1)
    {
        flagSt = ":/" + aux + ".png";
    }
    else
    {
        flagSt.clear();
    }

    flagSt = ":/flags/" + dataProxy->getISOName(_enti) + ".png";
    flagIcon->setIcon(QIcon(flagSt));
*/

    //infoLabel2->setText(world->getEntityName(_enti));
    continentLabel->setText( world->getContinentShortName(_enti) );
    prefixLabel->setText( world->getEntityMainPrefix(_enti));

    int i = -1;

    if ((_cq>0) && (_cq<41))
    {
        cqzLabel->setText( QString::number(_cq) );
    }
    else
    {
        i = world->getEntityCqz(_enti);
        if( i > 0 )
        {
            cqzLabel->setText( QString::number(i) );
        }
        else
        {
            cqzLabel->setText("0");
        }
    }

    if (_itu>0)
    {
        ituzLabel->setText( QString::number(_itu) );
    }
    else
    {
        i = world->getEntityItuz(_enti);
        if ( i > 0  )
        {
            ituzLabel->setText( QString::number(i) );
        }
        else
        {
            ituzLabel->setText("0");
        }
    }
}


void InfoWidget::showDistanceAndBearing(const QString &_locLocal, const QString &_locDX)
{// Local / DX
       //qDebug() << "InfoWidget::showDistanceAndBearing: " << _locLocal << "/" << _locDX;
    QString lloc = _locLocal.toUpper();
    QString ldx = _locDX.toUpper();

    if ( locator->isValidLocator(lloc)  )
    {
        if ( locator->isValidLocator(ldx)  )
        {
            dxLocator = ldx;

            int beam = locator->getBeamBetweenLocators(lloc, dxLocator);


            gradShortLabel->setText( QString::number(beam) );

             if (beam >= 180)
             {
                   gradLongLabel->setText( QString::number(beam -180 ) );
             }
             else
             {
                 gradLongLabel->setText( QString::number(beam + 180 ) );
             }

             distShortLabel->setText( QString::number( locator->getDistanceBetweenLocators(lloc, dxLocator, imperialSystem) ) );
             distLongLabel->setText( QString::number( 40000 - locator->getDistanceBetweenLocators(lloc, dxLocator, imperialSystem) ) );
        }
        else
        {
            clearInfoFromLocators();
            return;
        }
    }
    else
    {
        clearInfoFromLocators();
        return ;
    }
}

void InfoWidget::setLocalLocator(const QString &_loc)
{
    if (locator->isValidLocator(_loc))
    {
        localLocator = _loc;
    }
}

void InfoWidget::setDXLocator(const QString &_loc)
{
    if (locator->isValidLocator(_loc))
    {
        dxLocator = _loc;
    }
}

int InfoWidget::getDistance(bool shortPath)
{ // Returns the distance in KM
    //qDebug() << Q_FUNC_INFO << distShortLabel->text ();
    if (shortPath)
    {
        return (distShortLabel->text ()).toInt ();
    }
    else
    {
        return (distLongLabel->text ()).toInt ();
    }
}

int InfoWidget::getCQ()
{ // Returns the CQ zone shown in the widget
    return (cqzLabel->text()).toInt();
}

int InfoWidget::getITU()
{ // Returns the CQ zone shown in the widget
    return (ituzLabel->text()).toInt();
}


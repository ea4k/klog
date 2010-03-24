/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : sb dic  7 18:42:45 CET 2002
    copyright            : (C) 2002 by Jaime Robles
    email                : jaime@kde.org
 ***************************************************************************/
/******************************************************************************
*                                                                            *
*   This program is free software; you can redistribute it and/or modify     *
*  it under the terms of the GNU General Public License as published by     *
*  the Free Software Foundation; either version 2 of the License, or        *
*  (at your option) any later version.                                      *
*                                                                            *
*  This program is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
*  GNU General Public License for more details.                             *
 *                                                                            *
*  You should have received a copy of the GNU General Public License        *
*  along with this program; if not, write to the Free Software              *
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA*
 *****************************************************************************/

#include <KDE/KApplication>
#include <KDE/KAboutData>
#include <KDE/KCmdLineArgs>
#include <KDE/KUrl> //new
 

#include <kcmdlineargs.h>

#include <klocale.h>
//#include <kdebug.h>
//#include <ostream.h>
#include "klog.h"


//static const char *description = I18N_NOOP("The KDE Ham Radio Logging Software");


int main (int argc, char *argv[])
{
  KAboutData aboutData( "klog", "klog",
      ki18n("KLog"), "0.5.4",
      ki18n("The KDE Ham radio loggin software."),
      KAboutData::License_GPL,
      ki18n("Copyright (c) 2002-2010, Jaime Robles, EA4TV") );
//    aboutData.addAuthor("Jaime Robles, EA4TV","http://jaime.robles.es", "jaime@robles.es");
  //TODO: Add the command line options
  KCmdLineArgs::init( argc, argv, &aboutData );
 
  KCmdLineOptions options; //new
  options.add("+[file]", ki18n("Log to load (Still does not work)")); //new
  KCmdLineArgs::addCmdLineOptions(options); //new
 
  KApplication app;
 
  Klog* klog = new Klog();
  klog->show();
 
  KCmdLineArgs *args = KCmdLineArgs::parsedArgs(); //new
  if(args->count()) //new
  {
    //TODO: Add the open directly from the command line
    //klog->fileOpen(args->url(0).url()); //new
  }
 
  return app.exec();
}


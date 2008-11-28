/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : s b dic  7 18:42:45 CET 2002
    copyright            : (C) 2002 by Jaime Robles
    email                : jaime@kde.org
 ***************************************************************************/
/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation; either version 2 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program; if not, write to the Free Software              *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA*
 *****************************************************************************/
//#include <kcmdlineargs.h>
//#include <kaboutdata.h>
#include <klocale.h>
//#include <kdebug.h>
#include <qapplication.h>
#include "klog.h"


//static const char *description = I18N_NOOP("The KDE Ham Radio Logging Software");


// KLog is a free ham radio logging software

// With KDELIBS
/* 
static KCmdLineOptions options[] =
        {
                { 0, 0, 0 }
                // INSERT YOUR COMMANDLINE OPTIONS HERE
        };


int main(int argc, char *argv[])
{

        KAboutData aboutData( "klog", I18N_NOOP("Klog"), "KLog-0.4.4", description, KAboutData::License_GPL,                              "(c) 2002-2004, Jaime Robles, EA4TV", 0, "http://jaime.robles.es", "jaime@kde.org");
        aboutData.addAuthor("Jaime Robles, EA4TV","http://jaime.robles.es", "jaime@kde.org");
        KCmdLineArgs::init( argc, argv, &aboutData );
        KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

        KApplication a;
        Klog *klog = new Klog();

        if ((klog->haveWorld()) ) {
                a.setMainWidget(klog);
                klog->show();
                return a.exec();
        }

}
*/

// With QT
int main( int argc, char ** argv ) {
    QApplication a( argc, argv );
	Klog *klog = new Klog();
        if ((klog->haveWorld()) ) {
                a.setMainWidget(klog);
                klog->show();
                return a.exec();
        }


    //ApplicationWindow *mw = new ApplicationWindow();
	
/*    mw->setCaption( "KLog" );
    mw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();*/
}

# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project
noinst_HEADERS = adif.h award.h awarddxcc.h awardiota.h awards.h awardwaz.h \
	calls.h dxspot.h entity.h klog.h kloghamlib.h locator.h prefix.h qso.h \
	reference.h setup.h workedcalls.h world.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/klog.pot

KDE_ICON = AUTO

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = klog

# the application source, library search path, and link libraries
klog_SOURCES = adif.cpp award.cpp awarddxcc.cpp awardiota.cpp awards.cpp \
	awardwaz.cpp calls.cpp dxspot.cpp entity.cpp klog.cpp kloghamlib.cpp klogui.ui \
	locator.cpp main.cpp prefix.cpp qso.cpp reference.cpp setup.cpp setupui.ui \
	workedcalls.cpp world.cpp
klog_LDFLAGS = $(KDE_RPATH) $(all_libraries) -lhamlib -lkdeprint
klog_LDADD = $(LIB_QT) $(LIB_KDECORE) $(LIBSOCKET) $(LIB_KDEUI)

# this is where the desktop file will go 
shelldesktopdir   = $(kde_appsdir)/Utilities
shelldesktop_DATA = klog.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/klog
shellrc_DATA = klogui.rc


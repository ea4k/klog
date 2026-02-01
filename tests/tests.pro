#QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage -O0
#LIBS += -lgcov

TEMPLATE=subdirs
SUBDIRS=\
   #tst_main \
   tst_wizard \
   tst_adif \
   tst_callsign \
   tst_dxspot \
   tst_frequency \
   #tst_hamlib \
   tst_utilities \
   tst_mainqsoentrywidget \
   tst_mainwindowsattab \
   #tst_mainwindow \
   tst_dataproxy \
   tst_database \
   tst_filemanager \
   tst_locator \
   tst_logwindow \
   tst_mainwindowinputqso \
   tst_mainwindowinputothers \
   tst_qso \
   tst_qsodatacache \
   tst_setuphamlibnetworkwidget \
   tst_setuphamlibserialwidget \
   tst_setuppageelog \
   tst_world

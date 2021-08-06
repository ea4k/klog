#LIBS += -lgcov
#QMAKE_CXXFLAGS += --coverage
#QMAKE_LFLAGS += --coverage -fprofile-instr-generate -fcoverage-mapping

include(utilities/utilities.pri)
include(mainqsoentrywidget/mainqsoentrywidget.pri)
include(mainwindowsattab/mainwindowsattab.pri)
include(tst_dataproxy/tst_dataproxy.pri)
include(tst_mainwindowinputqso/tst_mainwindowinputqso.pri)
include(world/world.pri)


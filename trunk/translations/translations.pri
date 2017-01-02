# This voodoo comes from the Arora project

message( Importing the translation files...)

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
VPATH += $$PWD

# ls -1 *.ts | tr '\n' ' '
TRANSLATIONS += klog_ca.ts klog_es.ts klog_it.ts klog_ja.ts
isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

updateqm.input = TRANSLATIONS
updateqm.output = build/target/translations/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm build/target/translations/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

#qmfiles.files = TRANSLATIONS
#qmfiles.path = Content/Resources
#QMAKE_BUNDLE_DATA += qmfiles

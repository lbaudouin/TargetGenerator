#-------------------------------------------------
#
# Project created by QtCreator 2014-03-04T17:03:58
#
#-------------------------------------------------

QT       += core gui svg

unix {
    TARGET = targetgenerator

    isEmpty(PREFIX){
        PREFIX = /usr/local/bin/
    }

    target.path = $$PREFIX

    INSTALLS += target
}

win32 {
    TARGET = TargetGenerator
}

TEMPLATE = app


SOURCES += main.cpp\
        targetgenerator.cpp \
    highlighter.cpp

HEADERS  += targetgenerator.h \
    highlighter.h

FORMS    += targetgenerator.ui


TRANSLATIONS = TargetGenerator_fr.ts

QMAKE_EXTRA_COMPILERS += lrelease
lrelease.input         = TRANSLATIONS
lrelease.output        = ${QMAKE_FILE_BASE}.qm
lrelease.commands      = $$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_BASE}.qm
lrelease.CONFIG       += no_link target_predeps

RESOURCES += \
    ressources.qrc

OTHER_FILES += README


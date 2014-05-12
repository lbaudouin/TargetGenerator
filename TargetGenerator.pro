#-------------------------------------------------
#
# Copyright 2014, Léo Baudouin
#
# This file is part of a free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# This software is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details. You should have
# received a copy of the GNU General Public License along with
# this software. If not, see <http://www.gnu.org/licenses/>.
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
    highlighter.cpp \
    label.cpp

HEADERS  += targetgenerator.h \
    highlighter.h \
    label.h

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


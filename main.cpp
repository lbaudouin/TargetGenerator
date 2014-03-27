// Copyright 2014, Léo Baudouin
//
// This file is part of a free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// This software is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details. You should have
// received a copy of the GNU General Public License along with
// this software. If not, see <http://www.gnu.org/licenses/>.

#include <QtGui/QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#include "targetgenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString lang = QLocale::system().name().section('_', 0, 0);
    lang = lang.toLower();

    if(QFile::exists(":/lang/"+lang)){
        QTranslator *translator = new QTranslator();
        translator->load(QString(":/lang/") + lang);
        qApp->installTranslator( translator );
    }

    QTranslator *translatorQt = new QTranslator();
    translatorQt->load(QString("qt_") + lang, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator( translatorQt );

    TargetGenerator w;
    w.show();
    
    return a.exec();
}

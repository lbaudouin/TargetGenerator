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

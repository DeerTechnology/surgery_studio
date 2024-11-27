#include "homepage.h"
#include "main.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "appinit.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef __ENGLISH
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "untitled1_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
#endif
    HomePage w;
    AppInit::Instance()->start();
    w.show();
//    w.connect_serial();
    return a.exec();
}

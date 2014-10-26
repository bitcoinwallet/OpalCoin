#include <QApplication>
#include "filewindow.h"
#include <QTranslator>
#include <QLocale>

int mainupload(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator myTranslator;
    myTranslator.load("upcoder_"+QLocale::system().name(), ":/translations");
    a.installTranslator(&myTranslator);

    QTranslator qtTranslator;
    qtTranslator.load("qt_"+QLocale::system().name(), ":/translations");
    a.installTranslator(&qtTranslator);

    FileWindow w;
    w.show();
    return a.exec();
}

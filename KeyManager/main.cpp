#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>
#include <QFile>
#include <QFileInfo>
#include "mainwindow.h"
#include "datainterface.h"
#include "iointerfacesqlite.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //set translator
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "KeyManager_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    //set css styles
    QFile f(":qdarkstyle/light/lightstyle_.qss");
    if (!f.exists())
    {
        qDebug () << "Unable to set stylesheet, file not found:";
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    //start the app window
    MainWindow w;

    //set interfaces
    IOInterfaceSQLITE dbInterface;
    DataInterface dataInterface;

    w.setIOInterface(&dbInterface);
    w.setDataInterface(&dataInterface);

    w.init();
    w.show();

    return a.exec();
}

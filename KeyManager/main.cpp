#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>
#include <QFile>
#include <QFileInfo>
#include "mainwindow.h"
#include "datainterface.h"
#include "iointerfacesqlite.h"
#include "androidapplication.h"
#include <QPermissions>

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    AndroidApplication a(argc, argv);

    QFont font("Roboto");
    font.setStyleHint(QFont::SansSerif);
    font.setPixelSize(15);
    qApp->setFont(font);

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

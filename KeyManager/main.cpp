#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>
#include <QFile>
#include <QFileInfo>
#include "mainwindow.h"
#include "datainterface.h"
#include "iointerfacesqlite.h"
#include <QPermissions>

#ifdef Q_OS_ANDROID
#include "androidapplication.h"
#endif

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN64
    QApplication a(argc, argv);
#endif

#ifdef Q_OS_ANDROID
    AndroidApplication a(argc, argv);
#endif

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

    //set application-wide icon
    a.setWindowIcon(QIcon(":/images/icon.png"));

    //start the app window
    MainWindow w;

    //set interfaces
    IOInterfaceSQLITE dbInterface;
    DataInterface dataInterface;

    w.setIOInterface(&dbInterface);
    w.setDataInterface(&dataInterface);

    w.init();

    //w.show();
    w.showFullScreen();

    return a.exec();
}

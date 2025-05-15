#include "mainwindow.h"
#include <QPushButton>
#include <QGridLayout>
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedLayout>
#include <QMediaPlayer>
#include <QStringList>
#include <QFile>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMainWindow>
#include <QStackedWidget>
#include <QDockWidget>
#include <QStatusBar>

#if defined(Q_OS_ANDROID)
#include <QtCore/private/qandroidextras_p.h>
#include <QOperatingSystemVersion>
#endif

#include "homeview.h"
#include "tableview.h"
#include "keychainstatusview.h"
#include "recipientview.h"
#include "addrecipientview.h"
#include "handoverview.h"
#include "returndateview.h"
//#include "handoutsummaryview.h"
#include "annotationview.h"
#include "viewstackmanager.h"
#include "datainterface.h"
#include "scannerview.h"
#include "addkeychainview.h"
#include "editkeyview.h"
#include "iointerfacesqlite.h"
#include "toolsviewcodegenerator.h"
#include "returndateview.h"
#include "searchview.h"
#include "settingsviewdb.h"
#include "toolsviewdataadministration.h"

#ifndef GMANDANTID
    #define GMANDANTID 1
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow (parent)
{
    mScanView = 0;
    mHomeView = 0;
    mSearchView = 0;
    mKeychainStatusView = 0;
    mRecipientView = 0;
    mAddRecipientView = 0;
    mAnnotationView = 0;
    mDataInterface = 0;
    mAddKeychainView = 0;
    mEditKeyView = 0;
    mViewStack = 0;
    mSettingsViewDb = 0;
    mToolsViewCodegenerator = 0;
    mToolsViewDataAdministration = 0;
}

void MainWindow::setDataInterface (DataInterface *dataInterface)
{
    mDataInterface = dataInterface;
}

void MainWindow::setIOInterface (IOInterface *ioInterface)
{
    mDbInterface = ioInterface;
}

void MainWindow::init()
{   
    mViewStack = new QStackedWidget (this);
    setCentralWidget(mViewStack);
    qDebug () << "centralWidget width: " << centralWidget()->screen()->size().width();
    qDebug () << "centralWidget height: " << centralWidget()->screen()->size().height();
    QStatusBar *bar = statusBar();
    setStatusBar(bar);

    mHomeView = new HomeView (this);
    mScanView = new ScannerView (this);
    mSearchView = new SearchView (this);
    mRecipientView = new RecipientView (this);
    mKeychainStatusView = new KeychainStatusView (this);
    mAddRecipientView = new AddRecipientView (this);
    mHandoverView = new HandoverView (this);
    mAnnotationView = new AnnotationView (this);
    mAddKeychainView = new AddKeychainView (this);
    mEditKeyView = new EditKeyView (this);
    mToolsViewCodegenerator = new ToolsViewCodeGenerator (this);
    mReturnDateView = new ReturnDateView (this);
    mSettingsViewDb = new SettingsViewDb (this);
    mToolsViewDataAdministration = new ToolsViewDataAdministration (this);

    registerView (mHomeView);
    registerView (mScanView);
    registerView (mSearchView);
    registerView (mRecipientView);
    registerView (mKeychainStatusView);
    registerView (mAddRecipientView);
    registerView (mHandoverView);
    registerView (mAnnotationView);
    registerView (mAddKeychainView);
    registerView (mEditKeyView);
    registerView (mToolsViewCodegenerator);
    registerView (mReturnDateView);
    registerView (mSettingsViewDb);
    registerView (mToolsViewDataAdministration);

    mViewStack->setCurrentWidget(mHomeView);

    //register possible menu navigation paths
    mViewStackManager = new ViewStackManager ();

    //views in the section 'key handover management of known keychains'
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mScanView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mKeychainStatusView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mRecipientView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mReturnDateView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mAnnotationView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mHandoverView);

    // views in the section 'adding unknown (new) keychains to db'
    mViewStackManager->addNode(ViewStackManager::NewCodeScanned, mAddKeychainView);
    mViewStackManager->addNode(ViewStackManager::NewCodeScanned, mEditKeyView);

    // views in the section 'tools'
    mViewStackManager->addNode(ViewStackManager::Tools, mToolsViewCodegenerator);
    mViewStackManager->addNode(ViewStackManager::Tools, mToolsViewDataAdministration);

    // views in the section 'settings'
    mViewStackManager->addNode(ViewStackManager::Settings, mSettingsViewDb);

    // views in the section 'search'
    mViewStackManager->addNode(ViewStackManager::Search, mSearchView);

    mViewStackManager->setCurrentStackId(ViewStackManager::HandoverOut);
    mViewStack->setCurrentWidget(mHomeView);

    //set css styles for all widgets
    QFile f(":qdarkstyle/light/lightstyle.qss");
    if (!f.exists())
    {
        qDebug () << "Unable to set stylesheet, file not found:";
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        setStyleSheet(ts.readAll());
    }

/*#if defined(Q_OS_ANDROID)
    accessAllFiles();
#endif*/

    // init db
    if (0 == mDbInterface->countDbTables())
    {
        QMessageBox msgBox;
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setText ("Erster Start");
        msgBox.setInformativeText("Eine neue Datenbank wird erstellt.");
        msgBox.exec();

        if (!mDbInterface->initFirstStart())
        {
            QMessageBox msgBox;
            msgBox.setStandardButtons(QMessageBox::Abort);
            msgBox.setText ("Fehler!");
            msgBox.setInformativeText("Datenbank konnte nicht geöffnet werden.");
            msgBox.setDetailedText(mDbInterface->dbGetLastError());
            msgBox.exec();
        }
    }
}

void MainWindow::registerView (WinSubmenu *view)
{
    connect (view, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT (onMenuBtnClicked(Gui::MenuButton)));
    view->setDataInterface(mDataInterface);
    view->setIOInterface(mDbInterface);
    mViewStack->addWidget(view);
}

void MainWindow::onMenuBtnClicked (Gui::MenuButton btnType)
{
    //WinSubmenu *sender = dynamic_cast <WinSubmenu*> (QObject::sender());

    // qDebug () << "MainWindow::menuBtnClicked";
    // qDebug () << "btnType: " << btnType;

    // if (sender)
    // {
        QWidget *nextWidget = nullptr;

        switch (btnType)
        {
            case (Gui::Scanner):
                qDebug () << "Gui::Scanner";
                mViewStackManager->setCurrentStackId(ViewStackManager::HandoverOut);
                nextWidget = mViewStackManager->begin();
                break;
            case (Gui::Back):
                nextWidget = mViewStackManager->previous();
                break;
            case (Gui::Next):
                nextWidget = mViewStackManager->next();
                break;
            case (Gui::MainMenu):
                nextWidget = mHomeView;
                break;
            case (Gui::NewCodeScanned):
                mViewStackManager->setCurrentStackId(ViewStackManager::NewCodeScanned);
                nextWidget = mViewStackManager->begin();
                break;
            case (Gui::Settings):
                mViewStackManager->setCurrentStackId(ViewStackManager::Settings);
                nextWidget = mViewStackManager->begin();
                break;
            case (Gui::Tools):
                mViewStackManager->setCurrentStackId(ViewStackManager::Tools);
                nextWidget = mViewStackManager->begin();
                break;
            case (Gui::Search):
                mViewStackManager->setCurrentStackId(ViewStackManager::Search);
                nextWidget = mViewStackManager->begin();
                break;
            case (Gui::Exit):
                if (reallyQuit())
                {
                    doBackup();
                    qApp->quit();
                }
                break;
            default:
                qDebug () << "MainWindow::onMenuBtnClicked. Button not catched: " << btnType;
                return;
        }

        if (nextWidget)
        {
            mViewStack->setCurrentWidget(nextWidget);
        }
        else
        {
            mViewStack->setCurrentWidget(mHomeView);
        }
        mViewStack->currentWidget()->setFocus();
    //}
}

void MainWindow::setView (QWidget* view)
{
    if (!view)
        return;

    mViewStack->setCurrentWidget(view);
}

/*#if defined(Q_OS_ANDROID)
void MainWindow::accessAllFiles()
{
    if(QOperatingSystemVersion::current() < QOperatingSystemVersion(QOperatingSystemVersion::Android, 11)) {
        qDebug() << "OS is Android 10 or earlier - ALL FILES permission isn't possible!";
        return;
    }

#define PACKAGE_NAME "package:de.hva.schluesselverwaltung"

    jboolean value = QJniObject::callStaticMethod<jboolean>("android/os/Environment", "isExternalStorageManager");
    if(value == false) {
        qDebug() << "requesting ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION";
        QJniObject ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION = QJniObject::getStaticObjectField( "android/provider/Settings", "ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION","Ljava/lang/String;" );
        QJniObject intent("android/content/Intent", "(Ljava/lang/String;)V", ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION.object());
        QJniObject jniPath = QJniObject::fromString(PACKAGE_NAME);
        QJniObject jniUri = QJniObject::callStaticObjectMethod("android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", jniPath.object<jstring>());
        QJniObject jniResult = intent.callObjectMethod("setData", "(Landroid/net/Uri;)Landroid/content/Intent;", jniUri.object<jobject>() );
        QtAndroidPrivate::startActivity(intent, 0);
    } else {
        qDebug() << "SUCCESS ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION";
    }
}
#endif*/

void MainWindow::doBackup()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString fileName = currentTime.toString("yyyyMMdd_hhmmss");
    fileName.append(".sqlite");
    mDbInterface->doDbBackup(fileName);
}

bool MainWindow::reallyQuit()
{
    QMessageBox msgBox;
    msgBox.setStandardButtons(QMessageBox::Abort | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setText ("Programm beenden.");
    msgBox.setInformativeText("Möchten Sie das Programm wirklich beenden?");

    bool ok = false;
    int selection = msgBox.exec();

    switch (selection)
    {
        case QMessageBox::Abort:
            ok = false;
            break;
        case QMessageBox::Ok:
            ok = true;
            break;
        default:
            break;
    }
    return ok;
}

MainWindow::~MainWindow()
{
    // if (mDataInterface)
    //     delete mDataInterface;

    if (mViewStackManager)
        delete mViewStackManager;
}

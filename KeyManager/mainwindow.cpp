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
#include "toolsview.h"
#include "returndateview.h"

#ifndef GMANDANTID
    #define GMANDANTID 1
#endif

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    //setStyleSheet("QWidget {background-color: #FFFAFA; font: 25px;}");
    // setStyleSheet   ("QTableView {background-color: #FFFAFA; font: 20px;}\
    //                 QLabel {font: 17px;}\
    //                 QCheckBox {font: 17px;}\
    //                 QRadioButton {font: 17px;}\
    //                 QRadioButton::indicator::unchecked {image: url(:/images/radiobutton_unchecked.png)};\
    //                 QRadioButton::indicator::checked {image: url(:/images/radiobutton_checked.png)}\
    //                 QRadioButton::indicator {width: 25px; height 25px;}");

    mScanView = 0;
    mHomeView = 0;
    mTableView = 0;
    mKeychainStatusView = 0;
    mRecipientView = 0;
    mAddRecipientView = 0;
    mAnnotationView = 0;
    mDataInterface = 0;
    mAddKeychainView = 0;
    mEditKeyView = 0;
    mViewStack = 0;
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
    mViewStack = new QStackedLayout (this);
    setLayout(mViewStack);

    mHomeView = new HomeView (this);
    mScanView = new ScannerView (this);
    mTableView = new TableView ("addresses", this);
    mRecipientView = new RecipientView (this);
    mKeychainStatusView = new KeychainStatusView (this);
    mAddRecipientView = new AddRecipientView (this);
    mHandoverView = new HandoverView (this);
    mAnnotationView = new AnnotationView (this);
    mAddKeychainView = new AddKeychainView (this);
    mEditKeyView = new EditKeyView (this);
    mToolsView = new ToolsView (this);
    mReturnDateView = new ReturnDateView (this);

    registerView (mHomeView);
    registerView (mScanView);
    registerView (mTableView);
    registerView (mRecipientView);
    registerView (mKeychainStatusView);
    registerView (mAddRecipientView);
    registerView (mHandoverView);
    registerView (mAnnotationView);
    registerView (mAddKeychainView);
    registerView (mEditKeyView);
    registerView (mToolsView);
    registerView (mReturnDateView);

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

    // views in the section 'settings'
    mViewStackManager->addNode(ViewStackManager::Settings, mToolsView);

    mViewStackManager->setCurrentStackId(ViewStackManager::HandoverOut);
    mViewStack->setCurrentWidget(mHomeView);

    // init db
    if (0 == mDbInterface->countDbTables())
    {
        QMessageBox::information(0, "MainWindow::init()",
                                 "No database found!\n"
                                 "Creating database tables and default values!", QMessageBox::Ok);

        qDebug() << "First start, creating database";

        if (!mDbInterface->initFirstStart())
        {
            QMessageBox::critical(0, "MainWindow::firstStart()",
                                  "Database initialisation failed!",
                                  QMessageBox::Cancel);
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
    WinSubmenu *sender = dynamic_cast <WinSubmenu*> (QObject::sender());

    qDebug () << "MainWindow::menuBtnClicked";
    qDebug () << "btnType: " << btnType;

    if (sender)
    {
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
            default:
                qDebug () << "MainWindow::onMenuBtnClicked. Button not catched: " << btnType;
                return;
        }

        if (nextWidget)
            mViewStack->setCurrentWidget(nextWidget);
        else
            mViewStack->setCurrentWidget(mHomeView);
    }
}

// void MainWindow::playSound ()
// {
//     QMediaPlayer player;
//     QAudioOutput audioOut;
//     player.setAudioOutput(&audioOut);
//     QUrl filelocation ("qrc:/sounds/scanner_beep.mp3");
//     player.setSource(filelocation);
//     audioOut.setVolume(100);
//     player.play();♥
// }

void MainWindow::setView (QWidget* view)
{
    if (!view)
        return;

    mViewStack->setCurrentWidget(view);
}

MainWindow::~MainWindow()
{
    // if (mDataInterface)
    //     delete mDataInterface;

    if (mViewStackManager)
        delete mViewStackManager;
}

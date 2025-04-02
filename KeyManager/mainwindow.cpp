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

#ifndef GMANDANTID
    #define GMANDANTID 1
#endif

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    mScanView = 0;
    mHomeView = 0;
    mTableView = 0;
    mKeychainStatusView = 0;
    mRecipientView = 0;
    mAddRecipientView = 0;
    //mReturnDateView = 0;
    mAnnotationView = 0;
    mDataInterface = 0;

    mViewStack = 0;

    mDataInterface = new DataInterface();

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

    registerView (mHomeView);
    registerView (mScanView);
    registerView (mTableView);
    registerView (mRecipientView);
    registerView (mKeychainStatusView);
    registerView (mAddRecipientView);
    registerView (mHandoverView);
    registerView (mAnnotationView);

    mViewStack->setCurrentWidget(mHomeView);

    //register possible menu navigation paths
    mViewStackManager = new ViewStackManager ();

    mViewStackManager->addNode(ViewStackManager::HandoverOut, mScanView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mKeychainStatusView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mRecipientView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mAnnotationView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mHandoverView);
    mViewStackManager->setCurrentStackId(ViewStackManager::HandoverOut);

    mViewStack->setCurrentWidget(mHomeView);
}

void MainWindow::registerView (WinSubmenu *view)
{
    connect (view, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT (onMenuBtnClicked(Gui::MenuButton)));
    view->setDataInterface(mDataInterface);
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
            case (Gui::NewCode):
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
//     player.play();
// }

void MainWindow::setView (QWidget* view)
{
    if (!view)
        return;

    mViewStack->setCurrentWidget(view);
}

MainWindow::~MainWindow()
{
    if (mDataInterface)
        delete mDataInterface;

    if (mViewStackManager)
        delete mViewStackManager;
}

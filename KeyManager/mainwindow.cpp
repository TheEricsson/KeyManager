#include "mainwindow.h"
#include "scannerview.h"
#include <QPushButton>
#include <QGridLayout>
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedLayout>
#include <QTimer>
#include <QImageCapture>
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QStringList>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QFile>

#include "homeview.h"
#include "camera.h"
#include "databaseimpl.h"
#include "tableview.h"
#include "keychainstatusview.h"
#include "recipientview.h"
#include "addrecipientview.h"
#include "handoverview.h"
#include "returndateview.h"
#include "handoutsummaryview.h"
#include "dataobjecthandover.h"
#include "annotationview.h"
#include "viewstackmanager.h"
#include "datainterface.h"

#ifndef GMANDANTID
    #define GMANDANTID 1
#endif

#ifdef ENCODERTEST
#include "tests/qrencodertest.h"
#endif

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    //mLastView = 0;
    //mCameraInstance = 0;
    //mGrabTimer = 0;
    mScanView = 0;
    mHomeView = 0;
    mTableView = 0;
    mKeychainStatusView = 0;
    //mKeysOverviewModel = 0;
    //mRecipientsModel = 0;
    //mKeychainModel = 0;
    mRecipientView = 0;
    mAddRecipientView = 0;
    mReturnDateView = 0;
    mAnnotationView = 0;
    mDataInterface = 0;

    //mLayout = 0;
    mViewStack = 0;
    //mDatabase = 0;
    //mDataHandover = 0;

    //initDatabase ();
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
    mReturnDateView = new ReturnDateView (this);
    mHandoutSummaryView = new HandoutSummaryView (this);
    mAnnotationView = new AnnotationView (this);

    registerView (mHomeView);
    registerView (mScanView);
    registerView (mTableView);
    registerView (mRecipientView);
    registerView (mKeychainStatusView);
    registerView (mAddRecipientView);
    registerView (mHandoverView);
    registerView (mReturnDateView);
    registerView (mHandoutSummaryView);
    registerView (mAnnotationView);

    // views with a model need db access
    // mRecipientView->setDatabaseHandle (mDatabase);
    // mKeychainStatusView->setDatabaseHandle (mDatabase);
    // mAddRecipientView->setDatabaseHandle (mDatabase);

    mViewStack->setCurrentWidget(mHomeView);

    //register possible menu navigation paths
    mViewStackManager = new ViewStackManager ();

    mViewStackManager->addNode(ViewStackManager::HandoverOut, mScanView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mKeychainStatusView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mRecipientView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mReturnDateView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mAnnotationView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mHandoverView);
    mViewStackManager->addNode(ViewStackManager::HandoverOut, mHandoutSummaryView);
    mViewStackManager->setCurrentStackId(ViewStackManager::HandoverOut);

    //connect (mScanView, SIGNAL(keycodeRecognised(int)), this, SLOT(onKeycodeRecognised(int)));

    mViewStack->setCurrentWidget(mHomeView);

    //showHomeView();
}

void MainWindow::registerView (WinSubmenu *view)
{
    connect (view, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT (onMenuBtnClicked(Gui::MenuButton)));
    view->setDataInterface(mDataInterface);
    mViewStack->addWidget(view);
}

void MainWindow::handleScannedKey()
{
    //get current values
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

void MainWindow::onKeycodeRecognised (int keyCode)
{
    qDebug () << "MainWindow::onKeycodeRecognised: " << keyCode;
}

// void MainWindow::showTableView ()
// {
//     setView(mTableView);
// }

// void MainWindow::showRecipientView ()
// {
//     if (!mRecipientView)
//         return;

//     mRecipientView->reset();
//     mRecipientView->setDataObject(mDataHandover);

//     if (!mRecipientsModel)
//         mRecipientsModel = new QSqlRelationalTableModel;

//     if (!mDatabase->initRecipientModel(mRecipientsModel))
//         return;

//     if  (!mRecipientView->setModel(mRecipientsModel))
//         return;

//     setView(mRecipientView);
// }

// void MainWindow::closeRecipientView ()
// {
//     showScannerView();
// }

// void MainWindow::showAddRecipientView ()
// {
//     mAddRecipientView->clearForm();
//     setView(mAddRecipientView);
// }

// void MainWindow::closeAddRecipientView ()
// {
//     if (mLastView == eViewState::Recipient)
//     {
//         mRecipientView->setTableFilter (mAddRecipientView->getName ());
//         mRecipientView->update();
//         showRecipientView();
//     }
//     else
//         showHomeView();
// }

// void MainWindow::showReturnDateView()
// {
//     if (!mReturnDateView)
//         return;

//     mReturnDateView->setDataObject(mDataHandover);

//     setView(mReturnDateView);
// }

// void MainWindow::closeReturnDateView()
// {
//     showRecipientView ();
// }

// void MainWindow::showAnnotationView ()
// {
//     mAnnotationView->setDataObject(mDataHandover);
//     setView (mAnnotationView);
// }

// void MainWindow::closeAnnotationView ()
// {
//     showReturnDateView();
// }

// void MainWindow::addRecipientViewSubmitted ()
// {
//     if (!mAddRecipientView)
//         return;

//     if (!mDatabase)
//         return;

//     mDatabase->addNewRecipient (mAddRecipientView->getRecipientData());
//     closeAddRecipientView ();
// }

// bool MainWindow::showKeychainStatusView (int aBarcode)
// {
//     if (!mKeychainStatusView)
//         return false;

//     if (!mKeysOverviewModel)
//         mKeysOverviewModel = new QSqlRelationalTableModel ();

//     if(!mKeychainModel)
//         mKeychainModel = new QSqlRelationalTableModel ();

//     if (mDatabase->initKeyOverviewModel(mKeysOverviewModel, aBarcode))
//     {
//         if (!mDataHandover)
//             mDataHandover = new DataObjectHandover ();

//         mKeychainStatusView->setDataObject (mDataHandover);

//         if (!mKeychainStatusView->setKeysModel(mKeysOverviewModel))
//             return false;

//         if (!mDatabase->initKeychainModel(mKeychainModel, aBarcode))
//             return false;

//         if (!mKeychainStatusView->setKeychainModel(mKeychainModel))
//             return false;

//         Database::eKeychainStatusId keyChainStatus = mDatabase->getKeychainStatusId (aBarcode);
//         mDataHandover->setKeychainStatus(keyChainStatus);
//         mKeychainStatusView->setKeychainStatus (keyChainStatus);
//         mKeychainStatusView->setKeychainImagePath (mDatabase->getKeychainImagePath(aBarcode));

//         setView(mKeychainStatusView);

//         return true;
//     }
//     else
//         return false;
// }

// void MainWindow::closeKeychainStatusView ()
// {
//     showScannerView();
// }

// void MainWindow::showHandoverView ()
// {
//     qDebug () << "MainWindow::showHandoverView ()";
//     if (!mHandoverView)
//         return;
//     else
//     {
//         mHandoverView->clear();
//         mHandoverView->setDataObject(mDataHandover);
//         setView (mHandoverView);
//     }
// }

// void MainWindow::closeHandoverView ()
// {
//     setView (mAnnotationView);
// }

// void MainWindow::showHandoutSummaryView ()
// {
//     mHandoutSummaryView->setDataObject(mDataHandover);
//     setView (mHandoutSummaryView);
// }

// void MainWindow::closeHandoutSummaryView ()
// {
//     // todo: reset all views after completed handout
//     // do database update...
//     if (mDataHandover)
//     {
//         mDatabase->dataUpdate((DataObject*)mDataHandover);
//         delete mDataHandover;
//         mDataHandover = 0;
//     }
//     showHomeView();
// }

// void MainWindow::onSearchButtonReleased ()
// {

// }

// void MainWindow::onManageButtonReleased ()
// {

// }

// void MainWindow::decodeFromVideoFrame ()
// {
//     decodeImage (0, mCameraInstance->getImageFromVideoframe());
// }

// void MainWindow::decodeImage (int requestId, const QImage &img)
// {
//     qDebug () << "MainWindow::decodeImage called";

//     Q_UNUSED(requestId);

// // trigger decode

// #ifdef Q_OS_WIN64
//     // no cam on windows pc -> use test image with barcode
//     QImage testCode (":/images/barcode.png");
//     QString barcode = decoder.decodeImage(testCode);
// #else
//     QString barcode = decoder.decodeImage(img);
// #endif

//     QString barcodeAsNumber = barcode.mid (0, 5);
//     barcodeAsNumber.append(barcode.mid(6, 5));
//     int barcodeAsInt = barcodeAsNumber.toInt();

//     if ("" != barcode.toStdString())
//     {
//         qDebug () << "barcode:" << barcode;
//         qDebug () << "barcodeAsNumber:" << barcodeAsNumber;
//         qDebug () << "barcodeAsId: = " << barcodeAsInt;
//     }

//     QString customerId = barcode.mid (0, 5);
//     QString keyId = barcode.mid (6, 5);

//     if (customerId == "00001" && 5 == keyId.length())
//     {
//         mGrabTimer->stop ();
//         mCameraInstance->stopCamera();

//         // code recognized: play a supermarket beep sound :)
//         playSound ();

//         mScanView->setKeyLabel(barcodeAsNumber);

//         // set scanview ui state
//         mScanView->setScannerState(ScannerState::SCANSUCCEEDED);

//         // search key in database
//         if (mDatabase->findKeyCode(barcodeAsInt))
//         {
//             //barcode is recognised and found in database
//             bool retVal = mDatabase->setKeyCode(barcodeAsInt);

//             retVal = showKeychainStatusView(barcodeAsInt);
//             qDebug () << "MainWindow::showKeychainStatusView: " << retVal;
//         }
//         else
//         {
//             // todo: recognised unknown barcode with a legit barcode format -> create dialog to add this keychain to db
//             return;
//         }

//     }
// }

void MainWindow::playSound ()
{
    QMediaPlayer player;
    QAudioOutput audioOut;
    player.setAudioOutput(&audioOut);
    QUrl filelocation ("qrc:/sounds/scanner_beep.mp3");
    player.setSource(filelocation);
    audioOut.setVolume(100);
    player.play();
}

void MainWindow::setView (QWidget* view)
{
    if (!view)
        return;

    mViewStack->setCurrentWidget(view);
}

MainWindow::~MainWindow()
{
    // if (mDatabase)
    //     delete mDatabase;

    // if (mDataHandover)
    //     delete mDataHandover;

    // if (mKeychainStatusView)
    //     delete mKeychainStatusView;

    // if (mKeysOverviewModel)
    //     delete mKeysOverviewModel;

    // if (mKeysOverviewModel)
    //     delete mKeysOverviewModel;

    // if (0 != btnScan)
    //     delete btnScan;

    // if (0 != btnSearch)
    //     delete btnSearch;

    // if (0 != btnManage)
    //     delete btnManage;

    // if (0 != btnExit)
    //     delete btnExit;

    // if (0 != scannerView)
    //     delete scannerView;

    // if (0 != mainLayout)
    //     delete mainLayout;
}

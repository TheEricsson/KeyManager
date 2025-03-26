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

#ifndef GMANDANTID
    #define GMANDANTID 1
#endif

#ifdef ENCODERTEST
#include "tests/qrencodertest.h"
#endif

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    decoder.setDecoder(QZXing::DecoderFormat_CODE_128);
    //optional settings
    decoder.setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    decoder.setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);

    mLastView = 0;
    mViewState = eViewState::None;
    mCameraInstance = 0;
    mGrabTimer = 0;
    mScanView = 0;
    mHomeView = 0;
    mTableView = 0;
    mKeychainStatusView = 0;
    mKeysOverviewModel = 0;
    mRecipientsModel = 0;
    mKeychainModel = 0;
    mRecipientView = 0;
    mAddRecipientView = 0;
    mReturnDateView = 0;
    mAnnotationView = 0;

    mLayout = 0;
    mDatabase = 0;
    mDataHandover = 0;

    initDatabase ();

    mLayout = new QStackedLayout(this);
    setLayout(mLayout);

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

    mLayout->addWidget(mHomeView);
    mLayout->addWidget(mScanView);
    mLayout->addWidget(mTableView);
    mLayout->addWidget(mRecipientView);
    mLayout->addWidget(mKeychainStatusView);
    mLayout->addWidget(mAddRecipientView);
    mLayout->addWidget(mHandoverView);
    mLayout->addWidget(mReturnDateView);
    mLayout->addWidget(mHandoutSummaryView);
    mLayout->addWidget(mAnnotationView);

    showHomeView();

    // handle signals by ScanView
    connect (mScanView, SIGNAL(firstButtonClicked()), this, SLOT (closeScannerView()));
    connect (mScanView, SIGNAL(secondButtonClicked()), this, SLOT (showScannerView()));

    // handle signals by HomeView
    connect (mHomeView,SIGNAL(showScannerView()), this, SLOT(showScannerView()));
    connect (mHomeView,SIGNAL(showTableView()), this, SLOT(showTableView()));

    // handle signals by TableView
    connect (mTableView, SIGNAL(previousButtonClicked()), this, SLOT (closeTableView()));

    // handle signals by KeychainStatusView
    connect (mKeychainStatusView, SIGNAL(firstButtonClicked()), this, SLOT (closeKeychainStatusView()));
    connect (mKeychainStatusView, SIGNAL(secondButtonClicked()), this, SLOT (showRecipientView()));

    // handle signals by RecipientView
    connect (mRecipientView, SIGNAL(firstButtonClicked()), this, SLOT (closeRecipientView()));
    connect (mRecipientView, SIGNAL(secondButtonClicked()), this, SLOT (showAddRecipientView()));
    connect (mRecipientView, SIGNAL(thirdButtonClicked()), this, SLOT (showReturnDateView()));

    // handle signals by AddRecipientView
    connect (mAddRecipientView, SIGNAL(previousButtonClicked()), this, SLOT (closeAddRecipientView()));
    connect (mAddRecipientView, SIGNAL(okButtonClicked()), this, SLOT (addRecipientViewSubmitted()));

    // handle signals by returndateview
    connect (mReturnDateView, SIGNAL(firstButtonClicked()), this, SLOT (closeReturnDateView()));
    connect (mReturnDateView, SIGNAL(secondButtonClicked()), this, SLOT (showAnnotationView()));

    //handle signals by AnnotationView
    connect (mAnnotationView, SIGNAL(secondButtonClicked()), this, SLOT (showHandoverView()));
    connect (mAnnotationView, SIGNAL(firstButtonClicked()), this, SLOT (closeAnnotationView()));

    // handle signals by handoverview
    connect (mHandoverView, SIGNAL(firstButtonClicked()), this, SLOT (closeHandoverView()));
    connect (mHandoverView, SIGNAL(thirdButtonClicked()), this, SLOT (showHandoutSummaryView()));

    // handle signals by handoutSummaryView
    //connect (mHandoutSummaryView, SIGNAL(firstButtonClicked()), this, SLOT (generatePdfSummary()));
    connect (mHandoutSummaryView, SIGNAL(secondButtonClicked()), this, SLOT (closeHandoutSummaryView()));
}

void MainWindow::initDatabase ()
{
    mDatabase = new DatabaseImpl ();
}

void MainWindow::handleScannedKey()
{
    //get current values
}

void MainWindow::closeScannerView ()
{
    if (mGrabTimer)
    {
        mGrabTimer->stop();
    }
    if (mCameraInstance)
    {
        mCameraInstance->stopCamera();
    }

    showHomeView();
}

void MainWindow::closeTableView ()
{
    showHomeView();
}

void MainWindow::showHomeView ()
{
    if (mDataHandover)
        delete mDataHandover;
    setView(mHomeView);
}

void MainWindow::showScannerView ()
{
    if (!mScanView)
        return;

    if (!mCameraInstance)
    {
        mCameraInstance = new Camera ();
        mCameraInstance->setVideoOutput(mScanView->getViewfinder());
    }

    if (!mGrabTimer)
    {
        mGrabTimer = new QTimer (this);
        mGrabTimer->setInterval(500);
        connect (mGrabTimer, SIGNAL(timeout()), this, SLOT(decodeFromVideoFrame()));
    }

    mCameraInstance->startCamera();
    mGrabTimer->start();
    mScanView->setScannerState(ScannerState::SCANNING);

    setView(mScanView);
}

void MainWindow::showTableView ()
{
    setView(mTableView);
}

void MainWindow::showRecipientView ()
{
    if (!mRecipientView)
        return;

    if (!mRecipientsModel)
        mRecipientsModel = new QSqlRelationalTableModel;

    if (!mDatabase->initRecipientModel(mRecipientsModel))
        return;

    if  (!mRecipientView->setModel(mRecipientsModel))
        return;

    mRecipientView->reset();
    mRecipientView->setDataObject(mDataHandover);
    setView(mRecipientView);
}

void MainWindow::closeRecipientView ()
{
    showScannerView();
}

void MainWindow::showAddRecipientView ()
{
    mAddRecipientView->clearForm();
    setView(mAddRecipientView);
}

void MainWindow::closeAddRecipientView ()
{
    if (mLastView == eViewState::Recipient)
    {
        mRecipientView->setTableFilter (mAddRecipientView->getName ());
        mRecipientView->update();
        showRecipientView();
    }
    else
        showHomeView();
}

void MainWindow::showReturnDateView()
{
    if (!mReturnDateView)
        return;

    mReturnDateView->setDataObject(mDataHandover);

    setView(mReturnDateView);
}

void MainWindow::closeReturnDateView()
{
    showRecipientView ();
}

void MainWindow::showAnnotationView ()
{
    mAnnotationView->setDataObject(mDataHandover);
    setView (mAnnotationView);
}

void MainWindow::closeAnnotationView ()
{
    showReturnDateView();
}

void MainWindow::addRecipientViewSubmitted ()
{
    if (!mAddRecipientView)
        return;

    if (!mDatabase)
        return;

    mDatabase->addNewRecipient (mAddRecipientView->getRecipientData());
    closeAddRecipientView ();
}

bool MainWindow::showKeychainStatusView (int aBarcode)
{
    if (!mKeychainStatusView)
        return false;

    if (!mKeysOverviewModel)
        mKeysOverviewModel = new QSqlRelationalTableModel ();

    if(!mKeychainModel)
        mKeychainModel = new QSqlRelationalTableModel ();

    if (mDatabase->initKeyOverviewModel(mKeysOverviewModel, aBarcode))
    {
        if (!mDataHandover)
            mDataHandover = new DataObjectHandover ();

        mKeychainStatusView->setDataObject (mDataHandover);

        if (!mKeychainStatusView->setKeysModel(mKeysOverviewModel))
            return false;

        if (!mDatabase->initKeychainModel(mKeychainModel, aBarcode))
            return false;

        if (!mKeychainStatusView->setKeychainModel(mKeychainModel))
            return false;

        int keyChainStatus = mDatabase->getKeychainStatusId (aBarcode);
        mKeychainStatusView->setKeychainStatus (keyChainStatus);
        mKeychainStatusView->setKeychainImagePath (mDatabase->getKeychainImagePath(aBarcode));

        setView(mKeychainStatusView);

        return true;
    }
    else
        return false;
}

void MainWindow::closeKeychainStatusView ()
{
    showScannerView();
}

void MainWindow::showHandoverView ()
{
    qDebug () << "MainWindow::showHandoverView ()";
    if (!mHandoverView)
        return;
    else
    {
        mHandoverView->clear();
        mHandoverView->setDataObject(mDataHandover);
        setView (mHandoverView);
    }
}

void MainWindow::closeHandoverView ()
{
    setView (mAnnotationView);
}

void MainWindow::showHandoutSummaryView ()
{
    mHandoutSummaryView->setDataObject(mDataHandover);
    setView (mHandoutSummaryView);
}

void MainWindow::closeHandoutSummaryView ()
{
    // todo: reset all views after completed handout
    // do database update...
    if (mDataHandover)
    {
        mDatabase->dataUpdate((DataObject*)mDataHandover);
        delete mDataHandover;
        mDataHandover = 0;
    }
    showHomeView();
}

void MainWindow::onSearchButtonReleased ()
{

}

void MainWindow::onManageButtonReleased ()
{

}

void MainWindow::decodeFromVideoFrame ()
{
    decodeImage (0, mCameraInstance->getImageFromVideoframe());
}

void MainWindow::decodeImage (int requestId, const QImage &img)
{
    qDebug () << "MainWindow::decodeImage called";

    Q_UNUSED(requestId);

// trigger decode

#ifdef Q_OS_WIN64
    // no cam on windows pc -> use test image with barcode
    QImage testCode (":/images/barcode.png");
    QString barcode = decoder.decodeImage(testCode);
#else
    QString barcode = decoder.decodeImage(img);
#endif

    QString barcodeAsNumber = barcode.mid (0, 5);
    barcodeAsNumber.append(barcode.mid(6, 5));
    int barcodeAsInt = barcodeAsNumber.toInt();

    if ("" != barcode.toStdString())
    {
        qDebug () << "barcode:" << barcode;
        qDebug () << "barcodeAsNumber:" << barcodeAsNumber;
        qDebug () << "barcodeAsId: = " << barcodeAsInt;
    }

    QString customerId = barcode.mid (0, 5);
    QString keyId = barcode.mid (6, 5);

    if (customerId == "00001" && 5 == keyId.length())
    {
        mGrabTimer->stop ();
        mCameraInstance->stopCamera();

        // code recognized: play a supermarket beep sound :)
        playSound ();

        mScanView->setKeyLabel(barcodeAsNumber);

        // set scanview ui state
        mScanView->setScannerState(ScannerState::SCANSUCCEEDED);

        // search key in database
        if (mDatabase->findKeyCode(barcodeAsInt))
        {
            //barcode is recognised and found in database
            bool retVal = mDatabase->setKeyCode(barcodeAsInt);

            retVal = showKeychainStatusView(barcodeAsInt);
            qDebug () << "MainWindow::showKeychainStatusView: " << retVal;
        }
        else
        {
            // todo: recognised unknown barcode with a legit barcode format -> create dialog to add this keychain to db
            return;
        }

    }
}

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

    if (!mLayout)
        return;

    mLastView = mLayout->currentIndex();
    mLayout->setCurrentWidget (view);
}

MainWindow::~MainWindow()
{
    if (mDatabase)
        delete mDatabase;

    if (mDataHandover)
        delete mDataHandover;

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

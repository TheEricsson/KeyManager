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

#include "QZXing.h"

#ifndef GMANDANTID
    #define GMANDANTID 1
#endif

#ifdef ENCODERTEST
#include "tests/qrencodertest.h"
#endif

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
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

    mLayout = 0;
    mDatabase = 0;

    initDatabase ();

    mLayout = new QStackedLayout();
    setLayout(mLayout);

    mHomeView = new HomeView ();
    mScanView = new ScannerView ();
    mTableView = new TableView ("addresses");
    mRecipientView = new RecipientView ();
    mKeychainStatusView = new KeychainStatusView ();
    mAddRecipientView = new AddRecipientView ();

    mLayout->addWidget(mHomeView);
    mLayout->addWidget(mScanView);
    mLayout->addWidget(mTableView);
    mLayout->addWidget(mRecipientView);
    mLayout->addWidget(mKeychainStatusView);
    mLayout->addWidget(mAddRecipientView);

    mLayout->setCurrentWidget(mHomeView);

    // handle signals by ScanView
    connect (mScanView, SIGNAL (scanButtonClicked()), this, SLOT (showScannerView()));
    connect (mScanView, SIGNAL(previousButtonClicked()), this, SLOT (closeScannerView()));
    connect (mScanView, SIGNAL(nextButtonClicked()), this, SLOT (handleScannedKey()));

    // handle signals by HomeView
    connect (mHomeView,SIGNAL(showScannerView()), this, SLOT(showScannerView()));
    connect (mHomeView,SIGNAL(showTableView()), this, SLOT(showTableView()));

    // handle signals by TableView
    connect (mTableView, SIGNAL(previousButtonClicked()), this, SLOT (closeTableView()));

    // handle signals by KeychainStatusView
    connect (mKeychainStatusView, SIGNAL(previousButtonClicked()), this, SLOT (closeKeychainStatusView()));
    //connect (mKeychainStatusView, SIGNAL(nextButtonClicked()), this, SLOT (closeKeychainStatusView()));
    connect (mKeychainStatusView, SIGNAL(nextButtonClicked()), this, SLOT (showRecipientView()));

    // handle signals by RecipientView
    connect (mRecipientView, SIGNAL(previousButtonClicked()), this, SLOT (closeRecipientView()));
    connect (mRecipientView, SIGNAL(newRecipientButtonClicked()), this, SLOT (showAddRecipientView()));

    // handle signals by AddRecipientView
    connect (mAddRecipientView, SIGNAL(previousButtonClicked()), this, SLOT (closeAddRecipientView()));
    connect (mAddRecipientView, SIGNAL(okButtonClicked()), this, SLOT (addRecipientViewSubmitted()));
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

    mLayout->setCurrentWidget(mHomeView);
}

void MainWindow::closeTableView ()
{
    mLayout->setCurrentWidget(mHomeView);
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

    mLayout->setCurrentWidget(mScanView);

    mCameraInstance->startCamera();
    mGrabTimer->start();
    mScanView->setScannerState(ScannerState::SCANNING);
}

void MainWindow::showTableView ()
{
    mLayout->setCurrentWidget(mTableView);
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

    mLayout->setCurrentWidget(mRecipientView);
}

void MainWindow::closeRecipientView ()
{
    qDebug () << "MainWindow::closeRecipientView ()";
    if (!mRecipientView)
    {
        qDebug () << "MainWindow::closeRecipientView () - NULL pointer";
        return;
    }

    showScannerView();
}

void MainWindow::showAddRecipientView ()
{
    if (!mAddRecipientView)
        return;

    if (!mLayout)
        return;

    mLayout->setCurrentWidget(mAddRecipientView);
}

void MainWindow::closeAddRecipientView ()
{
    if (!mHomeView)
        return;

    if (!mLayout)
        return;

    mLayout->setCurrentWidget(mHomeView);
}

void MainWindow::addRecipientViewSubmitted ()
{
    if (!mAddRecipientView)
        return;

    if (!mDatabase)
        return;

    mDatabase->addNewRecipient (mAddRecipientView->getRecipientData());
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
        bool retVal = mKeychainStatusView->setKeysModel(mKeysOverviewModel);
        if (false == retVal)
            return false;

        if (mDatabase->initKeychainModel(mKeychainModel, aBarcode))
        {
            retVal = mKeychainStatusView->setKeychainModel(mKeychainModel);
            mLayout->setCurrentWidget(mKeychainStatusView);

            int keyChainStatus = mDatabase->getKeychainStatusId (aBarcode);
            mKeychainStatusView->setKeychainStatus (keyChainStatus);
        }
        else
            return false;

        return retVal;
    }
    else
        return false;
}

void MainWindow::closeKeychainStatusView ()
{
    showScannerView();
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
    //qDebug () << "MainWindow::decodeImage called";

    Q_UNUSED(requestId);

// no cam on windows pc -> use image for debugging
#ifdef Q_OS_WIN64
    QImage scaledImg (":/images/barcode.png");
#else
    QImage scaledImg = img.scaled(mScanView->getViewfinderSize(), Qt::KeepAspectRatio, Qt::FastTransformation);
#endif

    QZXing decoder;

    decoder.setDecoder(QZXing::DecoderFormat_CODE_128);

    // //optional settings
    decoder.setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    decoder.setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);

    //trigger decode
    QString barcode = decoder.decodeImage(scaledImg);
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

MainWindow::~MainWindow()
{
    if (mDatabase)
        delete mDatabase;

    if (mKeychainStatusView)
        delete mKeychainStatusView;

    if (mKeysOverviewModel)
        delete mKeysOverviewModel;

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

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

#include "homeview.h"
#include "camera.h"
#include "databaseimpl.h"
#include "tableview.h"
#include "keychainstatusview.h"

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

    mLayout = 0;
    mDatabase = 0;

    initDatabase ();

    mLayout = new QStackedLayout();
    setLayout(mLayout);

    mHomeView = new HomeView ();
    mScanView = new ScannerView ();
    mTableView = new TableView ("addresses");

    mLayout->addWidget(mHomeView);
    mLayout->addWidget(mScanView);
    mLayout->addWidget(mTableView);

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

void MainWindow::showKeychainStatusView (int aLocale, int aKeychainId)
{
    Q_UNUSED(aLocale);
    Q_UNUSED(aKeychainId);

    if (!mKeychainStatusView)
    {
        mKeychainStatusView = new KeychainStatusView ();
        mLayout->addWidget(mKeychainStatusView);
        connect (mKeychainStatusView, SIGNAL(previousButtonClicked()), this, SLOT (closeKeychainStatusView()));
    }

    mLayout->setCurrentWidget(mKeychainStatusView);
}

void MainWindow::closeKeychainStatusView ()
{
    mLayout->setCurrentWidget(mHomeView);
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
    QString result = decoder.decodeImage(scaledImg);

    QString customerId = result.mid (0, 5);
    QString keyId = result.mid (6, 5);

    if ("" != result.toStdString())
    {
        qDebug () << "Data:" << result.toStdString();
        qDebug () << "Mandant Id:" << customerId << "(Configured:" << GMANDANTID << ")";
        qDebug () << "Key Id:" << keyId;
    }

    if (customerId.toInt() == GMANDANTID)
    {
        mGrabTimer->stop ();

        // code recognized: play a supermarket beep sound :)
        playSound ();

        // set scanview ui state
        mScanView->setScannerState(ScannerState::SCANSUCCEEDED);
        mScanView->setCustomerLabel(customerId);
        mScanView->setKeyLabel(keyId);

        // search key in database
        bool result = mDatabase->findKeyCode(mScanView->getCustomerLabel().toInt(), mScanView->getKeyLabel().toInt());

        // check if barcode is known
        if (!result)
        {
            //barcode is recognised, but unknown in database
            //-> dialog - add to db...
        }
        else
        {
            //barcode is recognised and found in database
            bool retVal = mDatabase->setKeyCode(customerId.toInt(), keyId.toInt());
            qDebug () << "mDatabase->setKeyCode" << retVal;
            showKeychainStatusView(customerId.toInt(), keyId.toInt());
        }
    }
}

void MainWindow::playSound ()
{
    QMediaPlayer *player = new QMediaPlayer;
    QAudioOutput *audioOut = new QAudioOutput;
    player->setAudioOutput(audioOut);
    QUrl filelocation ("qrc:/sounds/scanner_beep.mp3");
    player->setSource(filelocation);
    audioOut->setVolume(100);
    player->play();
}

MainWindow::~MainWindow()
{
    if (mDatabase)
        delete mDatabase;
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

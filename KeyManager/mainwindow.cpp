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

#include "homeview.h"
#include "camera.h"
#include "keyScannedView.h"

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
    mKeyScannedView = 0;
    mLayout = 0;

    mLayout = new QStackedLayout();
    setLayout(mLayout);

    mHomeView = new HomeView ();
    mScanView = new ScannerView ();
    mKeyScannedView = new KeyScannedView ();

    mLayout->addWidget(mHomeView);
    mLayout->addWidget(mScanView);
    mLayout->addWidget(mKeyScannedView);

    mLayout->setCurrentWidget(mHomeView);

    // handle signals by ScanView
    //connect (mScanView,SIGNAL(codeRetrieved(int)), this,SLOT(onKeyScanned()));
    connect (mScanView, SIGNAL(abortScanner()), this, SLOT (closeScannerView()));

    // handle signals by HomeView
    connect (mHomeView,SIGNAL(showScannerView()), this, SLOT(showScannerView()));
}

void MainWindow::onKeyScanned ()
{
    mLayout->setCurrentWidget(mKeyScannedView);
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

void MainWindow::showScannerView ()
{
    if (!mScanView)
        return;

    if (!mCameraInstance)
    {
        mCameraInstance = new Camera ();
        mCameraInstance->setVideoOutput(mScanView->getViewfinder());

        connect (mCameraInstance->getImageCapture(), &QImageCapture::imageCaptured, this, &MainWindow::decodeImage);
    }

    if (!mGrabTimer)
    {
        mGrabTimer = new QTimer (this);
        mGrabTimer->setInterval(500);
        connect (mGrabTimer, SIGNAL(timeout()), mCameraInstance, SLOT(takePicture()));      
    }

    mLayout->setCurrentWidget(mScanView);

    mCameraInstance->startCamera();
    mGrabTimer->start();
}

void MainWindow::onSearchButtonReleased ()
{

}

void MainWindow::onManageButtonReleased ()
{

}

void MainWindow::decodeImage (int requestId, const QImage &img)
{
    qDebug () << "MainWindow::decodeImage called";

    Q_UNUSED(requestId);

    QImage scaledImg = img.scaled(mScanView->getViewfinderSize(), Qt::KeepAspectRatio, Qt::FastTransformation);

    QZXing decoder;

    decoder.setDecoder(QZXing::DecoderFormat_CODE_128);

    // //optional settings
    decoder.setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    decoder.setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);

    //trigger decode
    QString result = decoder.decodeImage(scaledImg);

    QString mandantId = result.mid (1, 4);
    QString keyId = result.mid (6, 4);

    qDebug () << "Data:" << result.toStdString();
    qDebug () << "Mandant Id:" << mandantId.toInt() << "(Configured:" << GMANDANTID << ")";
    qDebug () << "Key Id:" << keyId.toInt();

    // qDebug () << "Data: " << result.toStdString();

    // if (result.contains("M0100", Qt::CaseInsensitive))
    // {
    //     qDebug () << "Success! Code is valid: " << result;

    //     // code recognized: play a supermarket beep sound :)
    //     QMediaPlayer *player = new QMediaPlayer;
    //     QAudioOutput *audioOut = new QAudioOutput;
    //     player->setAudioOutput(audioOut);
    //     QUrl filelocation ("qrc:/sounds/scanner_beep.mp3");
    //     player->setSource(filelocation);
    //     audioOut->setVolume(100);
    //     player->play();

    //     m_DecoderTimer->stop();

    //     int code = 50;

    //     emit codeRetrieved (code);
    // }
}

MainWindow::~MainWindow()
{
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

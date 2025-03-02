#include "mainwindow.h"
#include "scannerView.h"
#include <QPushButton>
#include <QGridLayout>
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedLayout>
#include <QTimer>

#include "homeview.h"
#include "camera.h"
#include "keyScannedView.h"

#ifdef ENCODERTEST
#include "tests/qrencodertest.h"
#endif

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    mCameraInstance = 0;
    mGrabTimer = 0;

    mLayout = new QStackedLayout();
    setLayout(mLayout);

    mHomeView = new HomeView ();
    mScanView = new ScannerView ();
    mKeyScannedView = new KeyScannedView ();

    //mScanView->setCamera(mCameraInstance->getCamera());

    mLayout->addWidget(mHomeView);
    mLayout->addWidget(mScanView);
    mLayout->addWidget(mKeyScannedView);

    //mLayout->setCurrentWidget(scanView);
    //scanView->startScanning();

    mLayout->setCurrentWidget(mHomeView);

    // handle signals by ScanView
    //connect (mScanView,SIGNAL(codeRetrieved(int)), this,SLOT(onKeyScanned()));
    connect (mScanView, SIGNAL(closeScannerView()), this, SLOT (closeScannerView()));

    // handle signals by HomeView
    connect (mHomeView,SIGNAL(showScannerView()), this, SLOT(showScannerView()));
}

void MainWindow::onKeyScanned ()
{
    mLayout->setCurrentWidget(mKeyScannedView);
}

void MainWindow::closeScannerView ()
{
    mLayout->setCurrentWidget(mHomeView);

    if (mGrabTimer)
    {
        mGrabTimer->stop();
        delete mGrabTimer;
        mGrabTimer = 0;
    }
    if (mCameraInstance)
    {
        delete mCameraInstance;
        mCameraInstance = 0;
    }
}

void MainWindow::showScannerView ()
{
    if (!mCameraInstance)
    {
        mCameraInstance = new Camera ();
    }

    if (!mGrabTimer)
    {
        mGrabTimer = new QTimer (this);
        mGrabTimer->setInterval(500);
    }

    connect (mGrabTimer, SIGNAL(timeout()), mCameraInstance, SLOT(takePicture()));
    connect (mCameraInstance->getImageCapture(), &QImageCapture::imageCaptured, this, &MainWindow::decodeImage);

    mScanView->setVideoOutput(mCameraInstance->getCaptureSession());
    mLayout->setCurrentWidget(mScanView);
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

    // Q_UNUSED(requestId);

    // QImage scaledImage = img.scaled(m_viewfinder->size(), Qt::KeepAspectRatio, Qt::FastTransformation);

    // QZXing decoder;
    // //mandatory settings
    // decoder.setDecoder(QZXing::DecoderFormat_CODE_128);

    // //optional settings
    // decoder.setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    // decoder.setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);

    // m_lastPhoto->setPixmap(QPixmap::fromImage(scaledImage));
    // ;
    // //trigger decode
    // QString result = decoder.decodeImage(scaledImage);

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

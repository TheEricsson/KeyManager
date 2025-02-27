#include "mainwindow.h"
#include "scannerView.h"
#include <QPushButton>
#include <QGridLayout>
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedLayout>
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

    mLayout = new QStackedLayout();
    setLayout(mLayout);

    mHomeView = new HomeView ();
    mScanView = new ScannerView ();
    mKeyScannedView = new KeyScannedView ();

    mCameraInstance = new Camera ();

    mScanView->setCamera(mCameraInstance->getCamera());

    mLayout->addWidget(mHomeView);
    mLayout->addWidget(mScanView);
    mLayout->addWidget(mKeyScannedView);

    //mLayout->setCurrentWidget(scanView);
    //scanView->startScanning();

    mLayout->setCurrentWidget(mHomeView);

    // handle signals by ScanView
    connect (mScanView,SIGNAL(codeRetrieved(int)), this,SLOT(onKeyScanned()));
    connect (mScanView, SIGNAL(closeScannerView()), this, SLOT (showHomeView()));

    // handle signals by HomeView
    connect (mHomeView,SIGNAL(showScannerView()), this, SLOT(showScannerView()));
}

void MainWindow::onKeyScanned ()
{
    mLayout->setCurrentWidget(mKeyScannedView);
}

void MainWindow::showHomeView ()
{
    mLayout->setCurrentWidget(mHomeView);
}

void MainWindow::showScannerView ()
{
    mLayout->setCurrentWidget(mScanView);
    mScanView->startScanning ();
}

void MainWindow::onSearchButtonReleased ()
{

}

void MainWindow::onManageButtonReleased ()
{

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

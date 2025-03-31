#include "scannerview.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QCamera>
#include <QMediaDevices>
#include <QDebug>
#include <QStackedWidget>
#include <QLabel>
#include <QVideoWidget>
#include <QPermission>
#include <QApplication>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QScreenCapture>
#include <QLineEdit>
#include "winsubmenu.h"
#include "camera.h"
#include "dataobject.h"
#include "viewdatascanner.h"
#include "viewdata.h"

#if QT_CONFIG(permissions)
#include <QPermission>
#endif

ScannerView::ScannerView(QWidget *parent)
    : WinSubmenu {parent}
{
    mCameraInstance = 0;
    mGrabTimer = 0;

    decoder.setDecoder(QZXing::DecoderFormat_CODE_128);
    //optional settings
    decoder.setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    decoder.setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);

    setHeader("Scannen Sie einen Barcode");

    m_viewfinder = 0;

    // layout for the cam video/ pic screen
    m_viewfinder = new QVideoWidget ();
    m_viewfinder->setMinimumSize(300, 150);
    m_viewfinder->setAspectRatioMode(Qt::AspectRatioMode::KeepAspectRatioByExpanding);
    m_viewfinder->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    m_viewfinder->setContentsMargins(0, 0, 0, 0);

    layout()->addWidget(m_viewfinder);

    // mandant text field
    QHBoxLayout *mandantIdLayout = new QHBoxLayout;
    QLabel *mandantDescr = new QLabel ("Mandant", this);
    mCustomerLabel = new QLabel (this);
    mandantIdLayout->addWidget (mandantDescr);
    mandantIdLayout->addWidget(mCustomerLabel);

    layout()->addItem(mandantIdLayout);

    // key text field
    QHBoxLayout *keyIdLayout = new QHBoxLayout;
    QLabel *keyDescr = new QLabel ("Schlüssel", this);
    mKeyLabel = new QLabel (this);
    keyIdLayout->addWidget (keyDescr);
    keyIdLayout->addWidget(mKeyLabel);

    layout()->addItem(keyIdLayout);

    //setMenuButtons(Gui::Back, Gui::Repeat);
    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    setScannerState (ScannerState::READY);
}

void ScannerView::showEvent(QShowEvent *)
{
    startScanner ();
}

void ScannerView::hideEvent(QHideEvent *)
{
    stopScanner();
}

void ScannerView::startScanner ()
{
    if (!mCameraInstance)
    {
        mCameraInstance = new Camera ();
        mCameraInstance->setVideoOutput(getViewfinder());
    }

    if (!mGrabTimer)
    {
        mGrabTimer = new QTimer (this);
        mGrabTimer->setInterval(500);
        connect (mGrabTimer, SIGNAL(timeout()), this, SLOT(decodeFromVideoFrame()));
    }

    mCameraInstance->startCamera();
    mGrabTimer->start();

    setScannerState (ScannerState::SCANNING);
}

void ScannerView::stopScanner ()
{
    if (mGrabTimer)
        mGrabTimer->stop();

    if (mCameraInstance)
        mCameraInstance->stopCamera();

    setScannerState (ScannerState::READY);
}

void ScannerView::decodeFromVideoFrame ()
{
    QImage capture = mCameraInstance->getImageFromVideoframe ();
#ifdef Q_OS_WIN64
    // no cam on windows pc -> use test image with barcode
    QImage testCode (":/images/barcode.png");
    QString barcode = decoder.decodeImage(testCode);
#else
    QString barcode = decoder.decodeImage(capture);
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
        //playSound ();

        setKeyLabel(barcodeAsNumber);

        // set scanview ui state
        setScannerState(ScannerState::SCANSUCCEEDED);

        //emit keycodeRecognised (barcodeAsInt);
        ViewDataScanner *scannerData = new ViewDataScanner ();
        scannerData->setBarcode(barcodeAsInt);
        getViewData()->setData (scannerData);
        //emit viewDataChanged (scannerData);
    }
}

void ScannerView::setScannerState (ScannerState aStatus)
{
    mScannerState = aStatus;

    switch (mScannerState)
    {
        case READY:
            qDebug () << "ScannerState is READY";
            mCustomerLabel->setText("---");
            mKeyLabel->setText("---");
            break;
        case SCANNING:
            mCustomerLabel->setText("---");
            mKeyLabel->setText("---");
            qDebug() <<  "ScannerState is SCANNING";
            break;
        case SCANSUCCEEDED:
            qDebug() <<  "ScannerState is SCANSUCCEEDED";
            break;
    }
}

QSize ScannerView::getViewfinderSize ()
{
    return m_viewfinder->size();
}

QVideoWidget* ScannerView::getViewfinder ()
{
    if (!m_viewfinder)
        return 0;
    return m_viewfinder;
}

void ScannerView::setCustomerLabel (QString aCustomerId)
{
    mCustomerLabel->setText(aCustomerId);
}

void ScannerView::setKeyLabel (QString aKeyId)
{
    mKeyLabel->setText(aKeyId);
}

const QString ScannerView::getCustomerLabel ()
{
    return mCustomerLabel->text();
}

const QString ScannerView::getKeyLabel()
{
    return mKeyLabel->text();
}

ScannerView::~ScannerView()
{
    if (m_viewfinder != 0)
    {
        delete m_viewfinder;
    }
}

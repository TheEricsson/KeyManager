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
#include "datainterface.h"

#if QT_CONFIG(permissions)
#include <QPermission>
#endif

ScannerView::ScannerView(QWidget *parent)
    : WinSubmenu {parent}
{
    mCameraInstance = 0;
    mGrabTimer = 0;
    mScannerData = 0;

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
    menuButtons.append(Gui::Repeat);
    menuButtons.append(Gui::NewCode);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);
    hideButton(2, true);
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

void ScannerView::onMenuBtnClicked (Gui::MenuButton btnType)
{
    if (Gui::Repeat == btnType)
    {
        startScanner();
    }
    else
        WinSubmenu::menuButtonClicked(btnType);
}

void ScannerView::decodeFromVideoFrame ()
{
    QImage capture = mCameraInstance->getImageFromVideoframe ();
#ifdef Q_OS_WIN64
    // no cam on windows pc -> use test image with barcode
    //QImage testCode (":/images/barcode00010150.png");
    QImage testCode (":/images/barcode00010151.png");
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

        //emit keycodeRecognised (barcodeAsInt);
        if (mScannerData)
        {
            delete mScannerData;
            mScannerData = 0;
        }
        mScannerData = new ViewDataScanner ();
        mScannerData->setBarcode(barcodeAsInt);
        dataInterface()->setData (mScannerData);

        // set scanview ui state
        setScannerState(ScannerState::SCANSUCCEEDED);
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
            disableButton(1, false);
            disableButton(2, true);
            disableButton(3, true);
            break;
        case SCANNING:
            mCustomerLabel->setText("---");
            mKeyLabel->setText("---");
            qDebug() <<  "ScannerState is SCANNING";
            disableButton(1, true);
            disableButton(2, true);
            disableButton(3, true);
            break;
        case SCANSUCCEEDED:
            qDebug() <<  "ScannerState is SCANSUCCEEDED";
            int code = dataInterface()->getScannedCode();
            bool foundCode = dataInterface()->findKeycode(code);

            if (foundCode)
            {
                hideButton(2, true);
                //hideButton(3, false);
                enableButton(1, true);
                enableButton(3, true);
            }
            else
            {
                showButton(2, true);
                //hideButton(3, true);
                enableButton(1, true);
                enableButton(2, true);
                enableButton(3, false);
            }

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
        m_viewfinder = 0;
    }

    // if (mScannerData != 0)
    // {
    //     delete mScannerData;
    //     mScannerData = 0;
    // }
}

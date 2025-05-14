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
#include <QScreenCapture>
#include <QLineEdit>
#include <QKeyEvent>
#include <QAudioDevice>
#include "winsubmenu.h"
#include "camera.h"
#include "dataobject.h"
#include "viewdatascanner.h"
#include "datainterface.h"
#include "iointerface.h"

#if QT_CONFIG(permissions)
#include <QPermission>
#endif

ScannerView::ScannerView(QWidget *parent)
    : WinSubmenu {parent}
{
    mCameraInstance = 0;
    mGrabTimer = 0;
    mScannerData = 0;

    decoder.setDecoder(QZXing::DecoderFormat_QR_CODE);
    //optional settings
    decoder.setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    decoder.setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);

    //setHeader("Scannen Sie einen Barcode");

    m_viewfinder = 0;

    // layout for the cam video/ pic screen
    m_viewfinder = new QVideoWidget (this);
    m_viewfinder->setMinimumSize(300, 150);

    m_viewfinder->setAspectRatioMode(Qt::AspectRatioMode::KeepAspectRatioByExpanding);
    m_viewfinder->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    m_viewfinder->setContentsMargins(0, 0, 0, 0);

    // code text field
    QLabel *codeDescr = new QLabel ("Daten", this);
    mCodeLabel = new QLabel (this);

    // mandant text field
    QLabel *groupDescr = new QLabel ("Gruppe", this);
    mGroupLabel = new QLabel (this);

    // key text field
    QLabel *keyDescr = new QLabel ("Schlüsselnummer", this);
    mKeyLabel = new QLabel (this);

    QGridLayout* centralLayout = new QGridLayout();
    centralLayout->addWidget(m_viewfinder, 0, 0, 1, 2);
    centralLayout->addWidget (codeDescr, 1, 0);
    centralLayout->addWidget(mCodeLabel, 1, 1);
    centralLayout->addWidget (groupDescr, 2, 0);
    centralLayout->addWidget(mGroupLabel, 2, 1);
    centralLayout->addWidget (keyDescr, 3, 0);
    centralLayout->addWidget(mKeyLabel, 3, 1);

    setCentralLayout(centralLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Repeat);
    menuButtons.append(Gui::NewCodeScanned);
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
    }
    mCameraInstance->reset();
    mCameraInstance->setVideoOutput(getViewfinder());

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
    {
        mCameraInstance->stopCamera();
        delete mCameraInstance;
        mCameraInstance = 0;
    }

    setScannerState (ScannerState::READY);
}

bool ScannerView::codeIsValid(const unsigned int code)
{
    if (code <= 99999999 && code >= 10001)
        return true;
    else
        return false;
}

void ScannerView::playSound()
{
    const auto devices = QMediaDevices::audioOutputs();
    for (const QAudioDevice &device : devices)
        qDebug() << "Device: " << device.description();

    mAudioOut.setDevice(QMediaDevices::defaultAudioOutput());
    mAudioOut.setMuted(false);
    mAudioOut.setVolume(100);
    QUrl filelocation ("qrc:/sounds/scanner_beep.mp3");
    mPlayer.setSource(filelocation);
    mPlayer.setAudioOutput(&mAudioOut);
    mPlayer.play();
}

void ScannerView::keyReleaseEvent(QKeyEvent *event)
{
    //override android key release event for back button
    //stop camera before quitting view
    switch (event->key())
    {
        case Qt::Key_Back:
            onMenuBtnClicked(Gui::Back);
            break;
        default:
            break;
    }
}

void ScannerView::onMenuBtnClicked (Gui::MenuButton btnType)
{
    switch (btnType)
    {
        case Gui::Repeat:
            startScanner();
            break;
        case Gui::Back:
            stopScanner();
            emit menuButtonClicked(btnType);
            break;
        default:
            emit menuButtonClicked(btnType);
            break;
    }
}

void ScannerView::decodeFromVideoFrame ()
{
    QImage capture = mCameraInstance->getImageFromVideoframe ();
#ifdef Q_OS_WIN64
    // no cam on windows pc -> use test image with barcode
    //QImage testCode (":/images/barcode00010150.png");
    QImage testCode (":/images/qrcode_0001-0001.png");
    QString decodedString = decoder.decodeImage(testCode);
#else
    QString decodedString = decoder.decodeImage(capture);
#endif

    QString barcodeAsNumber = decodedString.mid (0, 4);
    barcodeAsNumber.append(decodedString.mid(5, 4));
    unsigned int barcodeAsInt = barcodeAsNumber.toInt();

    if ("" != decodedString.toStdString())
    {
        qDebug () << "barcode:" << decodedString;
        qDebug () << "barcodeAsNumber:" << barcodeAsNumber;
        qDebug () << "barcodeAsInt: = " << barcodeAsInt;
    }

    mCodeLabel->setText(decodedString);

    QString customerId = decodedString.mid (0, 4);
    QString keyId = decodedString.mid (5, 4);

    if  (codeIsValid(barcodeAsInt))
    {
        mGrabTimer->stop ();
        mCameraInstance->stopCamera();

        // code recognized: play a supermarket beep sound :)
        playSound ();

        setCustomerLabel(customerId);
        setKeyLabel(keyId);

        dataInterface()->resetScannerData();
        dataInterface()->setScannedCode (barcodeAsInt);

        // set scanview ui state
        setScannerState(ScannerState::SCANSUCCEEDED);
    }
    else
    {
        QString codeLabelWAppendix = decodedString + " (ungültig)";
        mCodeLabel->setText(codeLabelWAppendix);
    }
}

void ScannerView::setScannerState (ScannerState aStatus)
{
    mScannerState = aStatus;

    switch (mScannerState)
    {
        case READY:
            qDebug () << "ScannerState is READY";
            mGroupLabel->setText("---");
            mKeyLabel->setText("---");
            disableButton(1, false);
            disableButton(2, true);
            disableButton(3, true);
            break;
        case SCANNING:
            mGroupLabel->setText("---");
            mKeyLabel->setText("---");
            qDebug() <<  "ScannerState is SCANNING";
            disableButton(1, true);
            disableButton(2, true);
            disableButton(3, true);
            break;
        case SCANSUCCEEDED:
            qDebug() <<  "ScannerState is SCANSUCCEEDED";
            int code = dataInterface()->getScannedCode();
            bool foundCode = ioInterface()->findKeyCode(code);

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
    mGroupLabel->setText(aCustomerId);
}

void ScannerView::setKeyLabel (QString aKeyId)
{
    mKeyLabel->setText(aKeyId);
}

const QString ScannerView::getCustomerLabel ()
{
    return mGroupLabel->text();
}

const QString ScannerView::getKeyLabel()
{
    return mKeyLabel->text();
}

ScannerView::~ScannerView()
{
    if (m_viewfinder)
    {
        delete m_viewfinder;
        m_viewfinder = 0;
    }

    if (mCameraInstance)
    {
        delete mCameraInstance;
        mCameraInstance = 0;
    }
}

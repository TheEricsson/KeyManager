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
#include <QGroupBox>
#include <QFormLayout>
#include <QComboBox>
#include <QRadioButton>
#include <QButtonGroup>
#include "checkboxarray.h"
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
    mAvailableCameras = 0;
    mCameraInitDone = false;
    mDecoder = 0;

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

    // camera settings
    QGroupBox *camera = new QGroupBox(tr("Einstellungen"));
    mCamSettingsLayout = new QFormLayout();
    camera->setLayout(mCamSettingsLayout);

    // central layout of the view
    QGridLayout* centralLayout = new QGridLayout();
    centralLayout->addWidget(m_viewfinder, 0, 0, 1, 2);
    centralLayout->addWidget(camera, 1, 0, 1, 2);
    centralLayout->addWidget (codeDescr, 2, 0);
    centralLayout->addWidget(mCodeLabel, 2, 1);
    centralLayout->addWidget (groupDescr, 3, 0);
    centralLayout->addWidget(mGroupLabel, 3, 1);
    centralLayout->addWidget (keyDescr, 4, 0);
    centralLayout->addWidget(mKeyLabel, 4, 1);

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

void ScannerView::reset()
{
    mCodeLabel->setText("");
    setCustomerLabel("");
    setKeyLabel("");
    dataInterface()->resetScannerData();

    // if (0 != mAvailableCameras)
    // {
    //     if (0 != ioInterface())
    //     {
    //         int camId = ioInterface()->getDefaultCameraId();
    //         QRadioButton *btn = mAvailableCameras->getButtonAt(camId);
    //         if (0 != btn)
    //         {
    //             btn->setChecked(true);
    //         }
    //     }
    // }
}

void ScannerView::showEvent(QShowEvent *)
{
    // do camera init only one time at runtime
    if (!mCameraInitDone)
    {
        setAvailableCams();
    }
    // init decoder once

    if (!mDecoder)
    {
        mDecoder = new QZXing(this);
        mDecoder->setDecoder(QZXing::DecoderFormat_QR_CODE);
        //optional settings
        mDecoder->setSourceFilterType(QZXing::SourceFilter_ImageNormal);
        mDecoder->setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);
    }
    // reset view labels and scanner data
    reset();
    // start scanning
    startScanner();
}

void ScannerView::hideEvent(QHideEvent *)
{
    stopScanner();
}

void ScannerView::startScanner ()
{
    if (mCameraInstance)
    {
        if (mAvailableCameras)
        {
            mCameraInstance->setCameraDevice(mAvailableCameras->getCheckedButtonIndex());
            mCameraInstance->reset();
            mCameraInstance->setVideoOutput(getViewfinder());
            mCameraInstance->startCamera();
        }
    }

    if (!mGrabTimer)
    {
        mGrabTimer = new QTimer (this);
        mGrabTimer->setInterval(250);
        connect (mGrabTimer, SIGNAL(timeout()), this, SLOT(decodeFromVideoFrame()));
    }

    // wait half a second before capturing first frame
    // -> otherwise it may happen, that the old video frame is read (from the previous scan)
    mGrabTimer->start(500);

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

void ScannerView::setAvailableCams()
{
    qDebug() << "ScannerView::setAvailableCams()";

    QList<QCameraDevice> cams;

    if (!mCameraInstance)
    {
        mCameraInstance = new Camera (this);
    }

    if (mCameraInstance)
        cams = mCameraInstance->getAvailableCameraDevices();

    if (mCamSettingsLayout)
    {
        // clear layout
        while(mCamSettingsLayout->count() > 0)
        {
            QLayoutItem *item = mCamSettingsLayout->takeAt(0);

            if (item)
            {
                if (item->widget())
                {
                    delete item->widget();
                    delete item;
                }
            }
        }

        // at least one cam available
        if (0 < cams.size())
        {
            QVariant camId;
            QStringList camList;

            for (int i = 0; i < cams.size(); i++)
            {
                qDebug() << cams.value(i).description();
                camList.append(cams.value(i).description());
            }

            if(!mAvailableCameras)
            {
                mAvailableCameras = new CheckBoxArray(this);
                mAvailableCameras->init(camList);
            }

            int defaultCamId = ioInterface()->getDefaultCameraId();

            for (unsigned int j = 0; j < mAvailableCameras->count(); j++)
            {
                QRadioButton *btn = mAvailableCameras->getButtonAt(j);
                if (btn)
                {
                    //set default cam gui button checked
                    if (j == defaultCamId)
                    {
                        btn->setChecked(true);
                    }
                    mCamSettingsLayout->addWidget(btn);
                    connect(mAvailableCameras, SIGNAL(radioBtnToggled()), this, SLOT(onCameraChanged()));
                }
            }
        }
        // no cam found
        else
        {
            QLabel* noCam = new QLabel(tr("Keine Kamera verfügbar"));
            mCamSettingsLayout->addWidget(noCam);
        }
    }
    mCameraInitDone = true;
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

void ScannerView::onCameraChanged()
{
    updateDefaultCamSetting();
    stopScanner();
    startScanner();
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

void ScannerView::updateDefaultCamSetting()
{
    qDebug() << "ScannerView::updateDefaultCamSetting()";

    //update db setting, default value will always be the last selected cam
    if (mAvailableCameras)
    {
        if (ioInterface())
        {
            int selectedCamId = mAvailableCameras->getCheckedButtonIndex();
            ioInterface()->setDefaultCameraId(selectedCamId);
        }
    }
}

ScannerView::~ScannerView()
{
    /*if (m_viewfinder)
    {
        delete m_viewfinder;
        m_viewfinder = 0;
    }*/

    /*if (mCameraInstance)
    {
        delete mCameraInstance;
        mCameraInstance = 0;
    }*/
}

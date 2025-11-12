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
#include "keycodeinputdialog.h"

#if QT_CONFIG(permissions)
#include <QPermission>
#endif

ScannerView::ScannerView(QWidget *parent)
    : WinSubmenu {parent}
{
    mCameraInstance = 0;
    mGrabTimer = 0;
    mStartDelay = 0;
    mScannerData = 0;
    mAvailableCameras = 0;
    mCameraInitDone = false;
    mDecoder = 0;
    mPlayer = 0;
    mAudioOut = 0;

#ifdef Q_OS_WIN64
    mGroup = 10000;
    mCode = mGroup + 1;
#endif

    setHeader("Code-Scanner");

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
    QGroupBox *camera = new QGroupBox(tr("Kamera"));
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
    menuButtons.append(Gui::Edit);
    menuButtons.append(Gui::Repeat);
    menuButtons.append(Gui::NewCodeScanned);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);
    hideButton(3, true);
    //setScannerState (ScannerState::READY);
}

void ScannerView::reset()
{
    setScannerState(ScannerState::READY);
}

void ScannerView::showEvent(QShowEvent *)
{
    showFullScreen();

    // init camera at first show event
    if (!mCameraInitDone)
    {
        setAvailableCams();
    }

    // init decoder at first show event
    if (!mDecoder)
    {
        mDecoder = new QZXing(this);
        mDecoder->setDecoder(QZXing::DecoderFormat_QR_CODE);
        //optional settings
        mDecoder->setSourceFilterType(QZXing::SourceFilter_ImageNormal);
        mDecoder->setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);
    }

    // init sound player and output device on first show event
    if (!mAudioOut)
    {
        mAudioOut = new QAudioOutput (this);

        const auto devices = QMediaDevices::audioOutputs();
        for (const QAudioDevice &device : devices)
            qDebug() << "Device: " << device.description();

        mAudioOut->setDevice(QMediaDevices::defaultAudioOutput());
        mAudioOut->setMuted(false);
        mAudioOut->setVolume(100);
    }
    if (!mPlayer)
    {
        mPlayer = new QMediaPlayer (this);

        QUrl filelocation ("qrc:/sounds/scanner_beep.mp3");
        mPlayer->setSource(filelocation);
        mPlayer->setAudioOutput(mAudioOut);
    }

    // reset view labels and scanner data
    reset();
    // start scanning
    setScannerState(ScannerState::SCANNING);
}

void ScannerView::hideEvent(QHideEvent *)
{
    //setScannerState(ScannerState::STOPPED);
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

    if (!mStartDelay)
    {
        mStartDelay = new QTimer (this);
        mStartDelay->setSingleShot(true);
        connect (mStartDelay, SIGNAL(timeout()), this, SLOT(onStartDelayFinished()));
    }
    qDebug()<<"Delay started";
    mStartDelay->start(1000);
}

void ScannerView::stopScanner ()
{
    if (0 != mGrabTimer)
        mGrabTimer->stop();

    if (0 != mCameraInstance)
        mCameraInstance->stopCamera();

    //setScannerState (ScannerState::STOPPED);
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
    if (mPlayer)
    {
        mPlayer->play();
    }
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
                int defaultCamId = ioInterface()->getDefaultCameraId();
                mAvailableCameras->setButtonChecked(defaultCamId, true);
                connect(mAvailableCameras, SIGNAL(radioBtnToggled(int)), this, SLOT(onCameraChanged(int)));

                if (mCamSettingsLayout)
                {
                    mCamSettingsLayout->addWidget(mAvailableCameras);
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
            setScannerState(ScannerState::SCANNING);
            break;
        case Gui::Back:
            setScannerState(ScannerState::STOPPED);
            emit menuButtonClicked(btnType);
            break;
        case Gui::Edit:
            setScannerState(ScannerState::STOPPED);
            setCodeManual();
        default:
            emit menuButtonClicked(btnType);
            break;
    }
}

void ScannerView::onCameraChanged(int camId)
{
    setDefaultCam(camId);
    setScannerState(ScannerState::SCANNING);
}

void ScannerView::onStartDelayFinished()
{
    qDebug()<<"Delay finished";

    if (!mGrabTimer)
    {
        mGrabTimer = new QTimer (this);
        mGrabTimer->setInterval(50);
        connect (mGrabTimer, SIGNAL(timeout()), this, SLOT(decodeFromVideoFrame()));
    }
    mGrabTimer->start();
}

void ScannerView::decodeFromVideoFrame ()
{
    QImage capture;
    if (0 != mCameraInstance)
    {
        capture = mCameraInstance->getImageFromVideoframe ();
    }
#ifdef QT_DEBUG
    // debugging without cam on windows pc -> use test image with barcode
    // QImage testCode (":/images/qrcode_0001-0001.png");
    QString decodedString = "debug mode without cam";
    // mCode++;
#else
    QString decodedString ("");
    if (0 != mDecoder)
    {
        decodedString = mDecoder->decodeImage(capture);
    }
#endif

    QString barcodeAsNumber = decodedString.mid (0, 4);
    barcodeAsNumber.append(decodedString.mid(5, 4));
    unsigned int barcodeAsInt = barcodeAsNumber.toInt();

    /*if ("" != decodedString.toStdString())
    {
        qDebug () << "barcode:" << decodedString;
        qDebug () << "barcodeAsNumber:" << barcodeAsNumber;
        qDebug () << "barcodeAsInt: = " << barcodeAsInt;
    }*/

    if (0 != mCodeLabel)
    {
        mCodeLabel->setText(decodedString);
    }

    QString customerId = decodedString.mid (0, 4);
    QString keyId = decodedString.mid (5, 4);

    if  (codeIsValid(barcodeAsInt))
    {
        if (0 != mGrabTimer)
            mGrabTimer->stop ();

        if (0 != mCameraInstance)
            mCameraInstance->stopCamera();

        // code recognized: play a supermarket beep sound :)
        playSound ();

        setCustomerLabel(customerId);
        setKeyLabel(keyId);

        dataInterface()->resetScannerData();

        if (dataInterface()->setScannedCode (barcodeAsInt))
        {
            // set scanview ui state
            setScannerState(ScannerState::SCANSUCCEEDED);
        }
    }
    else
    {
        QString codeLabelWAppendix = decodedString + " (ungültig)";

        if (0 != mCodeLabel)
        {
            mCodeLabel->setText(codeLabelWAppendix);
        }
    }
}

void ScannerView::setScannerState (ScannerState aStatus)
{
    mScannerState = aStatus;

    switch (mScannerState)
    {
        case READY:
        case STOPPED:
            disableButton(1, false);
            disableButton(2, false);
            disableButton(3, true);
            disableButton(4, true);
            stopScanner();
            qDebug () << "ScannerState is READY";
            break;
        case SCANNING:
            resetLabels();
            dataInterface()->resetScannerData();
            startScanner();

            disableButton(1, false);
            disableButton(2, true);
            disableButton(3, true);
            disableButton(4, true);

            qDebug() <<  "ScannerState is SCANNING";
            break;
        case SCANSUCCEEDED:
            int code = dataInterface()->getScannedCode();
            bool foundCode = ioInterface()->findKeyCode(code);

            if (foundCode)
            {
                hideButton(3, true);
                //hideButton(3, false);
                enableButton(1, true);
                enableButton(2, true);
                enableButton(4, true);
            }
            else
            {
                showButton(3, true);
                //hideButton(3, true);
                enableButton(1, true);
                enableButton(2, true);
                enableButton(3, true);
                enableButton(4, false);
            }

            stopScanner();
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
    return m_viewfinder;
}

void ScannerView::setCodeLabel(QString aCodeLabel)
{
    mCodeLabel->setText(aCodeLabel);
}

void ScannerView::setCustomerLabel (QString aCustomerId)
{
    mGroupLabel->setText(aCustomerId);
}

void ScannerView::setKeyLabel (QString aKeyId)
{
    mKeyLabel->setText(aKeyId);
}

void ScannerView::setCodeManual()
{
    KeycodeInputDialog dlg;
    dlg.setInputMode(QInputDialog::TextInput);
    dlg.setLabelText("Code manuell eingeben:");
    if (QDialog::Accepted == dlg.exec())
    {
        QString decodedString = dlg.textValue();
        QString barcodeAsNumber = decodedString.mid (0, 4);
        barcodeAsNumber.append(decodedString.mid(5, 4));
        unsigned int barcodeAsInt = barcodeAsNumber.toInt();

        if ("" != decodedString.toStdString())
        {
            qDebug () << "barcode:" << decodedString;
            qDebug () << "barcodeAsNumber:" << barcodeAsNumber;
            qDebug () << "barcodeAsInt: = " << barcodeAsInt;
        }

        if (0 != mCodeLabel)
        {
            mCodeLabel->setText(decodedString);
        }

        QString customerId = decodedString.mid (0, 4);
        QString keyId = decodedString.mid (5, 4);

        if  (codeIsValid(barcodeAsInt) && dlg.isCodeValid())
        {
            if (0 != mGrabTimer)
                mGrabTimer->stop ();

            if (0 != mCameraInstance)
                mCameraInstance->stopCamera();

            // code recognized: play a supermarket beep sound :)
            playSound ();

            setCustomerLabel(customerId);
            setKeyLabel(keyId);

            dataInterface()->resetScannerData();

            if (dataInterface()->setScannedCode (barcodeAsInt))
            {
                // set scanview ui state
                setScannerState(ScannerState::SCANSUCCEEDED);
            }
        }
        else
        {
            QString codeLabelWAppendix = decodedString + " (ungültig)";

            if (0 != mCodeLabel)
            {
                mCodeLabel->setText(codeLabelWAppendix);
            }
            setScannerState(ScannerState::SCANNING);
        }

    }
    else
        setScannerState(ScannerState::SCANNING);
}

const QString ScannerView::getCustomerLabel ()
{
    return mGroupLabel->text();
}

const QString ScannerView::getKeyLabel()
{
    return mKeyLabel->text();
}

void ScannerView::setDefaultCam (int camId)
{
    qDebug() << "ScannerView::updateDefaultCamSetting()";

    //update db setting, default value will always be the last selected cam
    if (mAvailableCameras)
    {
        if (ioInterface())
        {
            ioInterface()->setDefaultCameraId(camId);
        }
    }
}

void ScannerView::resetLabels()
{
    setCodeLabel("");
    setCustomerLabel("");
    setKeyLabel("");
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

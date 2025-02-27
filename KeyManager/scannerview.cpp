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
#include "QZXing.h"
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>

#if QT_CONFIG(permissions)
#include <QPermission>
#endif

ScannerView::ScannerView(QWidget *parent)
    : QWidget{parent}
{
    m_viewfinder = 0;
    m_DecoderTimer = 0;
    m_ImageCapture = 0;

    m_DecoderTimer = 0;
    m_scanFrequency = 1000;

    // main Layout
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QLabel* header = new QLabel("Code Scanner");
    layout->addWidget(header);

    //checkPermissions();
    //checkAvailableCams ();

    // layout for the cam video/ pic screen
    m_viewfinder = new QVideoWidget ();

    layout->addWidget(m_viewfinder);

    layout->setStretch(1, 5);

    m_lastPhoto = new QLabel ("");

    layout->addWidget(m_lastPhoto);

    QPushButton* btnAbort = new QPushButton ("Abbrechen");
    layout->addWidget(btnAbort);

    m_ImageCapture = new QImageCapture ();
    m_captureSession.setImageCapture(m_ImageCapture);
    connect(m_ImageCapture, &QImageCapture::imageCaptured, this, &ScannerView::processCapturedImage);

    m_DecoderTimer = new QTimer (this);
    connect (m_DecoderTimer, &QTimer::timeout, this, &ScannerView::takePicture);

    connect (btnAbort, SIGNAL (clicked()), this, SLOT (onAbortBtnClicked()));

    // start scanning immediately
    //m_DecoderTimer->start (m_scanFrequency);
}

void ScannerView::onAbortBtnClicked ()
{
    stopScanning ();
    emit closeScannerView ();
}

void ScannerView::startScanning ()
{
    if (0 != m_DecoderTimer)
        m_DecoderTimer->start (m_scanFrequency);
}

void ScannerView::stopScanning ()
{
    if (0 != m_DecoderTimer)
        m_DecoderTimer->stop ();
}

void ScannerView::processCapturedImage(int requestId, const QImage &img)
{
    Q_UNUSED(requestId);
    QImage scaledImage = img.scaled(m_viewfinder->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    qDebug () << "Höhe: " << scaledImage.size().height() << "Breite: " << scaledImage.size().width();
    qDebug () << "Bytes: " << scaledImage.sizeInBytes();

    m_lastPhoto->setPixmap(QPixmap::fromImage(scaledImage));

    QZXing decoder;
    //mandatory settings
    decoder.setDecoder(QZXing::DecoderFormat_QR_CODE);

    //optional settings
    decoder.setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    decoder.setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);

    //trigger decode
    QString result = decoder.decodeImage(scaledImage);

    qDebug () << "Data: " << result.toStdString();

    if (result.contains("M0100", Qt::CaseInsensitive))
    {
        qDebug () << "Success! Code is valid: " << result;

        // code recognized: play a supermarket beep sound :)
        QMediaPlayer *player = new QMediaPlayer;
        QAudioOutput *audioOut = new QAudioOutput;
        player->setAudioOutput(audioOut);
        QUrl filelocation ("qrc:/sounds/scanner_beep.mp3");
        player->setSource(filelocation);
        audioOut->setVolume(100);
        player->play();

        m_DecoderTimer->stop();

        int code = 50;

        emit codeRetrieved (code);
    }
}

void ScannerView::onScanButtonReleased()
{
    if (0 != m_DecoderTimer)
    {
        if (m_DecoderTimer->isActive())
            m_DecoderTimer->stop();
        else
            m_DecoderTimer->start (m_scanFrequency);
    }
}

//void ScannerView::setCamera (const QCameraDevice &cameraDevice)
void ScannerView::setCamera (QCamera *cameraDevice)
{
    if (0 != cameraDevice)
    {
        m_captureSession.setCamera(cameraDevice);
        m_captureSession.setVideoOutput(m_viewfinder);
        cameraDevice->start();
    }
}

void ScannerView::takePicture ()
{
    m_ImageCapture->captureToFile();
}

ScannerView::~ScannerView()
{
    if (m_viewfinder != 0)
    {
        delete m_viewfinder;
    }

    if (m_DecoderTimer != 0)
    {
        delete m_DecoderTimer;
    }
}

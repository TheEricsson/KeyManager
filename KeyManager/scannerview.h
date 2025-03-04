#ifndef SCANNERVIEW_H
#define SCANNERVIEW_H

#include <QWidget>
#include <QMediaDevices>
#include <QScopedPointer>
#include <QImageCapture>
#include <QMediaRecorder>
#include <QMediaCaptureSession>
#include <QCamera>
#include <QCameraDevice>
#include <QImage>
#include <QSize>

class QTimer;
class QVideoWidget;
class QPushButton;
class QLabel;

class ScannerView : public QWidget
{
    Q_OBJECT
public:
    explicit ScannerView(QWidget *parent = nullptr);
    void setCamera (QCamera *cameraDevice);
    void startScanning ();
    void stopScanning ();
    void setVideoOutput (QMediaCaptureSession *captureSession);
    QVideoWidget* getViewfinder ();
    QSize getViewfinderSize ();

    ~ScannerView();

private:
    void checkAvailableCams ();
    void checkPermissions ();
    void takePicture ();

    QMediaDevices m_devices;
    QImageCapture *m_ImageCapture;
    QMediaCaptureSession m_captureSession;
    QVideoWidget *m_viewfinder;

    bool m_isCapturingImage = false;
    bool m_applicationExiting = false;
    bool m_doImageCapture = true;

    int m_scanFrequency;

    QTimer *m_DecoderTimer;
    QLabel *m_lastPhoto;

signals:
    void closeWindow (int returnCode);
    void codeRetrieved (int code);
    void abortScanner ();

private slots:
    void onAbortBtnClicked ();
    void onScanButtonReleased ();
    void processCapturedImage (int requestId, const QImage &img);
};

#endif // SCANNERVIEW_H

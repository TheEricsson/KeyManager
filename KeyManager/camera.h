#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include <QMediaCaptureSession>
#include <QImage>
#include <QCameraDevice>

class QCameraDevice;
class QCamera;
class QImageCapture;
class QVideoWidget;

class Camera : public QObject
{
    Q_OBJECT
public:
    Camera (QObject *parent = Q_NULLPTR);
    void reset ();
    ~Camera();
    QCamera* getCamera ();
    const QList<QCameraDevice> getAvailableCameraDevices();
    const QList<int> getAvailableCameraIds();
    void setCameraDevice(int camId);
    QImageCapture* getImageCapture ();
    void startCamera ();
    void stopCamera ();
    void setVideoOutput (QVideoWidget* videoOutput);
    QImage getImageFromVideoframe ();

signals:
    void onPictureTaken (const QImage &img);

public slots:
    void takePicture ();

private:
    void init ();
    void checkPermissions ();
    void checkAvailableCams ();
    void setCamera (const QCameraDevice &cameraDevice);
    void initCaptureSession ();
    void processCapturedImage (int requestId, const QImage &img);

    QMediaCaptureSession mCaptureSession;
    QImageCapture *mImageCapture;
    QVideoWidget* mVideoWidget;
    QList<QCameraDevice> mAvailableCams;
    QCameraDevice mCurrentCameraDevice;

    QCamera *mCamera;
};

#endif // CAMERA_H

#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include <QMediaCaptureSession>
#include <QImage>

class QCameraDevice;
class QCamera;
class QImageCapture;
class QVideoWidget;

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera();
    void reset ();
    ~Camera();
    QCamera* getCamera ();
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

    QCamera *mCamera;
};

#endif // CAMERA_H

#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include <QMediaCaptureSession>

class QCameraDevice;
class QCamera;
class QImageCapture;
class QImage;

class Camera : public QWidget
{
    Q_OBJECT
public:
    explicit Camera(QWidget *parent = nullptr);
    QCamera* getCamera ();
    QImageCapture* getImageCapture ();
    QMediaCaptureSession* getCaptureSession ();
    //void takePicture ();

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

    QMediaCaptureSession *mCaptureSession;
    QImageCapture *mImageCapture;

    //QScopedPointer<QCamera> m_camera;
    QCamera *mCamera;
};

#endif // CAMERA_H

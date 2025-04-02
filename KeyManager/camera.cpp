#include "camera.h"
#include <QPermission>
#include <QApplication>
#include <QCoreApplication>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QCamera>
#include <QImageCapture>
#include <QVideoWidget>
#include <QVideoSink>

#if QT_CONFIG(permissions)
    #include <QPermission>
#endif

Camera::Camera (QWidget *parent)
    : QWidget{parent}
{
    mCamera = 0;
    mImageCapture = 0;
    mVideoWidget = 0;

    init ();
}

void Camera::takePicture ()
{
    qDebug () << "Camera::takePicture () called";

    if (!mImageCapture)
    {
        qFatal () << "Camera::takePicture ():" << "mImageCapture is Null";
        return;
    }

    if (mImageCapture->isReadyForCapture())
        mImageCapture->capture();
}

QImageCapture* Camera::getImageCapture ()
{
    if (mImageCapture)
        return mImageCapture;
    else
    {
        qFatal () << "Camera::getImageCapture ():" << "mImageCapture is Null";
    }
    return 0;
}

void Camera::init ()
{
    checkPermissions();
    checkAvailableCams ();
    setCamera(QMediaDevices::defaultVideoInput());
    initCaptureSession ();
}

void Camera::checkPermissions ()
{
#if QT_CONFIG(permissions)
    // camera
    QCameraPermission cameraPermission;
    switch (qApp->checkPermission(cameraPermission))
    {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(cameraPermission, this, &Camera::checkPermissions);
        return;
    case Qt::PermissionStatus::Denied:
        qWarning("Camera permission is not granted!");
        return;
    case Qt::PermissionStatus::Granted:
        break;
    }
#endif
}

void Camera::checkAvailableCams ()
{
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();

    qDebug () << "Checking cams";

    for (const QCameraDevice &cameraDevice : cameras)
    {
        qDebug() << "Kamera gefunden: " << cameraDevice.description();
    }
}

void Camera::setCamera (const QCameraDevice &cameraDevice)
{
    if (0 != mCamera)
    {
        delete mCamera;
        mCamera = 0;
    }

    mCamera = new QCamera(cameraDevice);

    mCamera->setExposureMode(QCamera::ExposureAuto);
    mCamera->setAutoIsoSensitivity();
    mCamera->setAutoExposureTime();
    mCamera->setWhiteBalanceMode(QCamera::WhiteBalanceAuto);
    mCamera->setFocusMode(QCamera::FocusModeAuto);
}

void Camera::initCaptureSession ()
{
    if (mImageCapture)
        delete mImageCapture;

    mImageCapture = new QImageCapture ();

    mImageCapture->setQuality(QImageCapture::HighQuality);
    mImageCapture->setFileFormat(QImageCapture::JPEG);
    mCaptureSession.setImageCapture(mImageCapture);

    mCaptureSession.setCamera(mCamera);

    connect(mImageCapture, &QImageCapture::imageCaptured, this, &Camera::processCapturedImage);
}

void Camera::startCamera ()
{
    if (mCamera)
        mCamera->start();
    else
        qFatal () << "Camera::startCamera ():" << "mCamera is Null";
}

void Camera::stopCamera()
{
    if (mCamera)
        mCamera->stop();
    else
        qFatal () << "Camera::stopCamera ():" << "mCamera is Null";
}

void Camera::setVideoOutput (QVideoWidget* videoOutput)
{
    if (!videoOutput)
        return;

    qDebug () << "Camera::setVideoOutput:" << "ok";

    mCaptureSession.setVideoOutput(videoOutput);

    mVideoWidget = videoOutput;
}

QImage Camera::getImageFromVideoframe ()
{
    return mVideoWidget->videoSink()->videoFrame().toImage();
}

void Camera::processCapturedImage (int requestId, const QImage &img)
{
    qDebug () << "picture taken!";
    emit onPictureTaken (img);
}

QCamera* Camera::getCamera ()
{
    if (mCamera)
        return mCamera;
    else
        qFatal () << "Camera::getCamera ():" << "mCamera is Null";
    return 0;
}

Camera::~Camera()
{
    if (mCamera)
    {
        delete mCamera;
        mCamera = 0;
    }
}

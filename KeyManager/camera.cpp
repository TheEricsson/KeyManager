#include "camera.h"
#include <QPermission>
#include <QApplication>
#include <QCoreApplication>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QCamera>
#include <QImageCapture>

#if QT_CONFIG(permissions)
    #include <QPermission>
#endif

Camera::Camera (QWidget *parent)
    : QWidget{parent}
{
    mCamera = 0;

    init ();
}

void Camera::takePicture ()
{
    qDebug () << "Camera::takePicture () called";

    if (mImageCapture->isReadyForCapture())
        mImageCapture->capture();
}

QImageCapture* Camera::getImageCapture ()
{
    return mImageCapture;
}

void Camera::init ()
{
    checkPermissions();
    checkAvailableCams ();
    setCamera(QMediaDevices::defaultVideoInput());
    initCaptureSession ();
    mCamera->start();
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

    mCamera->setExposureMode(QCamera::ExposurePortrait);
    mCamera->setManualExposureTime(0.02);
    mCamera->setManualIsoSensitivity(100);
    //mCamera->setAutoIsoSensitivity();
    //mCamera->setAutoExposureTime();
    mCamera->setWhiteBalanceMode(QCamera::WhiteBalanceAuto);
    mCamera->setFocusMode(QCamera::FocusModeAuto);
    mCamera->setExposureCompensation(-75);

    QCameraDevice camDevice = mCamera->cameraDevice();
    QList<QCameraFormat> camFormatList = camDevice.videoFormats();
    QList<QSize> photoRes = camDevice.photoResolutions();

    qDebug () << "available video formats:";
    for (int i = 0; i < camFormatList.count(); i++)
    {
        qDebug () << camFormatList[i].resolution().width() << " x " << camFormatList[i].resolution().height();
    }

    qDebug () << "available photo formats:";
    for (int i = 0; i < photoRes.count(); i++)
    {
        qDebug () << photoRes[i].width() << " x " << photoRes[i].height();
    }
}

void Camera::initCaptureSession ()
{
    mImageCapture = new QImageCapture ();
    mCaptureSession = new QMediaCaptureSession;
    mCaptureSession->setImageCapture(mImageCapture);
    mImageCapture->setQuality(QImageCapture::HighQuality);
    mImageCapture->setFileFormat(QImageCapture::JPEG);

    connect(mImageCapture, &QImageCapture::imageCaptured, this, &Camera::processCapturedImage);
}

QMediaCaptureSession* Camera::getCaptureSession ()
{
    return mCaptureSession;
}

void Camera::processCapturedImage (int requestId, const QImage &img)
{
    qDebug () << "picture taken!";
    emit onPictureTaken (img);
}

QCamera* Camera::getCamera ()
{
    return mCamera;
}

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

Camera::Camera (QObject *parent) : QObject(parent)
{
    mCamera = 0;
    mImageCapture = 0;
    mVideoWidget = 0;
    mCurrentCameraDevice = QMediaDevices::defaultVideoInput();

    init ();
}

void Camera::reset ()
{
    setCamera(mCurrentCameraDevice);
    initCaptureSession ();
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
    setCamera(mCurrentCameraDevice);
}

void Camera::checkPermissions ()
{
#if QT_CONFIG(permissions)
    // camera
    QCameraPermission cameraPermission;
    switch (qApp->checkPermission(cameraPermission))
    {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(cameraPermission, &Camera::checkPermissions);
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
    mAvailableCams = QMediaDevices::videoInputs();

    qDebug () << "Checking cams";

    for (int i = 0; i < mAvailableCams.size(); i++)
    {
        qDebug() << "Kamera gefunden: " << mAvailableCams.at(i).description();
    }
}

void Camera::setCamera (const QCameraDevice &cameraDevice)
{
    if (!mCamera)
    {
        mCamera = new QCamera(cameraDevice, this);
    }
    else
    {
        mCamera->setCameraDevice(cameraDevice);
    }

    mCamera->setExposureMode(QCamera::ExposureAuto);
    mCamera->setAutoIsoSensitivity();
    mCamera->setAutoExposureTime();
    mCamera->setWhiteBalanceMode(QCamera::WhiteBalanceAuto);
    mCamera->setFocusMode(QCamera::FocusModeAuto);

    initCaptureSession ();
}

void Camera::initCaptureSession ()
{
    if (!mImageCapture)
    {
        mImageCapture = new QImageCapture (this);
        connect(mImageCapture, &QImageCapture::imageCaptured, this, &Camera::processCapturedImage);
    }

    mImageCapture->setQuality(QImageCapture::HighQuality);
    mImageCapture->setFileFormat(QImageCapture::JPEG);

    mCaptureSession.setImageCapture(mImageCapture);

    if (mCamera)
        mCaptureSession.setCamera(mCamera);
}

void Camera::startCamera ()
{
    if (mCamera)
        mCamera->start();
}

void Camera::stopCamera()
{
    if (mCamera)
        mCamera->stop();
}

void Camera::setVideoOutput (QVideoWidget* videoOutput)
{
    if (0 != videoOutput)
    {
        mCaptureSession.setVideoOutput(videoOutput);
        mVideoWidget = videoOutput;
    }
}

QImage Camera::getImageFromVideoframe ()
{
    QImage img;

    if (mVideoWidget)
    {
        img = mVideoWidget->videoSink()->videoFrame().toImage();
    }
    return img;
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

const QList<QCameraDevice> Camera::getAvailableCameraDevices()
{
    return mAvailableCams;
}

const QList<int> Camera::getAvailableCameraIds()
{
    QList<int> ids;

    for (int i = 0; i < mAvailableCams.size(); i++)
    {
        ids.insert(i,i);
    }

    return ids;
}

void Camera::setCameraDevice(int camId)
{
    if (camId >= 0)
    {
        if (mAvailableCams.size() >= camId)
        {
        mCurrentCameraDevice = mAvailableCams.value(camId);
        //setCamera(mCurrentCameraDevice);
        initCaptureSession ();
        }
    }
}

Camera::~Camera()
{
}

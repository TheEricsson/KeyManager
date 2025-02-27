#include "camera.h"
#include <QPermission>
#include <QApplication>
#include <QCoreApplication>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QCamera>

#if QT_CONFIG(permissions)
    #include <QPermission>
#endif

Camera::Camera (QWidget *parent)
    : QWidget{parent}
{
    mCamera = 0;

    init ();
}

void Camera::init ()
{
    checkPermissions();
    checkAvailableCams ();
    setCamera(QMediaDevices::defaultVideoInput());
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

    mCamera->setFocusMode(QCamera::FocusModeAuto);
    mCamera->setExposureMode(QCamera::ExposureBarcode);
}

QCamera* Camera::getCamera ()
{
    return mCamera;
}

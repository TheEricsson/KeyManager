#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>

class QCameraDevice;
class QCamera;

class Camera : public QWidget
{
    Q_OBJECT
public:
    explicit Camera(QWidget *parent = nullptr);
    QCamera* getCamera ();
private:
    void init ();
    void checkPermissions ();
    void checkAvailableCams ();
    void setCamera (const QCameraDevice &cameraDevice);

    //QScopedPointer<QCamera> m_camera;
    QCamera *mCamera;
};

#endif // CAMERA_H

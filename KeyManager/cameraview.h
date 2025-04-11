#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QWidget>
#include "winsubmenu.h"

class QVideoWidget;
class Camera;

class CameraView : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit CameraView(QWidget *parent = nullptr);
        ~CameraView ();
        QImage getImage();
        virtual void showEvent(QShowEvent *);
        virtual void hideEvent(QHideEvent *);
    signals:
    private:
        QVideoWidget *mViewfinder;
        Camera *mCamera;
};

#endif // CAMERAVIEW_H

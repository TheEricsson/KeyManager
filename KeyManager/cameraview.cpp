#include "cameraview.h"
#include <QVideoWidget>
#include "winsubmenu.h"
#include <QLayout>
#include "camera.h"

CameraView::CameraView(QWidget *parent)
    : WinSubmenu{parent}
{
    mCamera = 0;

    // layout for the cam video/ pic screen
    mViewfinder = new QVideoWidget(this);
    mViewfinder->setMinimumSize(300, 150);
    mViewfinder->setAspectRatioMode(Qt::AspectRatioMode::KeepAspectRatioByExpanding);
    mViewfinder->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    mViewfinder->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *centralLayout = new QHBoxLayout();
    centralLayout->addWidget(mViewfinder);
    setCentralLayout(centralLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::TakePicture);
    menuButtons.append(Gui::Save);
    setMenuButtons(menuButtons);
}

QImage CameraView::getImage()
{
    return mCamera->getImageFromVideoframe();
}

void CameraView::showEvent(QShowEvent *)
{
    if (!mCamera)
    {
        mCamera = new Camera ();
        mCamera->setVideoOutput(mViewfinder);
    }
    mCamera->startCamera();
}

void CameraView::hideEvent(QHideEvent *)
{
    if (mCamera)
        mCamera->stopCamera();
}

CameraView::~CameraView ()
{
    if (mViewfinder)
    {
        delete mViewfinder;
        mViewfinder = 0;
    }
}

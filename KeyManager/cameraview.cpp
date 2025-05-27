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
    menuButtons.append(Gui::Shutter);
    setMenuButtons(menuButtons);
}

QImage CameraView::getImage()
{
    QImage img;

    if (mCamera)
    {
        img = mCamera->getImageFromVideoframe();
    }
    return img;
}

void CameraView::showEvent(QShowEvent *)
{
    if (!mCamera)
    {
        mCamera = new Camera (this);
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
}

#include "imageview.h"
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>

ImageView::ImageView(QWidget *parent) : WinSubmenu {parent}
{
    setHeader("Schlüsselansicht Detail");

    mImagePane = new QLabel("Kein Bild vorhanden");
    //mImagePane->setFixedSize(800, 600);
    //mImagePane->setScaledContents(true);

    // mImagePane->setMaximumHeight(Gui::logoHeight);
    // mImagePane->setMaximumWidth(Gui::logoWidth);
    //mImagePane->setContentsMargins(50,50,50,50);
    //mImagePane->setPixmap(logo_scaled);
    mImagePane->setAlignment(Qt::AlignCenter);
    //mImagePane->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    mImagePane->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->addWidget(mImagePane, Qt::AlignCenter);
    centralLayout->addItem(spacer);
    setCentralLayout(centralLayout);

    QList <Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::TakePicture);
    menuButtons.append(Gui::Delete);

    setMenuButtons(menuButtons);
    disableButton(2, true);
}

void ImageView::setImage(QImage &img)
{
    QImage scaledImg = img.scaled(mImagePane->width(), mImagePane->height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap pic = QPixmap::fromImage(scaledImg);
    mImagePane->setPixmap(pic);
    mImagePane->setAlignment(Qt::AlignCenter);

    //enable the "delete image" button, if image is valid
    if (!img.isNull())
    {
        enableButton(2, true);
    }
}

void ImageView::showEvent(QShowEvent *)
{
    //setFixedSize(800, 600);
    showFullScreen();
}

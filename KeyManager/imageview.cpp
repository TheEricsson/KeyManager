#include "imageview.h"
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>

ImageView::ImageView(QWidget *parent) : WinSubmenu {parent}
{
    mImagePane = new QLabel("Kein Bild vorhanden");

    mImagePane->setScaledContents(true);
    // mImagePane->setMaximumHeight(Gui::logoHeight);
    // mImagePane->setMaximumWidth(Gui::logoWidth);
    mImagePane->setContentsMargins(0,0,0,0);
    //mImagePane->setPixmap(logo_scaled);
    mImagePane->setAlignment(Qt::AlignCenter);
    mImagePane->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->addWidget(mImagePane);
    //centralLayout->addItem(spacer);
    setCentralLayout(centralLayout);

    QList <Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::TakePicture);
    menuButtons.append(Gui::DeleteImage);

    setMenuButtons(menuButtons);
    disableButton(2, true);
}

void ImageView::setImage(QImage &img)
{
    QImage scaledImg = img.scaled(mImagePane->width()* 0.8, mImagePane->height() * 0.8, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap pic = QPixmap::fromImage(scaledImg);
    mImagePane->setPixmap(pic);

    //enable the "delete image" button, if image is valid
    if (!img.isNull())
    {
        enableButton(2, true);
    }
}

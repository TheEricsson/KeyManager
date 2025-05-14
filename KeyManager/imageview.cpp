#include "imageview.h"
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>

ImageView::ImageView(QWidget *parent) : WinSubmenu {parent}
{
    mImagePane = new QLabel("No image provided.");

    mImagePane->setScaledContents(true);
    // mImagePane->setMaximumHeight(Gui::logoHeight);
    // mImagePane->setMaximumWidth(Gui::logoWidth);
    mImagePane->setContentsMargins(0,0,0,0);
    //mImagePane->setPixmap(logo_scaled);
    mImagePane->setAlignment(Qt::AlignCenter);
    mImagePane->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->addWidget(mImagePane);
    //centralLayout->addItem(spacer);
    setCentralLayout(centralLayout);

    QList <Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::TakePicture);
    menuButtons.append(Gui::DeleteImage);

    setMenuButtons(menuButtons);
}

void ImageView::setImage(QImage &img)
{
    QImage scaledImg = img.scaled(mImagePane->width(), mImagePane->height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap pic = QPixmap::fromImage(scaledImg);
    mImagePane->setPixmap(pic);
}

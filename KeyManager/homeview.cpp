#include "homeview.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QLabel>
#include <QMainWindow>

#include "winsubmenu.h"
#include "globals.h"

HomeView::HomeView(QWidget *parent)
    : WinSubmenu {parent}
{
    mInitDone = false;
    setHeader("KeyManager");

    mLogo = new QLabel();
    mLogo->setScaledContents(true);
    mLogo->setContentsMargins(20,20,20,20);
    mLogo->setAlignment(Qt::AlignCenter);
    mLogo->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->addWidget(mLogo);
    centralLayout->addItem(spacer);
    setCentralLayout(centralLayout);

    QList <Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::MenuButton::Scanner);
    menuButtons.append(Gui::MenuButton::Search);
    menuButtons.append(Gui::MenuButton::Tools);
    menuButtons.append(Gui::MenuButton::Settings);
    menuButtons.append(Gui::MenuButton::Exit);

    setMenuButtons(menuButtons);
}

void HomeView::showEvent(QShowEvent *)
{
    //set logo once at first show event
    if (!mInitDone)
    {
        if (mLogo)
        {
            int logo_width = size().width();
            logo_width *= 0.75;

            QPixmap logo (":/images/keymanager_logo.png");
            QPixmap logo_scaled = logo.scaledToWidth(logo_width,Qt::SmoothTransformation);
            mLogo->setPixmap(logo_scaled);
        }
    }
}

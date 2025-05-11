#include "homeview.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QLabel>

#include "winsubmenu.h"
#include "globals.h"

HomeView::HomeView(QWidget *parent)
    : WinSubmenu {parent}
{
    setHeader("KEYMANAGER");

    QPixmap logo (":/images/logo.png");
    QPixmap logo_scaled = logo.scaled(QSize(Gui::logoWidth, Gui::logoHeight), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QLabel *label = new QLabel(this);
    label->setScaledContents(true);
    label->setMaximumHeight(Gui::logoHeight);
    label->setMaximumWidth(Gui::logoWidth);
    label->setContentsMargins(0,0,0,0);
    label->setPixmap(logo_scaled);
    label->setAlignment(Qt::AlignHCenter);
    label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *centralLayout = new QHBoxLayout();
    centralLayout->addWidget(label);
    centralLayout->addItem(spacer);
    setCentralLayout(centralLayout);

    QList <Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::MenuButton::Scanner);
    menuButtons.append(Gui::MenuButton::Search);
    menuButtons.append(Gui::MenuButton::Settings);
    menuButtons.append(Gui::MenuButton::Exit);

    setMenuButtons(menuButtons);
}

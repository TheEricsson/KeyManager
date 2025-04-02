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
    mBtnScan = 0;
    mBtnSearch = 0;
    mBtnManage = 0;
    mBtnExit = 0;
    mLayout = 0;

    setHeader("KEYMANAGER");

    QPixmap logo (":/images/logo.png");
    QLabel *label = new QLabel(this);
    label->setScaledContents(true);
    label->setMaximumHeight(Gui::logoHeight);
    label->setMaximumWidth(Gui::logoWidth);
    label->setAlignment(Qt::AlignHCenter);
    label->setPixmap(logo);
    layout()->addWidget(label);

    QList <Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::MenuButton::Scanner);
    menuButtons.append(Gui::MenuButton::Search);
    menuButtons.append(Gui::MenuButton::Settings);
    menuButtons.append(Gui::MenuButton::Exit);

    setMenuButtons(menuButtons);
}

// void HomeView::onScanButtonReleased()
// {
//     emit showScannerView ();
// }

// void HomeView::onSearchButtonReleased()
// {
//     emit showTableView ();
// }

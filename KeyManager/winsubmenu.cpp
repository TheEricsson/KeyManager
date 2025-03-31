#include "winsubmenu.h"

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include "globals.h"
#include "dataobject.h"
#include "menubutton.h"
#include "viewdata.h"

WinSubmenu::WinSubmenu(QWidget *parent)
    : QWidget{parent}
{
    mButtonsSet = false;
    mDataObject = 0;
    mViewData = 0;
    mDatabase = 0;
    mButtonLayout = 0;

    mLayout = new QVBoxLayout (this);

    mHeaderLabel = new QLabel (this);
    mLayout->addWidget(mHeaderLabel,0,Qt::AlignCenter);

    mHeaderLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    mHeaderLabel->setStyleSheet("QLabel {background-color: #C0C0C0; color: black; font: 20pt; font-color: #696969; border-style: outset; border-width: 3px; border-radius: 10px; border-color: #A9A9A9; }");

    setStyleSheet("QToolButton {background-color: #C0C0C0; border-style: outset; border-width: 3px; border-radius: 10px; border-color: #A9A9A9; font: bold 14px; padding: 6px;}");

    mViewData = new ViewData ();
}

// void WinSubmenu::setDataObject (DataObject *data)
// {
//     if (data)
//         mDataObject = data;
// }

void WinSubmenu::setDataObject (ViewData *data)
{
    if (data)
        mViewData = data;
}

void WinSubmenu::setDatabaseHandle (DatabaseImpl *db)
{
    if (db)
        mDatabase = db;
}

void WinSubmenu::setHeader (const QString& label)
{    
    mHeaderLabel->setText(label);
}

void  WinSubmenu::onMenuBtnClicked (Gui::MenuButton btnType)
{
    qDebug () << "WinSubmenu::onMenuBtnClicked (Gui::MenuButton btnType): " << btnType;
    emit menuButtonClicked(btnType);
}

void WinSubmenu::setMenuButtons (const QList<Gui::MenuButton> &buttons)
{
    // this method shall only be called once by now
    if (mButtonsSet)
        return;

    // // delete menu buttons, if already set before
    // for (int i = 0; i < mLayout->count(); i++)
    // {
    //     MenuButton *menuBtn = dynamic_cast<MenuButton*>(mLayout->itemAt(i));
    //     if (menuBtn != nullptr)
    //     {
    //         mLayout->removeWidget(menuBtn);
    //     }
    // }

    int itemCnt = buttons.size();

    if (0 == itemCnt)
        return;

    mButtonLayout = new QHBoxLayout ();

    for (int i = 0; i<buttons.count();i++)
    {
        MenuButton *menuBtn = new MenuButton (this);
        menuBtn->setButtonType(buttons[i]);
        menuBtn->setIconSize(QSize(Gui::buttonWidth,Gui::buttonHeight));

        switch (buttons[i])
        {
            case (Gui::Back):
                menuBtn->setIcon(QIcon(":/images/menu_back.png"));
                break;
            case (Gui::Pdf):
                menuBtn->setIcon(QIcon(":/images/menu_pdf.png"));
                break;
            case (Gui::Repeat):
                menuBtn->setIcon(QIcon(":/images/menu_retry.png"));
                break;
            case (Gui::Next):
                menuBtn->setIcon(QIcon(":/images/menu_next.png"));
                break;
            case (Gui::Ok):
                menuBtn->setIcon(QIcon(":/images/menu_ok.png"));
                break;
            case (Gui::AddRecipient):
                menuBtn->setIcon(QIcon(":/images/menu_add_recipient.jpeg"));
                break;
            case (Gui::Handout):
                menuBtn->setIcon(QIcon(":/images/menu_keyOut.png"));
                break;
            case (Gui::TakeBack):
                menuBtn->setIcon(QIcon(":/images/menu_keyBack.png"));
                break;
            case (Gui::Scanner):
                menuBtn->setIcon(QIcon(":/images/menu_scan.png"));
                break;
            case (Gui::Search):
                menuBtn->setIcon(QIcon(":/images/menu_search.svg"));
                break;
            case (Gui::Settings):
                menuBtn->setIcon(QIcon(":/images/menu_settings.svg"));
                break;
            case (Gui::Exit):
                menuBtn->setIcon(QIcon(":/images/menu_exit.svg"));
                break;
            default:
                break;
        }
        mButtonLayout->addWidget(menuBtn);
        connect (menuBtn, SIGNAL (clicked(Gui::MenuButton)), this, SLOT (onMenuBtnClicked(Gui::MenuButton)));
    }

    if (mButtonLayout)
    {
        QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding,QSizePolicy::Expanding);
        mLayout->addSpacerItem(spacer);
        mLayout->addLayout(mButtonLayout);
    }

    mButtonsSet = true;
}

void WinSubmenu::disableButton (int column, bool disable)
{
    if (mButtonLayout)
    {
        QToolButton *btn = (QToolButton*)mButtonLayout->itemAt(column)->widget();
        if (btn)
        {
            btn->setDisabled(disable);
        }
    }
}

void WinSubmenu::enableButton (int column, bool enable)
{
    disableButton(column, !enable);
}

WinSubmenu::~WinSubmenu ()
{
    // todo! segfault
    // if (0 != mViewData)
    //     delete mViewData;
}

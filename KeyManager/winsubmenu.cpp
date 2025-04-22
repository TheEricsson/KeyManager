#include "winsubmenu.h"

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include "globals.h"
#include "dataobject.h"
#include "menubutton.h"
#include "datainterface.h"
#include "iointerface.h"

WinSubmenu::WinSubmenu(QWidget *parent)
    : QWidget{parent}
{
    mButtonsSet = false;
    mDataInterface = 0;
    mIOInterface = 0;
    mButtonLayout = 0;

    mLayout = new QVBoxLayout (this);

    mHeaderLabel = new QLabel (this);
    mLayout->addWidget(mHeaderLabel,0,Qt::AlignCenter);

    mHeaderLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    //mHeaderLabel->setStyleSheet("QLabel {background-color: #e0ffff; color: black; font: bold 30px; border-style: solid; border-width: 1px; border-radius: 5px; border-color: #A9A9A9; }");

    //setStyleSheet("QToolButton {border-style: solid; border-width: 1px; border-radius: 10px; border-color: #A9A9A9; font: 12px; padding: 6px;}");
}

void WinSubmenu::setDataInterface (DataInterface *data)
{
    if (data)
        mDataInterface = data;
}

void WinSubmenu::setIOInterface (IOInterface *ioData)
{
    if (ioData)
        mIOInterface = ioData;
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
        menuBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        switch (buttons[i])
        {
            case (Gui::Back):
                menuBtn->setIcon(QIcon(":/images/menu_back.png"));
                menuBtn->setButtonText("Zurück");
                break;
            case (Gui::Pdf):
                menuBtn->setIcon(QIcon(":/images/menu_pdf.png"));
                menuBtn->setButtonText("PDF");
                break;
            case (Gui::Repeat):
                menuBtn->setIcon(QIcon(":/images/menu_retry.png"));
                menuBtn->setButtonText("Wiederholen");
                break;
            case (Gui::Next):
                menuBtn->setIcon(QIcon(":/images/menu_next.png"));
                menuBtn->setButtonText("Weiter");
                break;
            case (Gui::Ok):
                menuBtn->setIcon(QIcon(":/images/menu_ok.png"));
                break;
            case (Gui::AddRecipient):
                menuBtn->setIcon(QIcon(":/images/menu_add_recipient.jpeg"));
                menuBtn->setButtonText("Empfänger \nhinzufügen");
                break;
            case (Gui::Handout):
                menuBtn->setIcon(QIcon(":/images/menu_keyOut.png"));
                menuBtn->setButtonText("Ausgabe");
                break;
            case (Gui::TakeBack):
                menuBtn->setIcon(QIcon(":/images/menu_keyBack.png"));
                menuBtn->setButtonText("Rücknahme");
                break;
            case (Gui::Scanner):
                menuBtn->setIcon(QIcon(":/images/menu_scan.png"));
                menuBtn->setButtonText("Scannen");
                break;
            case (Gui::Search):
                menuBtn->setIcon(QIcon(":/images/menu_search.svg"));
                menuBtn->setButtonText("Suche");
                break;
            case (Gui::Settings):
                menuBtn->setIcon(QIcon(":/images/menu_settings.svg"));
                menuBtn->setButtonText("Einstellungen");
                break;
            case (Gui::Exit):
                menuBtn->setIcon(QIcon(":/images/menu_exit.svg"));
                menuBtn->setButtonText("Verlassen");
                break;
            case (Gui::MainMenu):
                //menuBtn->setIcon(QIcon(":/images/TODO"));
                menuBtn->setButtonText("Hauptmenü");
                break;
            case (Gui::AddCustomer):
                //menuBtn->setIcon(QIcon(":/images/TODO"));
                menuBtn->setButtonText("Kunde anlegen");
                break;
            case (Gui::Edit):
                //menuBtn->setIcon(QIcon(":/images/TODO"));
                menuBtn->setButtonText("Editieren");
                break;
            case (Gui::Print):
                //menuBtn->setIcon(QIcon(":/images/TODO"));
                menuBtn->setButtonText("Drucken");
                break;
            default:
                break;
        }
        mButtonLayout->addWidget(menuBtn);
        connect (menuBtn, SIGNAL (clicked(Gui::MenuButton)), this, SLOT (onMenuBtnClicked(Gui::MenuButton)));
    }

    if (mButtonLayout)
    {
        // QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding,QSizePolicy::Expanding);
        // mLayout->addSpacerItem(spacer);
        mLayout->addLayout(mButtonLayout);
    }

    mButtonsSet = true;
}

void WinSubmenu::setButtonText (int column, const QString &btnText)
{
    if (mButtonLayout)
    {
        QToolButton *btn = (QToolButton*)mButtonLayout->itemAt(column)->widget();
        if (btn)
        {
            btn->setText(btnText);
        }
    }
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

void WinSubmenu::hideButton (int column, bool disable)
{
    if (mButtonLayout)
    {
        QToolButton *btn = (QToolButton*)mButtonLayout->itemAt(column)->widget();
        if (btn)
        {
            btn->setHidden(disable);
        }
    }
}

void WinSubmenu::showButton (int column, bool enable)
{
    hideButton(column, !enable);
}

WinSubmenu::~WinSubmenu ()
{
}

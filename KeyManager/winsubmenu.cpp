#include "winsubmenu.h"

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QFile>
#include <QKeyEvent>
#include <QBoxLayout>
#include <QStyleOption>
#include <QPainter>

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
    mBottomLayout = 0;
    mTopLayout = 0;
    mCentralLayout  = 0;

    /*set layout dummies for the layout sections
    this prevents errors, when layouts are replaced at runtime not starting with index 0*/

    mBaseLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(mBaseLayout);

    setTopLayout(new QHBoxLayout());
    setCentralLayout(new QHBoxLayout());
    setBottomLayout(new QHBoxLayout());

    setAttribute(Qt::WA_LayoutUsesWidgetRect);

    //mHeaderLabel = new QLabel (this);
    //mLayout->addWidget(mHeaderLabel,0,Qt::AlignCenter);

    //mHeaderLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    //set css styles
    // QFile f(":qdarkstyle/light/lightstyle.qss");
    // if (!f.exists())
    // {
    //     qDebug () << "Unable to set stylesheet, file not found:";
    // }
    // else   {
    //     f.open(QFile::ReadOnly | QFile::Text);
    //     QTextStream ts(&f);
    //     setStyleSheet(ts.readAll());
    // }

    // mHeaderLabel->setStyleSheet("QLabel {background-color: #e0ffff; color: black; font: bold 30px; border-style: solid; border-width: 1px; border-radius: 5px; border-color: #A9A9A9; }");

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

void WinSubmenu::setTopLayout (QLayout *layout)
{
    QLayout *current = getTopLayout();
    if (0 != current)
    {
        mBaseLayout->removeItem(current);
        delete current;
    }

    if (layout)
    {
        layout->setSizeConstraint(QLayout::SetMinimumSize);
        mBaseLayout->insertLayout(0, layout);
    }
    update();
}

void WinSubmenu::setCentralLayout (QLayout *layout)
{
    QLayout *current = getCentralLayout();
    if (0 != current)
    {
        mBaseLayout->removeItem(current);
        delete current;
    }

    if (layout)
    {
        layout->setSizeConstraint(QLayout::SetMaximumSize);
        mBaseLayout->insertLayout(1, layout);
        mBaseLayout->update();
    }
    update();
}

void WinSubmenu::setBottomLayout (QLayout *layout)
{
    QLayout *current = getBottomLayout();
    if (0 != current)
    {
        mBaseLayout->removeItem(current);
        delete current;
    }

    if (layout)
    {
        layout->setSizeConstraint(QLayout::SetMinimumSize);
        mBaseLayout->insertLayout(2, layout);
        mBaseLayout->update();
    }
    update();
}

QLayout* WinSubmenu::getTopLayout ()
{
    QLayoutItem *item = mBaseLayout->itemAt(0);
    QLayout *layout = 0;

    if (0 != item)
        layout = (QLayout*) item;

    return layout;
}

QLayout* WinSubmenu::getCentralLayout ()
{
    QLayoutItem *item = mBaseLayout->itemAt(1);
    QLayout *layout = 0;

    if (0 != item)
        layout = (QLayout*) item;

    return layout;
}

QLayout* WinSubmenu::getBottomLayout ()
{
    QLayoutItem *item = mBaseLayout->itemAt(2);
    QLayout *layout = 0;

    if (0 != item)
        layout = (QLayout*) item;

    return layout;
}

void WinSubmenu::setHeader (const QString& label)
{
    QHBoxLayout *layout = new QHBoxLayout();
    QLabel *header = new QLabel (label);
    layout->addWidget(header,0,Qt::AlignCenter);
    setTopLayout(layout);
    //mHeaderLabel->setText(label);
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

    QHBoxLayout *btnLayout = new QHBoxLayout ();

    for (int i = 0; i<buttons.count();i++)
    {
        MenuButton *menuBtn = new MenuButton ();
        menuBtn->setButtonType(buttons[i]);
        menuBtn->setIconSize(QSize(Gui::buttonWidth,Gui::buttonHeight));
        menuBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        QFont font("Roboto");
        font.setStyleHint(QFont::SansSerif);
        font.setPixelSize(12);
        menuBtn->setFont(font);

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
            case (Gui::DeleteImage):
                //menuBtn->setIcon(QIcon(":/images/TODO"));
                menuBtn->setButtonText("Bild\nLöschen");
                break;
            case (Gui::NewImageFromCam):
                //menuBtn->setIcon(QIcon(":/images/TODO"));
                menuBtn->setButtonText("Neues Bild\naufnehmen");
                break;
            default:
                break;
        }
        btnLayout->addWidget(menuBtn);
        connect (menuBtn, SIGNAL (clicked(Gui::MenuButton)), this, SLOT (onMenuBtnClicked(Gui::MenuButton)));
    }

    setBottomLayout(btnLayout);
    mButtonsSet = true;
}

void WinSubmenu::setButtonText (int column, const QString &btnText)
{
    QLayout *btnLayout = getBottomLayout();
    if (btnLayout)
    {
        QToolButton *btn = (QToolButton*)btnLayout->itemAt(column)->widget();
        if (btn)
        {
            btn->setText(btnText);
        }
    }
}

void WinSubmenu::disableButton (int column, bool disable)
{
    QLayout *btnLayout = getBottomLayout();
    if (btnLayout)
    {
        QToolButton *btn = (QToolButton*)btnLayout->itemAt(column)->widget();
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
    QLayout *btnLayout = getBottomLayout();
    if (btnLayout)
    {
        QToolButton *btn = (QToolButton*)btnLayout->itemAt(column)->widget();
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

void WinSubmenu::keyReleaseEvent(QKeyEvent *event)
{
    //catch native android button (back) and handle like the gui generated back key
    switch (event->key())
    {
    case Qt::Key_Back:
        qDebug () << "WinSubmenu::keyReleaseEvent - back key pressed";
        onMenuBtnClicked(Gui::Back);
        break;
    default:
        break;
    }
    //QWidget::keyReleaseEvent(event);
}

WinSubmenu::~WinSubmenu ()
{
    QLayout *current = getBottomLayout();
    if (0 != current)
    {
        mBaseLayout->removeItem(current);
        delete current;
    }

    current = getTopLayout();
    if (0 != current)
    {
        mBaseLayout->removeItem(current);
        delete current;
    }

    current = getCentralLayout();
    if (0 != current)
    {
        mBaseLayout->removeItem(current);
        delete current;
    }
}

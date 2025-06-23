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
#include <QScrollArea>

#include "globals.h"
#include "menubutton.h"
#include "datainterface.h"
#include "iointerface.h"

WinSubmenu::WinSubmenu(QWidget *parent)
    : QWidget{parent}
{
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
    QFile f(":qdarkstyle/light/lightstyle.qss");
    if (!f.exists())
    {
        qDebug () << "Unable to set stylesheet, file not found:";
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        setStyleSheet(ts.readAll());
    }
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
    }
}

void WinSubmenu::setBottomLayout (QLayout *layout)
{
    qDebug () << "setBottomLayout";

    QLayout *current = getBottomLayout();
    qDebug () << "1";
    if (0 != current)
    {
        qDebug () << "2";
        mBaseLayout->removeItem(current);
        qDebug () << "3";
        delete current;
    }

    if (layout)
    {
        qDebug () << "4";
        layout->setSizeConstraint(QLayout::SetMinimumSize);
        mBaseLayout->insertLayout(2, layout);
        qDebug () << "5";
    }
    qDebug () << "6";
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
    QVBoxLayout *layout = new QVBoxLayout();

    // QFrame *frame = new QFrame (this);
    // frame->setLayout(layout);
    // frame->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    // frame->setObjectName("WinSubMenuTopFrame");

    QLabel *headerWinSubMenu = new QLabel (label);
    headerWinSubMenu->setAlignment(Qt::AlignCenter);
    headerWinSubMenu->setObjectName("WinSubMenuHeader");

    layout->addWidget(headerWinSubMenu,0);
    setTopLayout(layout);
}

void  WinSubmenu::onMenuBtnClicked (Gui::MenuButton btnType)
{
    qDebug () << "WinSubmenu::onMenuBtnClicked (Gui::MenuButton btnType): " << btnType;
    emit menuButtonClicked(btnType);
}

void WinSubmenu::setMenuButtons (const QList<Gui::MenuButton> &buttons)
{
    int itemCnt = buttons.size();

    if (0 == itemCnt)
        return;

    QHBoxLayout *btnLayout = new QHBoxLayout ();

    qDebug () << "setMenuButtons";

    for (int i = 0; i<buttons.count();i++)
    {
        MenuButton *menuBtn = new MenuButton (this);
        menuBtn->setButtonType(buttons[i]);
        menuBtn->setIconSize(QSize(Gui::buttonWidth,Gui::buttonHeight));
        menuBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        // QFont font("Roboto");
        // font.setStyleHint(QFont::SansSerif);
        // font.setPixelSize(12);
        // menuBtn->setFont(font);

        switch (buttons[i])
        {
            case (Gui::Back):
                menuBtn->setIcon(QIcon(":/images/google_material_design/arrow_back_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Zurück");
                break;
            case (Gui::Pdf):
                menuBtn->setIcon(QIcon(":/images/google_material_design/picture_as_pdf_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("PDF");
                break;
            case (Gui::Repeat):
                menuBtn->setIcon(QIcon(":/images/google_material_design/redo_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Wiederholen");
                break;
            case (Gui::Next):
                menuBtn->setIcon(QIcon(":/images/google_material_design/arrow_forward_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Weiter");
                break;
            case (Gui::Ok):
                menuBtn->setIcon(QIcon(":/images/google_material_design/check_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                break;
            case (Gui::AddRecipient):
                menuBtn->setIcon(QIcon(":/images/google_material_design/person_add_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Empfänger \nhinzufügen");
                break;
            case (Gui::Handout):
                menuBtn->setIcon(QIcon(":/images/google_material_design/logout_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Ausgabe");
                break;
            case (Gui::TakeBack):
                menuBtn->setIcon(QIcon(":/images/google_material_design/login_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Rücknahme");
                break;
            case (Gui::Scanner):
                menuBtn->setIcon(QIcon(":/images/google_material_design/qr_code_scanner_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Scannen");
                break;
            case (Gui::Search):
                menuBtn->setIcon(QIcon(":/images/google_material_design/search_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Suche");
                break;
            case (Gui::Settings):
                menuBtn->setIcon(QIcon(":/images/google_material_design/settings_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Einstellungen");
                break;
            case (Gui::Exit):
                menuBtn->setIcon(QIcon(":/images/google_material_design/exit_to_app_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Verlassen");
                break;
            case (Gui::MainMenu):
                menuBtn->setIcon(QIcon(":/images/google_material_design/home_app_logo_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Hauptmenü");
                break;
            case (Gui::AddCustomer):
                menuBtn->setIcon(QIcon(":/images/google_material_design/add_home_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Kunde anlegen");
                break;
            case (Gui::Edit):
                menuBtn->setIcon(QIcon(":/images/google_material_design/edit_note_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Editieren");
                break;
            case (Gui::Print):
                menuBtn->setIcon(QIcon(":/images/google_material_design/print_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Drucken");
                break;
            case (Gui::Delete):
                menuBtn->setIcon(QIcon(":/images/google_material_design/delete_forever_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Bild\nLöschen");
                break;
            case (Gui::NewImageFromCam):
                menuBtn->setIcon(QIcon(":/images/google_material_design/photo_camera_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Neues Bild\naufnehmen");
                break;
            case (Gui::Tools):
                menuBtn->setIcon(QIcon(":/images/google_material_design/apps_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Tools");
                break;
            case (Gui::AddKey):
                menuBtn->setIcon(QIcon(":/images/google_material_design/key_add_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Schlüssel\nhinzufügen");
                break;
            case (Gui::TakePicture):
                menuBtn->setIcon(QIcon(":/images/google_material_design/photo_camera_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Foto\naufnehmen");
                break;
            case (Gui::Save):
                menuBtn->setIcon(QIcon(":/images/google_material_design/save_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Bild\nspeichern");
                break;
            case (Gui::Shutter):
                menuBtn->setIcon(QIcon(":/images/google_material_design/camera_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Bild\nspeichern");
                break;
            case (Gui::NewCodeScanned):
                menuBtn->setIcon(QIcon(":/images/google_material_design/key_add_48dp_5985E1_FILL0_wght400_GRAD0_opsz48.svg"));
                //menuBtn->setButtonText("Bild\nspeichern");
                break;
            default:
                break;
        }
        btnLayout->addWidget(menuBtn);
        connect (menuBtn, SIGNAL (clicked(Gui::MenuButton)), this, SLOT (onMenuBtnClicked(Gui::MenuButton)));
        qDebug () << "setMenuButtons end";
    }

    setBottomLayout(btnLayout);
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
    /*uncommented:
    base layout should remove all 'child' layouts itself, because they are all child items*/

    /*QLayout *current = getBottomLayout();
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
    }*/
}

#include "winsubmenu.h"

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include "globals.h"

WinSubmenu::WinSubmenu(QWidget *parent)
    : QWidget{parent}
{
    mLayout = new QVBoxLayout (this);
    setLayout(mLayout);

    mHeaderLabel = new QLabel (this);
    mLayout->addWidget(mHeaderLabel);
}

void WinSubmenu::setHeader (const QString& label)
{
    mHeaderLabel->setText(label);
}

void WinSubmenu::onFirstBtnClicked ()
{
    emit firstButtonClicked();
}

void WinSubmenu::onSecondBtnClicked ()
{
    emit secondButtonClicked();
}

void WinSubmenu::onThirdBtnClicked ()
{
    emit thirdButtonClicked();
}

void WinSubmenu::setMenuButtons (const UiSpecs::eMenuButton& column1, const UiSpecs::eMenuButton& column2, const UiSpecs::eMenuButton& column3)
{
    QPushButton *btnColumn1 = 0;
    QPushButton *btnColumn2 = 0;
    QPushButton *btnColumn3 = 0;
    QHBoxLayout *btnLayout = 0;

    // set first button
    switch (column1)
    {
        case (UiSpecs::eMenuButton::BackButton):
            btnColumn1 = new QPushButton (this);
            btnColumn1->setIcon(QIcon(":/images/menu_back.png"));
            break;
        default:
            break;
            //right now we don't need any other case for column1
    }

    if (0 != btnColumn1)
    {
        connect (btnColumn1, SIGNAL (clicked()), this, SLOT (onFirstBtnClicked()));
        btnColumn1->setIconSize(QSize(UiSpecs::buttonWidth,UiSpecs::buttonHeight));

        if (!btnLayout)
        {
            btnLayout = new QHBoxLayout (this);
        }
        btnLayout->addWidget(btnColumn1);
    }

    // set second button
    switch (column2)
    {
        case (UiSpecs::eMenuButton::RepeatButton):
            btnColumn2 = new QPushButton (this);
            btnColumn2->setIcon(QIcon(":/images/menu_retry.png"));
            break;
        case (UiSpecs::eMenuButton::ForwardButton):
            btnColumn2 = new QPushButton (this);
            btnColumn2->setIcon(QIcon(":/images/menu_next.png"));
            break;
        case (UiSpecs::eMenuButton::OkButton):
            btnColumn2 = new QPushButton (this);
            btnColumn2->setIcon(QIcon(":/images/btn_Ok.png"));
            break;
        default:
            break;
    }

    if (0 != btnColumn2)
    {
        connect (btnColumn2, SIGNAL (clicked()), this, SLOT (onSecondBtnClicked()));
        btnColumn2->setIconSize(QSize(UiSpecs::buttonWidth,UiSpecs::buttonHeight));

        if (!btnLayout)
        {
            btnLayout = new QHBoxLayout (this);
        }
        btnLayout->addWidget(btnColumn2);
    }

    // set third button
    switch (column3)
    {
    case (UiSpecs::eMenuButton::RepeatButton):
        btnColumn3 = new QPushButton (this);
        btnColumn3->setIcon(QIcon(":/images/menu_retry.png"));
        break;
    case (UiSpecs::eMenuButton::ForwardButton):
        btnColumn3 = new QPushButton (this);
        btnColumn3->setIcon(QIcon(":/images/menu_next.png"));
        break;
    case (UiSpecs::eMenuButton::OkButton):
        btnColumn3 = new QPushButton (this);
        btnColumn3->setIcon(QIcon(":/images/btn_Ok.png"));
        break;
    default:
        break;
    }

    if (0 != btnColumn3)
    {
        connect (btnColumn3, SIGNAL (clicked()), this, SLOT (onThirdBtnClicked()));
        btnColumn3->setIconSize(QSize(UiSpecs::buttonWidth,UiSpecs::buttonHeight));

        if (!btnLayout)
        {
            btnLayout = new QHBoxLayout (this);
        }
        btnLayout->addWidget(btnColumn3);
    }

    if (btnLayout)
        mLayout->addLayout(btnLayout);
}

#include "winsubmenu.h"

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include "globals.h"

WinSubmenu::WinSubmenu(QWidget *parent)
    : QWidget{parent}
{
    mBtnColumn0 = 0;
    mBtnColumn1 = 0;
    mBtnColumn2 = 0;

    mLayout = new QVBoxLayout (this);
    setLayout(mLayout);

    mHeaderLabel = new QLabel (this);
    mLayout->addWidget(mHeaderLabel,0,Qt::AlignCenter);

    mHeaderLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    mHeaderLabel->setStyleSheet("QLabel {background-color: #C0C0C0; color: black; font: 20pt; font-color: #696969; border-style: outset; border-width: 3px; border-radius: 10px; border-color: #A9A9A9; }");

    this->setStyleSheet("QPushButton {background-color: #C0C0C0; border-style: outset; border-width: 3px; border-radius: 10px; border-color: #A9A9A9; font: bold 14px; padding: 6px;}");
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

void WinSubmenu::setMenuButtons (const UiSpecs::eMenuButton& column0, const UiSpecs::eMenuButton& column1, const UiSpecs::eMenuButton& column2)
{
    QHBoxLayout *btnLayout = 0;

    // set first button
    switch (column0)
    {
        case (UiSpecs::eMenuButton::BackButton):
            mBtnColumn0 = new QPushButton (this);
            mBtnColumn0->setIcon(QIcon(":/images/menu_back.png"));
            break;
        default:
            break;
            //right now we don't need any other case for column1
    }

    if (0 != mBtnColumn0)
    {
        connect (mBtnColumn0, SIGNAL (clicked()), this, SLOT (onFirstBtnClicked()));
        mBtnColumn0->setIconSize(QSize(UiSpecs::buttonWidth,UiSpecs::buttonHeight));

        if (!btnLayout)
        {
            btnLayout = new QHBoxLayout (this);
        }
        btnLayout->addWidget(mBtnColumn0);
    }

    // set second button
    switch (column1)
    {
        case (UiSpecs::eMenuButton::RepeatButton):
            mBtnColumn1 = new QPushButton (this);
            mBtnColumn1->setIcon(QIcon(":/images/menu_retry.png"));
            break;
        case (UiSpecs::eMenuButton::ForwardButton):
            mBtnColumn1 = new QPushButton (this);
            mBtnColumn1->setIcon(QIcon(":/images/menu_next.png"));
            break;
        case (UiSpecs::eMenuButton::OkButton):
            mBtnColumn1 = new QPushButton (this);
            mBtnColumn1->setIcon(QIcon(":/images/btn_Ok.png"));
            break;
        default:
            break;
    }

    if (0 != mBtnColumn1)
    {
        connect (mBtnColumn1, SIGNAL (clicked()), this, SLOT (onSecondBtnClicked()));
        mBtnColumn1->setIconSize(QSize(UiSpecs::buttonWidth,UiSpecs::buttonHeight));

        if (!btnLayout)
        {
            btnLayout = new QHBoxLayout (this);
        }
        btnLayout->addWidget(mBtnColumn1);
    }

    // set third button
    switch (column2)
    {
    case (UiSpecs::eMenuButton::RepeatButton):
        mBtnColumn2 = new QPushButton (this);
        mBtnColumn2->setIcon(QIcon(":/images/menu_retry.png"));
        break;
    case (UiSpecs::eMenuButton::ForwardButton):
        mBtnColumn2 = new QPushButton (this);
        mBtnColumn2->setIcon(QIcon(":/images/menu_next.png"));
        break;
    case (UiSpecs::eMenuButton::OkButton):
        mBtnColumn2 = new QPushButton (this);
        mBtnColumn2->setIcon(QIcon(":/images/btn_Ok.png"));
        break;
    default:
        break;
    }

    if (0 != mBtnColumn2)
    {
        connect (mBtnColumn2, SIGNAL (clicked()), this, SLOT (onThirdBtnClicked()));
        mBtnColumn2->setIconSize(QSize(UiSpecs::buttonWidth,UiSpecs::buttonHeight));

        if (!btnLayout)
        {
            btnLayout = new QHBoxLayout (this);
        }
        btnLayout->addWidget(mBtnColumn2);
    }

    if (btnLayout)
    {
        QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding,QSizePolicy::Expanding);
        mLayout->addSpacerItem(spacer);

        mLayout->addLayout(btnLayout);
    }
}

void WinSubmenu::disableButton (int column, bool disable)
{
    switch (column)
    {
        case 0:
            if (mBtnColumn0)
                mBtnColumn0->setDisabled(disable);
            break;
        case 1:
            if (mBtnColumn1)
                mBtnColumn1->setDisabled(disable);
            break;
        case 2:
            if (mBtnColumn2)
                mBtnColumn2->setDisabled(disable);
            break;
    }
}

void WinSubmenu::enableButton (int column, bool enable)
{
    disableButton(column, !enable);
}

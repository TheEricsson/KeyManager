#include "menubutton.h"

MenuButton::MenuButton (QWidget *parent)
    : QToolButton {parent}
{
    mButtonType = Gui::None;
    connect (this, SIGNAL(clicked()), this, SLOT(menuButtonClicked())); //adapt base class signal
}

void MenuButton::menuButtonClicked()
{
    emit clicked (mButtonType);
}

#include "handoverview.h"

HandoverView::HandoverView (QWidget *parent)
    : WinSubmenu {parent}
{
    setMenuButtons(UiSpecs::BackButton, UiSpecs::eMenuButton::ForwardButton);
    setHeader("Ausgabe abschließen");
}

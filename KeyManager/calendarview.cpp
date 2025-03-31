#include "calendarview.h"
#include <QCalendarWidget>
#include <QVBoxLayout>

CalendarView::CalendarView (QWidget *parent) : WinSubmenu {parent}
{
    QCalendarWidget* calender = new QCalendarWidget (this);
    this->layout()->addWidget(calender);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    setHeader("Rückgabedatum wählen");
}

#include "calendarview.h"
#include <QCalendarWidget>
#include <QVBoxLayout>

CalendarView::CalendarView (QWidget *parent) : WinSubmenu {parent}
{
    QCalendarWidget* calender = new QCalendarWidget (this);
    this->layout()->addWidget(calender);

    setMenuButtons(UiSpecs::BackButton, UiSpecs::OkButton);
    setHeader("Rückgabedatum wählen");
}

#ifndef CALENDARVIEW_H
#define CALENDARVIEW_H

#include <QWidget>
#include "winsubmenu.h"

class CalendarView : public WinSubmenu
{
    Q_OBJECT
public:
    CalendarView (QWidget *parent = nullptr);
};

#endif // CALENDARVIEW_H

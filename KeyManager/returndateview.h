#ifndef RETURNDATEVIEW_H
#define RETURNDATEVIEW_H

#include <QWidget>
#include "winsubmenu.h"
#include <QDate>

class QCalendarWidget;
class QLabel;

class ReturnDateView : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit ReturnDateView (QWidget *parent = nullptr);
    private slots:
        void onHandoverTemporaryClicked (bool aChecked);
        void onHandoverPermanentClicked(bool aChecked);
        void onHandoverEndOfServiceClicked(bool aChecked);
        void onDateClicked (QDate date);
        void onSecondBtnClicked (); //override
    private:
        QCalendarWidget *mCalendar;
        QLabel *mReturnDateLabel;

        Database::eKeychainStatusId mDurationHandout;
};

#endif // RETURNDATEVIEW_H

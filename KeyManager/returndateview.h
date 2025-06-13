#ifndef RETURNDATEVIEW_H
#define RETURNDATEVIEW_H

#include <QWidget>
#include "winsubmenu.h"
#include <QDate>

class QCalendarWidget;
class QLabel;
class QCheckBox;

class ReturnDateView : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit ReturnDateView (QWidget *parent = nullptr);
    signals:
        void dateSelectionChanged (QDate date);
        void keychainStatusChanged (KeychainStatus::Value aStatus);
    private slots:
        void onHandoverTemporaryClicked (bool aChecked);
        void onHandoverPermanentClicked(bool aChecked);
        void onHandoverEndOfServiceClicked(bool aChecked);
        void onDateClicked (QDate date);
    private:
        void reset();
        void showEvent(QShowEvent *);
        QCalendarWidget *mCalendar;
        QLabel *mReturnDateLabel;
        QCheckBox* mHandoverTemporary;
};

#endif // RETURNDATEVIEW_H

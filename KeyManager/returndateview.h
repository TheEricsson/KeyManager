#ifndef RETURNDATEVIEW_H
#define RETURNDATEVIEW_H

#include <QWidget>
#include "winsubmenu.h"
#include <QDate>

class QCalendarWidget;
class QLabel;
class QCheckBox;

class ReturnDateView : public QWidget
{
    Q_OBJECT
    public:
        explicit ReturnDateView (QWidget *parent = nullptr);
    signals:
        void dateSelectionChanged (QDate date);
        void keychainStatusChanged (Database::KeychainStatus aStatus);
    private slots:
        void onHandoverTemporaryClicked (bool aChecked);
        void onHandoverPermanentClicked(bool aChecked);
        void onHandoverEndOfServiceClicked(bool aChecked);
        void onDateClicked (QDate date);
        void onSecondBtnClicked (); //override
    private:
        void showEvent(QShowEvent *);
        QCalendarWidget *mCalendar;
        QLabel *mReturnDateLabel;
        QCheckBox* mHandoverTemporary;

        Database::KeychainStatus mDurationHandout;
};

#endif // RETURNDATEVIEW_H

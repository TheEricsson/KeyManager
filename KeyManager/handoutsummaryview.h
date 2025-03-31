#ifndef HANDOUTSUMMARYVIEW_H
#define HANDOUTSUMMARYVIEW_H

#include "winsubmenu.h"

class QLineEdit;
class QTextEdit;

class HandoutSummaryView : public WinSubmenu
{
    Q_OBJECT
    public:
        HandoutSummaryView (QWidget *parent = nullptr);
        //void setDataObject (DataObject *data);

    private:
        void updateForm (DataObject *data);
        QLineEdit *mBarcodeLineEdit;
        QLineEdit *mNewStatusEdit;
        QLineEdit *mHandoverDateEdit;
        QLineEdit *mDeadlineDateEdit;
        QLineEdit *mRecipientEdit;
        QLineEdit *mRecipientStreetEdit;
        QLineEdit *mRecipientAreaCodeAndCityEdit;
        QLineEdit *mRecipientSigneeEdit;
        QTextEdit *mNotesEdit;
};

#endif // HANDOUTSUMMARYVIEW_H

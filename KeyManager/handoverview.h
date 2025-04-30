#ifndef HANDOVERVIEW_H
#define HANDOVERVIEW_H

#include <QWidget>
#include "winsubmenu.h"
#include "globals.h"

class SignaturePad;
class QLineEdit;
class QTextEdit;

class HandoverView : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit HandoverView (QWidget *parent = nullptr);
        ~HandoverView();

    private slots:
        virtual void onMenuBtnClicked (Gui::MenuButton btnType);

    private slots:
        void onSignaturePaint ();

    private:
        void showEvent(QShowEvent *);
        void reset ();
        void resetSignature ();
        bool reportRequested();
        bool printRequested();
        void pdfReport();
        void printReport();
        void insertDbErrorMessage();
        SignaturePad *mSigPad;
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

#endif // HANDOVERVIEW_H

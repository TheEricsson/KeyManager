#ifndef HANDOVERVIEW_H
#define HANDOVERVIEW_H

#include <QWidget>
#include "winsubmenu.h"

class SignaturePad;
class QLineEdit;

class HandoverView : public WinSubmenu
{
    Q_OBJECT
public:
    explicit HandoverView (QWidget *parent = nullptr);
    void clear ();

private slots:
    void onSignaturePaint ();
    void onSignatureClear ();
    void onSetReturnDate ();

private:
    SignaturePad *mSigPad;
    QLineEdit *mRecipientName;

};

#endif // HANDOVERVIEW_H

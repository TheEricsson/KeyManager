#ifndef HANDOVERVIEW_H
#define HANDOVERVIEW_H

#include <QWidget>
#include "winsubmenu.h"
#include "signaturepad.h"

class HandoverView : public WinSubmenu
{
    Q_OBJECT
public:
    explicit HandoverView (QWidget *parent = nullptr);
    void clear ();
private:
    SignaturePad *mSigPad;
private slots:
    void onSignaturePaint ();
    void onSignatureClear ();
};

#endif // HANDOVERVIEW_H

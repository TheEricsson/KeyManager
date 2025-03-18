#include "handoverview.h"
#include "signaturepad.h"
#include <QLayout>
#include <QPushButton>

HandoverView::HandoverView (QWidget *parent)
    : WinSubmenu {parent}
{
    setHeader("Ausgabe abschließen");
    mSigPad = new SignaturePad ();
    this->layout()->addWidget(mSigPad);

    QPushButton *btnClear = new QPushButton ("Reset");
    this->layout()->addWidget(btnClear);

    setMenuButtons(UiSpecs::BackButton, UiSpecs::OkButton);
    disableButton(1, true);

    connect (btnClear, SIGNAL (clicked()), mSigPad, SLOT (clearImage ()));
    connect (btnClear, SIGNAL (clicked()), this, SLOT (onSignatureClear ()));
    connect (mSigPad, SIGNAL (signaturePaint ()), this, SLOT(onSignaturePaint ()));
}

void HandoverView::clear ()
{
    mSigPad->clearImage();
    onSignatureClear();
}

void HandoverView::onSignaturePaint ()
{
    if (mSigPad->isModified())
        enableButton(1, true);
}

void HandoverView::onSignatureClear ()
{
    if (!mSigPad->isModified())
        disableButton(1, true);
}

// HandoverView::onClearBtnClicked ()
// {
//     mSigPad->cle
// }

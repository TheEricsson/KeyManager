#include "handoverview.h"
#include "signaturepad.h"
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

HandoverView::HandoverView (QWidget *parent)
    : WinSubmenu {parent}
{
    setHeader("Ausgabe abschließen");

    QHBoxLayout *recipientNameLayout = new QHBoxLayout (this);

    QLabel* recipientNameLabel = new QLabel ("Empfängername", this);
    mRecipientName = new QLineEdit (this);
    recipientNameLayout->addWidget(recipientNameLabel);
    recipientNameLayout->addWidget(mRecipientName);
    mRecipientName->setEnabled(false);

    layout()->addItem(recipientNameLayout);

    mSigPad = new SignaturePad ();
    layout()->addWidget(mSigPad);

    QPushButton *btnClear = new QPushButton ("Reset");
    layout()->addWidget(btnClear);

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

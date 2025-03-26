#include "handoverview.h"
#include "signaturepad.h"
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCalendarWidget>
#include "calendarview.h"
#include "dataobjecthandover.h"

HandoverView::HandoverView (QWidget *parent)
    : WinSubmenu {parent}
{
    setHeader("Ausgabe abschließen");

    mSigPad = new SignaturePad ();

    layout()->addWidget(mSigPad);

    setMenuButtons(UiSpecs::BackButton, UiSpecs::RepeatButton, UiSpecs::OkButton);
    clear ();

    connect (mSigPad, SIGNAL (signaturePaint ()), this, SLOT(onSignaturePaint ()));
}

void HandoverView::clear ()
{
    mSigPad->clearImage();

    if (!mSigPad->isModified())
    {
        disableButton(1, true);
        disableButton(2, true);
        update ();
    }
}

void HandoverView::onSignaturePaint ()
{
    if (mSigPad->isModified())
    {
        enableButton(1, true);
        enableButton(2, true);
        update ();
    }
}

void HandoverView::onSecondBtnClicked ()
{
    clear ();
}

void HandoverView::onThirdBtnClicked ()
{
    //set current data
    if (mDataObject)
    {
        DataObjectHandover *dataObj = (DataObjectHandover*)mDataObject;
        if (dataObj)
        {
            QImage sigImg = mSigPad->getSignature();
            dataObj->setSignature(sigImg);
        }
    }
    emit thirdButtonClicked ();
}

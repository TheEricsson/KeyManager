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

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Repeat);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

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

void  HandoverView::onMenuBtnClicked (Gui::MenuButton btnType)
{
    switch (btnType)
    {
        //reset signature pad
        case (Gui::Repeat):
            clear ();
            break;
        //no catch in this class, emit signal
        default:
            emit menuButtonClicked(btnType);
            break;
    }
}

void HandoverView::onSecondBtnClicked ()
{
    clear ();
}

void HandoverView::onThirdBtnClicked ()
{
    // //set current data
    // if (mDataObject)
    // {
    //     DataObjectHandover *dataObj = (DataObjectHandover*)mDataObject;
    //     if (dataObj)
    //     {
    //         QImage sigImg = mSigPad->getSignature();
    //         dataObj->setSignature(sigImg);
    //     }
    // }
    // emit thirdButtonClicked ();
}

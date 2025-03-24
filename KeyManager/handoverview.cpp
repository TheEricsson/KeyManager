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

HandoverView::HandoverView (QWidget *parent)
    : WinSubmenu {parent}
{
    setHeader("Ausgabe abschließen");

    QHBoxLayout *recipientNameLayout = new QHBoxLayout ();

    QLabel* recipientNameLabel = new QLabel ("Empfängername", this);
    mRecipientName = new QLineEdit (this);
    recipientNameLayout->addWidget(recipientNameLabel);
    recipientNameLayout->addWidget(mRecipientName);
    mRecipientName->setEnabled(false);
    layout()->addItem(recipientNameLayout);

    QRadioButton* handoverLimited = new QRadioButton ("Temporäre Ausgabe", this);
    handoverLimited->setChecked(true);
    QRadioButton* handoverPermanent = new QRadioButton ("Dauerhafte Ausgabe", this);
    QButtonGroup* duration = new QButtonGroup (this);
    duration->addButton(handoverLimited, 0);
    duration->addButton(handoverPermanent, 1);

    QPushButton* dateOfReturn = new QPushButton ("Rückgabedatum ändern", this);

    mSigPad = new SignaturePad ();

    QPushButton *btnClear = new QPushButton ("Reset");

    layout()->addWidget(handoverLimited);
    layout()->addWidget(dateOfReturn);
    layout()->addWidget(handoverPermanent);
    layout()->addWidget(mSigPad);
    layout()->addWidget(btnClear);

    setMenuButtons(UiSpecs::BackButton, UiSpecs::OkButton);
    disableButton(1, true);

    //connect (btnClear, SIGNAL (clicked()), mSigPad, SLOT (clearImage ()));
    connect (btnClear, SIGNAL (clicked()), this, SLOT (onSignatureClear ()));
    connect (mSigPad, SIGNAL (signaturePaint ()), this, SLOT(onSignaturePaint ()));
    connect (dateOfReturn, SIGNAL (clicked()), this, SLOT (onSetReturnDate ()));
}

void HandoverView::clear ()
{
    onSignatureClear();
}

void HandoverView::onSignaturePaint ()
{
    if (mSigPad->isModified())
    {
        enableButton(1, true);
        update ();
    }
}

void HandoverView::onSignatureClear ()
{
    mSigPad->clearImage();

    if (!mSigPad->isModified())
        disableButton(1, true);

    update ();
}

void HandoverView::onSetReturnDate ()
{
    CalendarView* calView = new CalendarView ();
    calView->setWindowModality(Qt::WindowModality::ApplicationModal);
    calView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    calView->show();
}

#include "returndateview.h"
#include <QCheckBox>
#include <QButtonGroup>
#include <QLayout>
#include <QCalendarWidget>
#include <QLabel>
#include <QDate>
#include "dataobjecthandover.h"

ReturnDateView::ReturnDateView (QWidget *parent)
    : WinSubmenu {parent}
{
    mCalendar = 0;

    setHeader("Rückgabedatum wählen");

    mCalendar = new QCalendarWidget (this);

    QCheckBox* handoverTemporary= new QCheckBox ("Befristete Ausgabe", this);
    handoverTemporary->setChecked(true);

    QCheckBox* handoverPermanent = new QCheckBox ("Dauerhafte Ausgabe", this);
    handoverPermanent->setChecked(false);

    QCheckBox* handoverEndOfService = new QCheckBox ("Dauerhafte Ausgabe (Verwaltungsende)", this);
    handoverEndOfService->setChecked(false);

    QButtonGroup *btnGroup = new QButtonGroup (this);
    btnGroup->addButton(handoverTemporary);
    btnGroup->addButton(handoverPermanent);
    btnGroup->addButton(handoverEndOfService);

    mReturnDateLabel = new QLabel (this);

    layout()->addWidget(handoverTemporary);
    layout()->addWidget(handoverPermanent);
    layout()->addWidget(handoverEndOfService);
    layout()->addWidget(mCalendar);
    layout()->addWidget(mReturnDateLabel);

    mCalendar->setMinimumDate(QDate::currentDate());
    mCalendar->setSelectedDate(QDate::currentDate().addDays(14));
    onDateClicked(mCalendar->selectedDate());
    mDurationHandout = Database::eKeychainStatusId::TemporaryOut;

    setMenuButtons(UiSpecs::BackButton, UiSpecs::NextButton);

    connect (handoverTemporary, SIGNAL (clicked (bool)), this, SLOT(onHandoverTemporaryClicked (bool)));
    connect (handoverPermanent, SIGNAL (clicked (bool)), this, SLOT(onHandoverPermanentClicked (bool)));
    connect (handoverEndOfService, SIGNAL (clicked (bool)), this, SLOT(onHandoverEndOfServiceClicked (bool)));
    connect (mCalendar, SIGNAL(clicked(QDate)), this, SLOT(onDateClicked(QDate)));
}

void ReturnDateView::onHandoverTemporaryClicked (bool aChecked)
{
    if (!mCalendar)
        return;

    mCalendar->setDisabled(!aChecked);
    mDurationHandout = Database::eKeychainStatusId::TemporaryOut;

    update ();
}

void ReturnDateView::onHandoverPermanentClicked(bool aChecked)
{
    if (!mCalendar)
        return;

    mCalendar->setDisabled(aChecked);
    mDurationHandout = Database::eKeychainStatusId::PermanentOut;

    update ();
}

void ReturnDateView::onHandoverEndOfServiceClicked(bool aChecked)
{
    if (!mCalendar)
        return;

    mCalendar->setDisabled(aChecked);
    mDurationHandout = Database::eKeychainStatusId::AdministrationEnded;

    update ();
}

void ReturnDateView::onDateClicked (QDate date)
{
    QString dateLabel ("Gesetztes Rückgabedatum: ");
    QString dateDeadline;

    int day = 0;
    int month = 0;
    int year = 0;

    date.getDate (&year, &month, &day);

    dateDeadline.append(QString::number(day));
    dateDeadline.append(".");
    dateDeadline.append(QString::number(month));
    dateDeadline.append(".");
    dateDeadline.append(QString::number(year));

    dateLabel.append(dateDeadline);

    mReturnDateLabel->setText(dateLabel);

    update ();
}

void ReturnDateView::onSecondBtnClicked ()
{
    //set current data in data object
    if (mDataObject)
    {
        DataObjectHandover *dataObj = (DataObjectHandover*)mDataObject;

        dataObj->setKeychainStatus(mDurationHandout);

        QDate date = QDate::currentDate();
        QString currentDate = QString::number(date.day());
        currentDate.append(".");
        currentDate.append(QString::number(date.month()));
        currentDate.append(".");
        currentDate.append(QString::number(date.year()));

        dataObj->setDateHandover(currentDate);

        if (mDurationHandout == Database::eKeychainStatusId::TemporaryOut)
        {
            int day = 0;
            int month = 0;
            int year = 0;
            QString dateDeadline;

            date = mCalendar->selectedDate();
            date.getDate (&year, &month, &day);

            dateDeadline.append(QString::number(day));
            dateDeadline.append(".");
            dateDeadline.append(QString::number(month));
            dateDeadline.append(".");
            dateDeadline.append(QString::number(year));

            dataObj->setDateDeadline(dateDeadline);
        }

        if (mDurationHandout == Database::eKeychainStatusId::AdministrationEnded ||
            mDurationHandout == Database::eKeychainStatusId::PermanentOut)
        {
            dataObj->setDateDeadline("31.12.9999");
        }
    }

    emit secondButtonClicked ();
}

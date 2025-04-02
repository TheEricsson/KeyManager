#include "returndateview.h"
#include <QCheckBox>
#include <QButtonGroup>
#include <QLayout>
#include <QCalendarWidget>
#include <QLabel>
#include <QDate>
#include "dataobjecthandover.h"

ReturnDateView::ReturnDateView (QWidget *parent)
{
    mCalendar = 0;

    //setHeader("Rückgabedatum wählen");

    mCalendar = new QCalendarWidget (this);

    mHandoverTemporary= new QCheckBox ("Befristete Ausgabe", this);
    mHandoverTemporary->setChecked(true);

    QCheckBox* handoverPermanent = new QCheckBox ("Dauerhafte Ausgabe", this);
    handoverPermanent->setChecked(false);

    QCheckBox* handoverEndOfService = new QCheckBox ("Dauerhafte Ausgabe (Verwaltungsende)", this);
    handoverEndOfService->setChecked(false);

    QButtonGroup *btnGroup = new QButtonGroup (this);
    btnGroup->addButton(mHandoverTemporary);
    btnGroup->addButton(handoverPermanent);
    btnGroup->addButton(handoverEndOfService);

    mReturnDateLabel = new QLabel (this);

    QVBoxLayout *layout = new QVBoxLayout ();

    layout->addWidget(mHandoverTemporary);
    layout->addWidget(handoverPermanent);
    layout->addWidget(handoverEndOfService);
    layout->addWidget(mCalendar);
    layout->addWidget(mReturnDateLabel);

    setLayout(layout);

    mCalendar->setMinimumDate(QDate::currentDate());
    mCalendar->setSelectedDate(QDate::currentDate().addDays(14));
    onDateClicked(mCalendar->selectedDate());
    mDurationHandout = Database::KeychainStatus::TemporaryOut;

    // QList<Gui::MenuButton> menuButtons;
    // menuButtons.append(Gui::Back);
    // menuButtons.append(Gui::Next);
    // setMenuButtons(menuButtons);

    connect (mHandoverTemporary, SIGNAL (clicked (bool)), this, SLOT(onHandoverTemporaryClicked (bool)));
    connect (handoverPermanent, SIGNAL (clicked (bool)), this, SLOT(onHandoverPermanentClicked (bool)));
    connect (handoverEndOfService, SIGNAL (clicked (bool)), this, SLOT(onHandoverEndOfServiceClicked (bool)));
    connect (mCalendar, SIGNAL(clicked(QDate)), this, SLOT(onDateClicked(QDate)));
}

void ReturnDateView::showEvent(QShowEvent *)
{
    mHandoverTemporary->setChecked (true);
    mCalendar->setMinimumDate(QDate::currentDate());
    mCalendar->setSelectedDate(QDate::currentDate().addDays(14));
    mDurationHandout = Database::KeychainStatus::TemporaryOut;

    emit keychainStatusChanged (mDurationHandout);
    emit dateSelectionChanged (mCalendar->selectedDate());
}


void ReturnDateView::onHandoverTemporaryClicked (bool aChecked)
{
    if (!mCalendar)
        return;

    mCalendar->setDisabled(!aChecked);
    mDurationHandout = Database::KeychainStatus::TemporaryOut;
    emit keychainStatusChanged (mDurationHandout);

    update ();
}

void ReturnDateView::onHandoverPermanentClicked(bool aChecked)
{
    if (!mCalendar)
        return;

    mCalendar->setDisabled(aChecked);
    mDurationHandout = Database::KeychainStatus::PermanentOut;
    emit keychainStatusChanged (mDurationHandout);

    update ();
}

void ReturnDateView::onHandoverEndOfServiceClicked(bool aChecked)
{
    if (!mCalendar)
        return;

    mCalendar->setDisabled(aChecked);
    mDurationHandout = Database::KeychainStatus::AdministrationEnded;
    emit keychainStatusChanged (mDurationHandout);

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

    emit dateSelectionChanged (date);
}

void ReturnDateView::onSecondBtnClicked ()
{
    // //set current data in data object
    // if (mDataObject)
    // {
    //     DataObjectHandover *dataObj = (DataObjectHandover*)mDataObject;

    //     dataObj->setKeychainStatus(mDurationHandout);

    //     QDate date = QDate::currentDate();
    //     QString currentDate = QString::number(date.day());
    //     currentDate.append(".");
    //     currentDate.append(QString::number(date.month()));
    //     currentDate.append(".");
    //     currentDate.append(QString::number(date.year()));

    //     dataObj->setDateHandover(currentDate);

    //     if (mDurationHandout == Database::eKeychainStatusId::TemporaryOut)
    //     {
    //         int day = 0;
    //         int month = 0;
    //         int year = 0;
    //         QString dateDeadline;

    //         date = mCalendar->selectedDate();
    //         date.getDate (&year, &month, &day);

    //         dateDeadline.append(QString::number(day));
    //         dateDeadline.append(".");
    //         dateDeadline.append(QString::number(month));
    //         dateDeadline.append(".");
    //         dateDeadline.append(QString::number(year));

    //         dataObj->setDateDeadline(dateDeadline);
    //     }

    //     if (mDurationHandout == Database::eKeychainStatusId::AdministrationEnded ||
    //         mDurationHandout == Database::eKeychainStatusId::PermanentOut)
    //     {
    //         dataObj->setDateDeadline("31.12.9999");
    //     }
    // }

    // emit secondButtonClicked ();
}

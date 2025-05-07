#include "returndateview.h"
#include <QCheckBox>
#include <QButtonGroup>
#include <QLayout>
#include <QCalendarWidget>
#include <QLabel>
#include <QDate>
#include "datainterface.h"

ReturnDateView::ReturnDateView (QWidget *parent) : WinSubmenu {parent}
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
    //mReturnDateLabel->setStyleSheet("{font: bold 25px; color: #EE2C2C}");

    //QVBoxLayout *layout = new QVBoxLayout ();

    layout()->addWidget(mHandoverTemporary);
    layout()->addWidget(handoverPermanent);
    layout()->addWidget(handoverEndOfService);
    layout()->addWidget(mCalendar);
    layout()->addWidget(mReturnDateLabel);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout()->addItem(spacer);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    mCalendar->setMinimumDate(QDate::currentDate());
    mCalendar->setSelectedDate(QDate::currentDate().addDays(14));

    connect (mHandoverTemporary, SIGNAL (clicked(bool)), this, SLOT(onHandoverTemporaryClicked(bool)));
    connect (handoverPermanent, SIGNAL (clicked(bool)), this, SLOT(onHandoverPermanentClicked(bool)));
    connect (handoverEndOfService, SIGNAL (clicked(bool)), this, SLOT(onHandoverEndOfServiceClicked(bool)));
    connect (mCalendar, SIGNAL(clicked(QDate)), this, SLOT(onDateClicked(QDate)));
}

void ReturnDateView::showEvent(QShowEvent *)
{
    switch (dataInterface()->getKeychainStatusId())
    {
        case KeychainStatus::Value::AdministrationEnded:
        case KeychainStatus::Value::Lost:
        case KeychainStatus::Value::PermanentOut:
        case KeychainStatus::Value::TemporaryOut:
        case KeychainStatus::Value::Undefined:
            onHandoverPermanentClicked (true);
            mReturnDateLabel->setVisible(false);
            dataInterface()->setNewKeychainStatusId(KeychainStatus::Value::Available);
            emit menuButtonClicked(Gui::Next);
            break;
        case KeychainStatus::Value::Available:
        default:
            mHandoverTemporary->setChecked(true);
            onHandoverTemporaryClicked (true);
            mCalendar->setMinimumDate(QDate::currentDate());
            mCalendar->setSelectedDate(QDate::currentDate().addDays(14));
            dataInterface()->setNewKeychainStatusId(KeychainStatus::Value::TemporaryOut);
            onDateClicked(mCalendar->selectedDate());
            mReturnDateLabel->setVisible(true);
            break;
    }
    update ();
}


void ReturnDateView::onHandoverTemporaryClicked (bool aChecked)
{
    if (!mCalendar)
        return;

    mCalendar->setDisabled(!aChecked);
    mReturnDateLabel->setVisible(true);
    //mCalendar->setVisible(aChecked);
    //mReturnDateLabel->setVisible(true);
    dataInterface()->setNewKeychainStatusId(KeychainStatus::Value::TemporaryOut);
    onDateClicked(mCalendar->selectedDate());
    update ();
}

void ReturnDateView::onHandoverPermanentClicked(bool aChecked)
{
    if (!mCalendar)
        return;

    mCalendar->setDisabled(aChecked);
    //mCalendar->setVisible(!aChecked);
    //mReturnDateLabel->setVisible(false);

    dataInterface()->setNewKeychainStatusId(KeychainStatus::Value::PermanentOut);
    dataInterface()->setRecipientDeadlineDate ("");
    mReturnDateLabel->setVisible(false);
    update ();
}

void ReturnDateView::onHandoverEndOfServiceClicked(bool aChecked)
{
    if (!mCalendar)
        return;

    mCalendar->setDisabled(aChecked);
    //mCalendar->setVisible(!aChecked);
    //mReturnDateLabel->setVisible(false);

    dataInterface()->setNewKeychainStatusId(KeychainStatus::Value::AdministrationEnded);
    dataInterface()->setRecipientDeadlineDate ("");
    mReturnDateLabel->setVisible(false);
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

    dataInterface()->setRecipientDeadlineDate(dateDeadline);

    update ();
}

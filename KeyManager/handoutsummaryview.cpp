#include "handoutsummaryview.h"
#include <QLabel>
#include <QPixmap>
#include <QLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include "dataobjecthandover.h"

HandoutSummaryView::HandoutSummaryView (QWidget *parent) : WinSubmenu {parent}
{
    setHeader("Zusammenfassung");

    //clear ();

    QGridLayout *gridLayout = new QGridLayout ();

    QLabel *barcodeLabel = new QLabel ("Barcode", this);
    mBarcodeLineEdit = new QLineEdit (this);
    mBarcodeLineEdit->setDisabled(true);

    QLabel *keychainStatus = new QLabel ("Neuer Status", this);
    mNewStatusEdit = new QLineEdit (this);
    mNewStatusEdit->setDisabled(true);

    QLabel *handoverDate = new QLabel ("Ausgabedatum", this);
    mHandoverDateEdit = new QLineEdit (this);
    mHandoverDateEdit->setDisabled(true);

    QLabel *deadlineDate = new QLabel ("Rückgabedatum", this);
    mDeadlineDateEdit = new QLineEdit (this);
    mDeadlineDateEdit->setDisabled(true);

    QLabel *recipient = new QLabel ("Empfänger", this);
    mRecipientEdit = new QLineEdit (this);
    mRecipientEdit->setDisabled(true);

    QLabel *recipientStreetAndNumber  = new QLabel ("Straße, Hausnummer", this);
    mRecipientStreetEdit = new QLineEdit (this);
    mRecipientStreetEdit->setDisabled(true);

    QLabel *recipientAreaCodeAndCity  = new QLabel ("PLZ, Ort", this);
    mRecipientAreaCodeAndCityEdit = new QLineEdit (this);
    mRecipientAreaCodeAndCityEdit->setDisabled(true);

    QLabel *signee  = new QLabel ("Unterzeichner", this);
    mRecipientSigneeEdit = new QLineEdit (this);
    mRecipientSigneeEdit->setDisabled(true);

    QLabel *notes  = new QLabel ("Zusätzliche Notizen", this);
    mNotesEdit = new QTextEdit (this);
    mNotesEdit->setDisabled(true);

    gridLayout->addWidget(barcodeLabel, 0, 0);
    gridLayout->addWidget(mBarcodeLineEdit, 0, 1);

    gridLayout->addWidget(keychainStatus, 1, 0);
    gridLayout->addWidget(mNewStatusEdit, 1, 1);

    gridLayout->addWidget(handoverDate, 2, 0);
    gridLayout->addWidget(mHandoverDateEdit, 2, 1);

    gridLayout->addWidget(deadlineDate, 3, 0);
    gridLayout->addWidget(mDeadlineDateEdit, 3, 1);

    gridLayout->addWidget(recipient, 4, 0);
    gridLayout->addWidget(mRecipientEdit, 4, 1);

    gridLayout->addWidget(recipientStreetAndNumber, 5, 0);
    gridLayout->addWidget(mRecipientStreetEdit, 5, 1);

    gridLayout->addWidget(recipientAreaCodeAndCity, 6, 0);
    gridLayout->addWidget(mRecipientAreaCodeAndCityEdit, 6, 1);

    gridLayout->addWidget(signee, 7, 0);
    gridLayout->addWidget(mRecipientSigneeEdit, 7, 1);

    gridLayout->addWidget(notes, 8, 0);
    gridLayout->addWidget(mNotesEdit, 8, 1);

    layout()->addItem(gridLayout);

    QPixmap iconCheckOk (":/images/icon_check.png");
    QLabel *iconCheckOkLabel = new QLabel (this);
    iconCheckOkLabel->setPixmap(iconCheckOk);

    iconCheckOkLabel->setScaledContents(true);
    iconCheckOkLabel->setMaximumHeight(250);
    iconCheckOkLabel->setMaximumWidth(250);

    layout()->addWidget(iconCheckOkLabel);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Pdf);
    menuButtons.append(Gui::MainMenu);
    setMenuButtons(menuButtons);
}

// void HandoutSummaryView::setDataObject (DataObject *data)
// {
//     if (data)
//         mDataObject = data;

//     updateForm (mDataObject);
// }

void HandoutSummaryView::updateForm (DataObject *data)
{
    DataObjectHandover *_data = (DataObjectHandover*) data;

    if (_data)
    {
        mBarcodeLineEdit->setText(QString::number(_data->getKeychainId()));

        Database::eKeychainStatusId status = _data->getKeychainStatus();
        QString newStatus ("");

        switch (status)
        {
            case Database::eKeychainStatusId::TemporaryOut:
                newStatus = "Temporäre Ausgabe";
                break;
            case Database::eKeychainStatusId::AdministrationEnded:
                newStatus = "Verwaltungsabgabe";
                break;
            case Database::eKeychainStatusId::PermanentOut:
                newStatus = "Dauerhafte Ausgabe";
                break;
            case Database::eKeychainStatusId::Lost:
                newStatus = "Verloren";
                break;
            case Database::eKeychainStatusId::Available:
                newStatus = "Verfügbar (Rückgabe)";
                break;
            default:
                newStatus = "Fehler: Unbekannter Status";
                break;
        }
        mNewStatusEdit->setText(newStatus);

        mHandoverDateEdit->setText(_data->getDateHandover());
        mDeadlineDateEdit->setText(_data->getDateDeadline());
        mRecipientEdit->setText(_data->getRecipient());

        QString streetAndNumber = _data->getRecipientStreet();
        streetAndNumber.append(" ");
        streetAndNumber.append(QString::number(_data->getRecipientStreetNumber()));
        mRecipientStreetEdit->setText(streetAndNumber);

        QString areaCodeAndCity = QString::number(_data->getRecipientAreaCode());
        areaCodeAndCity.append(" ");
        areaCodeAndCity.append(_data->getRecipientCity());
        mRecipientAreaCodeAndCityEdit->setText(areaCodeAndCity);

        mRecipientSigneeEdit->setText(_data->getSignatureName());
        mNotesEdit->setText(_data->getAnnotation());
    }
}

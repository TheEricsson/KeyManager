#include "handoverview.h"
#include "signaturepad.h"
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCalendarWidget>
#include <QMessageBox>
#include <QTextEdit>
#include "dataobjecthandover.h"
#include "datainterface.h"
#include "iointerface.h"
#include "printerinterfacepdf.h"
#include "reportgenerator.h"

HandoverView::HandoverView (QWidget *parent)
    : WinSubmenu {parent}
{
    mSigPad = 0;

    setHeader("Zusammenfassung");

    QGridLayout *gridLayout = new QGridLayout ();

    QLabel *barcodeLabel = new QLabel ("Barcode", this);
    mBarcodeLineEdit = new QLineEdit (this);
    mBarcodeLineEdit->setDisabled(true);

    QLabel *keychainStatus = new QLabel ("Neuer Status", this);
    mNewStatusEdit = new QLineEdit (this);
    mNewStatusEdit->setDisabled(true);

    QLabel *handoverDate = new QLabel ("Übergabedatum", this);
    mHandoverDateEdit = new QLineEdit (this);
    mHandoverDateEdit->setDisabled(true);

    QLabel *deadlineDate = new QLabel ("Rückgabefrist", this);
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

    mSigPad = new SignaturePad ();
    gridLayout->addWidget(mSigPad, 9, 0, 1, 2);

    setCentralLayout(gridLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Repeat);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    connect (mSigPad, SIGNAL (signaturePaint ()), this, SLOT(onSignaturePaint ()));
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
            resetSignature ();
            break;
        case (Gui::Next):
            dataInterface()->setRecipientSigImg(mSigPad->getSignature());
            if (ioInterface()->dbInsertHandover(dataInterface()->getHandle()))
            {
                if (reportRequested())
                    pdfReport();
                //if (printRequested())
                //    printReport();
            }
            else
                insertDbErrorMessage();
        //no catch in this class, emit signal
        default:
            emit menuButtonClicked(btnType);
            break;
    }
}

void HandoverView::showEvent(QShowEvent *)
{
    showFullScreen();
    reset ();
}

void HandoverView::reset ()
{
    mBarcodeLineEdit->setText(QString::number(dataInterface()->getScannedCode()));
    KeychainStatus::Value status = dataInterface()->getNewKeychainStatusId();
    QString statusById = ioInterface()->getKeychainStatusText(status);
    mNewStatusEdit->setText(statusById);
    mHandoverDateEdit->setText(dataInterface()->getHandoverDate());
    mDeadlineDateEdit->setText(dataInterface()->getDeadlineDate());
    mRecipientEdit->setText(dataInterface()->getRecipientName());

    QString streetAndNumber = dataInterface ()->getRecipientStreet();
    streetAndNumber.append(" ");
    streetAndNumber.append(dataInterface()->getRecipientStreetNumber());
    mRecipientStreetEdit->setText(streetAndNumber);

    QString areaCodeAndCity = QString::number(dataInterface()->getRecipientAreaCode());
    areaCodeAndCity.append(" ");
    areaCodeAndCity.append(dataInterface()->getRecipientCity());
    mRecipientAreaCodeAndCityEdit->setText(areaCodeAndCity);

    mRecipientSigneeEdit->setText(dataInterface()->getRecipientSigName());
    mNotesEdit->setText(dataInterface()->getRecipientAnnotation());

    resetSignature ();
}

void HandoverView::resetSignature ()
{
    mSigPad->clearImage();

    if (!mSigPad->isModified())
    {
        disableButton(1, true);
        disableButton(2, true);
        mSigPad->update ();
    }
}

bool HandoverView::reportRequested()
{
    QMessageBox msgBox;
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText ("Protokoll");
    msgBox.setInformativeText("Die Schlüsselübergabe wurde vollständig verarbeitet.\nMöchten Sie ein Protokoll im PDF-Format erzeugen?");

    bool retVal = false;

    switch (msgBox.exec())
    {
        case QMessageBox::Yes:
            retVal = true;
            break;
        case QMessageBox::No:
            break;
        default:
            break;
    }

    return retVal;
}

bool HandoverView::printRequested()
{
    QMessageBox msgBox;
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setText ("Druck");
    msgBox.setInformativeText("Möchten Sie das Protokoll jetzt ausdrucken?");

    bool retVal = false;

    switch (msgBox.exec())
    {
    case QMessageBox::Yes:
        retVal = true;
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }

    return retVal;
}

void HandoverView::pdfReport()
{
    ReportGenerator generator (ioInterface());
    generator.createHandoverProtocol(ioInterface()->getLastHandoverId(), ReportGenerator::Pdf);
}

void HandoverView::printReport()
{
    ReportGenerator generator (ioInterface());
    generator.createHandoverProtocol(ioInterface()->getLastHandoverId(), ReportGenerator::Device);
}

void HandoverView::insertDbErrorMessage()
{
    QMessageBox msgBox;
    msgBox.setStandardButtons(QMessageBox::Abort);
    msgBox.setText ("Datenbankfehler!");
    QString errorText = "Datenübergabe an Datenbank fehlgeschlagen.\nFehlermeldung: ";
    errorText.append(ioInterface()->dbGetLastError());
    errorText.append("\nRückkehr zum Hauptmenü. Bitte den Vorgang erneut starten.");
    msgBox.setInformativeText(errorText);
    msgBox.exec();
}

HandoverView::~HandoverView()
{
    if (mSigPad)
    {
        mSigPad->deleteLater();
    }
}

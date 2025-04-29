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
#include "printerinterfacepdf.h".h"

HandoverView::HandoverView (QWidget *parent)
    : WinSubmenu {parent}
{
    mSigPad = 0;

    setHeader("Ausgabe abschließen");

    //clear ();

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

    layout()->addItem(gridLayout);
    mSigPad = new SignaturePad ();
    layout()->addWidget(mSigPad);

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
            ioInterface()->dbInsertHandover(dataInterface()->getHandle());
            if (reportRequested())
                generateReport();
        //no catch in this class, emit signal
        default:
            emit menuButtonClicked(btnType);
            break;
    }
}

void HandoverView::showEvent(QShowEvent *)
{
    reset ();
}

void HandoverView::reset ()
{
    mBarcodeLineEdit->setText(QString::number(dataInterface()->getScannedCode()));
    Database::KeychainStatus status = dataInterface()->getNewKeychainStatusId();
    QString statusById = ioInterface()->getKeychainStatusText(status);
    mNewStatusEdit->setText(statusById);
    mHandoverDateEdit->setText(dataInterface()->getHandoverDate());
    mDeadlineDateEdit->setText(dataInterface()->getDeadlineDate());
    mRecipientEdit->setText(dataInterface()->getRecipientName());

    QString streetAndNumber = dataInterface ()->getRecipientStreet();
    streetAndNumber.append(" ");
    streetAndNumber.append(QString::number(dataInterface()->getRecipientStreetNumber()));
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
    msgBox.setText ("Protokoll");
    msgBox.setInformativeText("Möchten Sie ein Protokoll (PDF-Format) erzeugen?");

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

void HandoverView::generateReport()
{
    PrinterInterface *pdfPrinter = new PrinterInterfacePdf ();

    pdfPrinter->saveAsFile();
    pdfPrinter->begin();

    // header
    pdfPrinter->insertHeader("Schlüsselübergabe");
    pdfPrinter->insertHLine();

    // address data of property
    pdfPrinter->insertText("Objektadresse:");
    int addressId = dataInterface()->getKeychainAddressId();
    QString address = ioInterface()->getAddressStreet(addressId);
    address.append(" ");
    address.append(ioInterface()->getAddressStreetNumber(addressId));
    pdfPrinter->insertText(address);
    QString addressAreaCodeAndCity = QString::number(ioInterface()->getAddressAreaCode(addressId));
    address.append(" ");
    address.append(ioInterface()->getAddressCity(addressId));
    pdfPrinter->insertText(addressAreaCodeAndCity);
    pdfPrinter->insertHLine();

    // internal data of keychain
    pdfPrinter->insertText("Daten zum Schlüsselbund: ");
    QString keyCode = "Code: ";
    keyCode.append(QString::number(dataInterface()->getScannedCode()));
    pdfPrinter->insertText(keyCode);
    QString internalLoc = "Interne Zuordnung: ";
    internalLoc.append(QString::number(dataInterface()->getInternalLocation()));
    pdfPrinter->insertText(internalLoc);
    pdfPrinter->insertHLine();

    // recipient data
    pdfPrinter->insertText("Schlüsselempfänger:");
    QString recipientName = "Empfänger: ";
    recipientName.append(dataInterface()->getRecipientName());
    pdfPrinter->insertText(recipientName);
    QString recipientStreet;
    recipientStreet = dataInterface()->getRecipientStreet();
    recipientStreet.append(" ");
    recipientStreet.append(QString::number(dataInterface()->getRecipientStreetNumber()));
    pdfPrinter->insertText(recipientStreet);
    QString recipientAreaCodeAndCity;
    recipientAreaCodeAndCity = QString::number(dataInterface()->getRecipientAreaCode());
    recipientAreaCodeAndCity.append(" ");
    recipientAreaCodeAndCity.append(dataInterface()->getRecipientCity());
    pdfPrinter->insertText(recipientAreaCodeAndCity);
    QString signee = dataInterface()->getRecipientSigName();
    if ("" != signee)
    {
        signee.prepend("Unterzeichner: ");
        pdfPrinter->insertText(signee);
    }
    pdfPrinter->insertHLine();

    //additional infos
    QString addInfo = dataInterface()->getRecipientAnnotation();
    if ("" != addInfo)
    {
        addInfo.prepend("Zusätzliche Informationen: ");
        pdfPrinter->insertText(addInfo);
        pdfPrinter->insertHLine();
    }

    //handout date and deadline
    pdfPrinter->insertText("Aus- und Rückgabedaten:");

    Database::KeychainStatus status = dataInterface()->getNewKeychainStatusId();
    QString statusById = ioInterface()->getKeychainStatusText(status);
    statusById.prepend("Ausgabedauer: ");
    pdfPrinter->insertText(statusById);
    QString handoverDate = "Ausgabedatum: ";
    handoverDate.append(dataInterface()->getHandoverDate());
    pdfPrinter->insertText(handoverDate);

    QString deadline = dataInterface()->getDeadlineDate();
    if ("" == deadline)
    {
        deadline = "unbegrenzt";
    }
    deadline.prepend("Rückgabefrist: ");
    pdfPrinter->insertText(deadline);
    pdfPrinter->insertHLine();

    pdfPrinter->insertText("Unterschrift des Empfängers:");
    pdfPrinter->insertImage(dataInterface()->getRecipientSignature(), PrinterInterface::Medium, PrinterInterface::Left);
    pdfPrinter->insertHLine();

    pdfPrinter->finish();

    delete pdfPrinter;
}

HandoverView::~HandoverView()
{
    if (mSigPad)
    {
        delete mSigPad;
        mSigPad = 0;
    }
}

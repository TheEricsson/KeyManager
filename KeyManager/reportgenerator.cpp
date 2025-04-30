#include "reportgenerator.h"
#include "printerinterfacepdf.h"
#include "printerinterfacedevice.h"
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>

ReportGenerator::ReportGenerator(IOInterface *ioInterface)
{
    mIOInterface = ioInterface;
}

void ReportGenerator::createHandoverProtocol(unsigned int id, ReportGenerator::OutputFormat outputFormat)
{
    PrinterInterface *printer;
    bool showPdfFile = false;

    switch (outputFormat)
    {
        case ReportGenerator::Pdf:
            printer = new PrinterInterfacePdf();
            printer->saveAsFile();
            showPdfFile = true;
            break;
        case ReportGenerator::Device:
            printer = new PrinterInterfaceDevice();
            break;
        default:
            printer = new PrinterInterfacePdf();
    }

    printer->begin();

    // company logo
    QPixmap logo (":/images/logo.png");
    QPixmap logo_scaled = logo.scaledToWidth(Gui::logoWidth, Qt::SmoothTransformation);
    printer->insertImage(logo_scaled.toImage(), PrinterInterface::Medium, PrinterInterface::Right);

    // header
    printer->insertHeaderH1("Schlüsselübergabe");
    printer->insertHLine();

    // company data
    printer->insertHeaderH2("Verwalter:");
    printer->insertText("Alkenbrecher Haus- und Immobilienverwaltung GmbH");
    printer->insertText("Schillerstraße 4");
    printer->insertText("99096 Erfurt");
    printer->insertHLine();

    // address data of property
    printer->insertHeaderH2("Objektadresse:");
    int keyCode = mIOInterface->getValue ("handovers", "keychainId", id).toInt();
    int addressId = mIOInterface->getKeychainAddressId(keyCode);
    QString address = mIOInterface->getAddressStreet(addressId);
    address.append(" ");
    address.append(mIOInterface->getAddressStreetNumber(addressId));
    printer->insertText(address);
    QString addressAreaCodeAndCity = QString::number(mIOInterface->getAddressAreaCode(addressId));
    address.append(" ");
    address.append(mIOInterface->getAddressCity(addressId));
    printer->insertText(addressAreaCodeAndCity);
    printer->insertHLine();

    // internal data of keychain
    printer->insertHeaderH2("Daten zum Schlüsselbund: ");
    QString keyCodeString = "Code: ";
    keyCodeString.append(Database::normaliseKeycode(keyCode));
    printer->insertText(keyCodeString);
    QString internalLoc = "Interne Zuordnung: ";
    internalLoc.append(QString::number(mIOInterface->getKeychainInternalLocation(keyCode)));
    printer->insertText(internalLoc);
    printer->insertHLine();

    // recipient data
    printer->insertHeaderH2("Schlüsselempfänger:");
    printer->insertText(mIOInterface->getValue("handovers", "recipient", id).toString());
    QString recipientStreet;
    recipientStreet = mIOInterface->getValue("handovers", "recipientStreet", id).toString();
    recipientStreet.append(" ");
    recipientStreet.append(mIOInterface->getValue("handovers", "recipientStreetNumber", id).toString());
    printer->insertText(recipientStreet);
    QString recipientAreaCodeAndCity;
    recipientAreaCodeAndCity = QString::number(mIOInterface->getValue("handovers", "recipientAreaCode", id).toInt());
    recipientAreaCodeAndCity.append(" ");
    recipientAreaCodeAndCity.append(mIOInterface->getValue("handovers", "recipientCity", id).toString());
    printer->insertText(recipientAreaCodeAndCity);
    QString signee = mIOInterface->getValue("handovers", "signatureName", id).toString();
    if ("" != signee)
    {
        QString signeeHeader ("Unterzeichner:");
        printer->insertHeaderH2(signeeHeader);
        printer->insertText(signee);
    }
    printer->insertHLine();

    //additional infos
    QString addInfo = mIOInterface->getValue("handovers", "annotation", id).toString();
    if ("" != addInfo)
    {
        QString addInfoHeader("Zusätzliche Informationen: ");
        printer->insertHeaderH2(addInfoHeader);
        printer->insertText(addInfo);
        printer->insertHLine();
    }

    //handout date and deadline
    printer->insertHeaderH2("Aus- und Rückgabedaten:");

    QString duration= mIOInterface->getValue("handovers", "duration", id).toString();
    duration.prepend("Ausgabedauer: ");
    printer->insertText(duration);
    QString handoverDate = "Ausgabedatum: ";
    handoverDate.append(mIOInterface->getValue("handovers", "dateHandover", id).toString());
    printer->insertText(handoverDate);

    QString deadline = (mIOInterface->getValue("handovers", "dateDeadline", id).toString());
    if ("" == deadline)
    {
        deadline = "---";
    }
    deadline.prepend("Rückgabefrist: ");
    printer->insertText(deadline);
    printer->insertHLine();

    QImage imgKeychain;

    if (mIOInterface->getKeychainImg(keyCode, imgKeychain))
    {
        if (!imgKeychain.isNull())
        {
            if (imgKeychain.height() > imgKeychain.width())
            {
                QImage img = imgKeychain.transformed(QTransform().rotate(90.0));
                imgKeychain = img;
            }
            printer->insertHeaderH2("Schlüsselbund:");
            printer->insertImage(imgKeychain, PrinterInterface::Medium, PrinterInterface::Left);
            printer->insertHLine();
        }
    }

    int getNumberOfEntries (const QString &tableName);
    QVariant getValue (const QString &tableName, const QString& columnName, int index);

    printer->insertHeaderH2("Unterschrift des Empfängers:");
    QByteArray sigByteArray = mIOInterface->getValue("handovers", "signature", id).toByteArray();
    QImage sigImage;
    mIOInterface->byteArrayToImage(sigByteArray, sigImage);
    printer->insertImage(sigImage, PrinterInterface::Small, PrinterInterface::Left);
    printer->insertHLine();

    printer->finish();

    if (showPdfFile)
    {
        //open file with the default app
        QDesktopServices::openUrl(QUrl(printer->getFilePath()));
    }

    delete printer;
}

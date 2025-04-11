#include "datainterface.h"
#include "viewdatakeychain.h"
#include "viewdatahandover.h"
#include "viewdatarecipient.h"
#include "viewdatareturndate.h"
#include "viewdatascanner.h"
#include "viewdataannotation.h"
#include <QBuffer>
#include <QPixmap>

DataInterface::DataInterface()
{
    mDataHandover = new ViewDataHandover();
    mDataRecipient = new ViewDataRecipient();
    mDataReturnDate = new ViewDataReturnDate();
    mDataScanner = new ViewDataScanner();
    mDataKeychain = new ViewDataKeychain();
    mDataAnnotation = new ViewDataAnnotation();
}

Database::KeychainStatus DataInterface::getKeychainStatusId ()
{
    return mDataKeychain->getStatus();
}

Database::KeychainStatus DataInterface::getNewKeychainStatusId ()
{
    return mDataKeychain->getNewStatus();
}

int DataInterface::getKeychainAddressId ()
{
    return mDataKeychain->getAddressId();
}

int DataInterface::getInternalLocation ()
{
    return mDataKeychain->getInternalLocation();
}

bool DataInterface::setKeychainInternalLocation (const int location)
{
    if (!mDataKeychain)
        return false;

    mDataKeychain->setInternalLocation(location);
    return true;
}

bool DataInterface::setKeychainAddressId (const int addressId)
{
    if (!mDataKeychain)
        return false;

    mDataKeychain->setAddressId(addressId);
    return true;
}

const QString DataInterface::getHandoverDate ()
{
    if (!mDataRecipient)
        return "00.00.0000";

    return mDataRecipient->getCurrentDate();
}

const QString DataInterface::getDeadlineDate ()
{
    if (!mDataRecipient)
        return "00.00.0000";

    return mDataRecipient->getDeadlineDate();
}

const QString DataInterface::getRecipientName ()
{
    if (!mDataRecipient)
        return "DataInterface::getRecipientName (): mDataRecipient is NULL!";

    return mDataRecipient->getRecipientName();
}

const QString DataInterface::getRecipientStreet ()
{
    if (!mDataRecipient)
        return "DataInterface::getRecipientStreet (): mDataRecipient is NULL!";

    return mDataRecipient->getRecipientStreet();
}

int DataInterface::getRecipientStreetNumber ()
{
    if (!mDataRecipient)
        return _UNDEFINED;

    return mDataRecipient->getRecipientStreetNumber();
}

int DataInterface::getRecipientAreaCode ()
{
    if (!mDataRecipient)
        return _UNDEFINED;

    return mDataRecipient->getRecipientAreaCode();
}

const QString DataInterface::getRecipientCity ()
{
    if (!mDataRecipient)
        return "DataInterface::getRecipientCity (): mDataRecipient is NULL!";

    return mDataRecipient->getRecipientCity();
}

const QString DataInterface::getRecipientSigName ()
{
    if (!mDataRecipient)
        return "DataInterface::getRecipientSigName (): mDataRecipient is NULL!";

    return mDataRecipient->getSignatureName();
}

const QImage DataInterface::getRecipientSignature ()
{   
    return mDataRecipient->getSignatureImg();
}

const QString DataInterface::getRecipientAnnotation ()
{
    if (!mDataAnnotation)
        return "";

    return mDataAnnotation->getAnnotation();
}

bool DataInterface::setKeychainStatusId (Database::KeychainStatus status)
{
    if (!mDataKeychain)
        return false;

    mDataKeychain->setStatus(status);
    return true;
}

bool DataInterface::setNewKeychainStatusId (Database::KeychainStatus status)
{
    if (!mDataKeychain)
        return false;

    mDataKeychain->setNewStatus(status);
    return true;
}

bool DataInterface::setRecipientName(const QString& name)
{
    if (!mDataRecipient)
        return false;

    mDataRecipient->setRecipientName(name);
    return true;
}

bool DataInterface::setRecipientType(const QString& type)
{
    if (!mDataRecipient)
        return false;

    mDataRecipient->setRecipientType(type);
    return true;
}

bool DataInterface::setRecipientStreet(const QString& street)
{
    if (!mDataRecipient)
        return false;

    mDataRecipient->setRecipientStreet(street);
    return true;
}

bool DataInterface::setRecipientStreetNumber(const int streetNr)
{
    if (!mDataRecipient)
        return false;

    mDataRecipient->setRecipientStreetNumber(streetNr);
    return true;
}

bool DataInterface::setRecipientAreaCode(const int areaCode)
{
    if (!mDataRecipient)
        return false;

    mDataRecipient->setRecipientAreaCode(areaCode);
    return true;
}

bool DataInterface::setRecipientCity(const QString& city)
{
    if (!mDataRecipient)
        return false;

    mDataRecipient->setRecipientCity(city);
    return true;
}

bool DataInterface::setRecipientSigName (const QString &name)
{
    if (!mDataRecipient)
        return false;

    mDataRecipient->setSignatureName(name);
    return true;
}

bool DataInterface::setRecipientSigImg (QImage sigImg)
{
    if (!mDataRecipient)
        return false;

    mDataRecipient->setSignatureImg(sigImg);
    return true;
}

bool DataInterface::setRecipientCurrentDate (const QString& date)
{
    if (!mDataRecipient)
        return false;

    mDataRecipient->setCurrentDate(date);
    return true;
}

bool DataInterface::setRecipientDeadlineDate (const QString& date)
{
    if (!mDataRecipient)
        return false;

    mDataRecipient->setDeadlineDate(date);
    return true;
}

bool DataInterface::setAnnotationText(const QString& text)
{
    if (!mDataAnnotation)
        return false;

    mDataAnnotation->setAnnotation(text);
    return true;
}

bool DataInterface::setScannedCode (const int keyCode)
{
    if (!mDataScanner)
        return false;

    mDataScanner->setBarcode(keyCode);
    return true;
}

void DataInterface::resetRecipientData ()
{
    if (mDataRecipient)
        delete mDataRecipient;

    mDataRecipient = new ViewDataRecipient();
}

void DataInterface::resetAnnotationData ()
{
    if (mDataAnnotation)
        delete mDataAnnotation;

    mDataAnnotation = new ViewDataAnnotation();
}

void DataInterface::resetHandoverData ()
{
    if (mDataHandover)
        delete mDataHandover;

    mDataHandover = new ViewDataHandover();
}

void DataInterface::resetKeychainData ()
{
    if (mDataKeychain)
        delete mDataKeychain;

    mDataKeychain = new ViewDataKeychain();
}

void DataInterface::resetReturnDateData ()
{
    if (mDataReturnDate)
        delete mDataReturnDate;

    mDataReturnDate = new ViewDataReturnDate();
}

void DataInterface::resetScannerData ()
{
    if (mDataScanner)
        delete mDataScanner;

    mDataScanner = new ViewDataScanner();
}

const QImage DataInterface::getKeychainImg ()
{
    return mDataKeychain->getImg();
}

int DataInterface::getScannedCode ()
{
    if (mDataScanner)
        return mDataScanner->getBarcode();
    else
        return _UNDEFINED;
}

void DataInterface::setData (ViewDataAnnotation* data)
{
    mDataAnnotation = data;
}

void DataInterface::setData (ViewDataHandover* data)
{
    mDataHandover = data;
}

void DataInterface::setData (ViewDataKeychain* data)
{
    mDataKeychain = data;
}

void DataInterface::setData (ViewDataRecipient* data)
{
    mDataRecipient = data;
}

void DataInterface::setData (ViewDataReturnDate* data)
{
    mDataReturnDate = data;
}

void DataInterface::setData (ViewDataScanner* data)
{
    mDataScanner = data;
}

DataInterface::~DataInterface()
{
    if (0 != mDataHandover)
    {
        delete mDataHandover;
        mDataHandover = 0;
    }
    if (0 != mDataKeychain)
    {
        delete mDataKeychain;
        mDataKeychain = 0;
    }
    if (0 != mDataRecipient)
    {
        delete mDataRecipient;
        mDataRecipient = 0;
    }
    if (0 != mDataReturnDate)
    {
        delete mDataReturnDate;
        mDataReturnDate = 0;
    }
    if (0 != mDataScanner)
    {
        delete mDataScanner;
        mDataScanner = 0;
    }
    if (0 != mDataAnnotation)
    {
        delete mDataAnnotation;
        mDataAnnotation = 0;
    }
}

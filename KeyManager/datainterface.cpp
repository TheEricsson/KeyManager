#include "datainterface.h"
#include "viewdatakeychain.h"
#include "viewdatahandover.h"
#include "viewdatarecipient.h"
#include "viewdatareturndate.h"
#include "viewdatascanner.h"
#include "viewdataannotation.h"
#include "databaseimpl.h"

DataInterface::DataInterface()
{
    mDataHandover = 0;
    mDataRecipient = 0;
    mDataReturnDate = 0;
    mDataScanner = 0;
    mDataKeychain = 0;
    mDataAnnotation = 0;

    mDatabase = new DatabaseImpl ();
}

bool DataInterface::initKeyOverviewModel (QSqlRelationalTableModel *model, const QString& filter)
{
    return mDatabase->initKeyOverviewModel(model, filter);
}

bool DataInterface::initKeychainModel (QSqlRelationalTableModel *model, const QString& filter)
{
    return mDatabase->initKeychainModel(model, filter);
}

bool DataInterface::initRecipientModel (QSqlRelationalTableModel *model)
{
    return mDatabase->initRecipientModel(model);
}

bool DataInterface::findKeycode (int keyCode)
{
    return mDatabase->findKeyCode(keyCode);
}

// Database::eKeychainStatusId DataInterface::getKeychainStatusId (int keyCode)
// {
//     return mDatabase->getKeychainStatusId(keyCode);
// }

Database::KeychainStatus DataInterface::getKeychainStatusId ()
{
    return mDataKeychain->getStatus();
}

Database::KeychainStatus DataInterface::getNewKeychainStatusId ()
{
    return mDataKeychain->getNewStatus();
}

int DataInterface::getInternalLocation ()
{
    return mDatabase->getKeychainInternalLocation(getScannedCode());
}

const QString DataInterface::getKeychainStatusText (int statusId)
{
    return mDatabase->getKeychainStatusText(statusId);
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

bool DataInterface::setRecipientSigImg (QImage sigImg)
{
    if (!mDataRecipient)
        return false;

    mDataRecipient->setSignatureImg(sigImg);
    return true;
}

bool DataInterface::setDeadlineDate (const QString& date)
{
    if (!mDataRecipient)
        return false;

    mDataRecipient->setDeadlineDate(date);
    return true;
}

bool DataInterface::submitHandover ()
{
    bool retVal = false;

    retVal = mDatabase->dbInsertHandover(this);

    if (retVal)
    {
        retVal = mDatabase->dbCleanupTable ("handovers", Database::maxHandoverEntries);
    }

    // if (0 != mDataHandover)
    // {
    //     delete mDataHandover;
    //     mDataHandover = 0;
    // }
    // if (0 != mDataKeychain)
    // {
    //     delete mDataKeychain;
    //     mDataKeychain = 0;
    // }
    // if (0 != mDataRecipient)
    // {
    //     delete mDataRecipient;
    //     mDataRecipient = 0;
    // }
    // if (0 != mDataReturnDate)
    // {
    //     delete mDataReturnDate;
    //     mDataReturnDate = 0;
    // }
    // if (0 != mDataScanner)
    // {
    //     delete mDataScanner;
    //     mDataScanner = 0;
    // }

    return retVal;
}

bool DataInterface::resetKeychainData ()
{
    if (mDataScanner)
    {
        // free memory before updating data
        if (mDataKeychain)
        {
            delete mDataKeychain;
            mDataKeychain = 0;
        }

        mDataKeychain = new ViewDataKeychain ();
        return mDatabase->setKeychainData(mDataKeychain, mDataScanner->getBarcode());
    }
    return false;
}

bool DataInterface::resetRecipientData ()
{
    // if (mDataRecipient)
    //     delete mDataRecipient;

    mDataRecipient = new ViewDataRecipient();

    return true;
}

const QString DataInterface::getKeychainImgPath ()
{
    if (!mDataKeychain)
    {
        return "";
    }
    return mDataKeychain->getImgPath();
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
    // if (mDataAnnotation)
    //     delete mDataAnnotation;

    mDataAnnotation = data;
}

void DataInterface::setData (ViewDataHandover* data)
{
    // if (mDataHandover)
    //     delete mDataHandover;

    mDataHandover = data;
}

void DataInterface::setData (ViewDataKeychain* data)
{
    // if (mDataKeychain)
    //     delete mDataKeychain;

    mDataKeychain = data;
}

void DataInterface::setData (ViewDataRecipient* data)
{
    // if (mDataRecipient)
    //     delete mDataRecipient;

    mDataRecipient = data;
}

void DataInterface::setData (ViewDataReturnDate* data)
{
    // if (mDataReturnDate)
    //     delete mDataReturnDate;

    mDataReturnDate = data;
}

void DataInterface::setData (ViewDataScanner* data)
{
    // if (mDataScanner)
    //     delete mDataScanner;

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

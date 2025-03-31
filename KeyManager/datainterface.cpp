#include "datainterface.h"
#include "viewdatakeychainstatus.h"
#include "viewdatahandover.h"
#include "viewdatarecipient.h"
#include "viewdatareturndate.h"
#include "viewdatascanner.h"
#include "databaseimpl.h"

DataInterface::DataInterface()
{
    mDataHandover = 0;
    mDataKeychainStatus = 0;
    mDataRecipient = 0;
    mDataReturnDate = 0;
    mDataScanner = 0;

    mDatabase = new DatabaseImpl ();
}

bool DataInterface::initKeyOverviewModel (QSqlRelationalTableModel *model, int keyCode)
{
    return mDatabase->initKeyOverviewModel(model, keyCode);
}

bool DataInterface::initKeychainModel (QSqlRelationalTableModel *model, int keyCode)
{
    return mDatabase->initKeychainModel(model, keyCode);
}

bool DataInterface::initRecipientModel (QSqlRelationalTableModel *model)
{
    return mDatabase->initRecipientModel(model);
}

Database::eKeychainStatusId DataInterface::getKeychainStatusId (int keyCode)
{
    return mDatabase->getKeychainStatusId(keyCode);
}

void DataInterface::setData (ViewDataHandover* data)
{
    if (mDataHandover)
        delete mDataHandover;

    mDataHandover = data;
}

void DataInterface::setData (ViewDataKeychainStatus* data)
{
    if (mDataKeychainStatus)
        delete mDataKeychainStatus;

    mDataKeychainStatus = data;
}

void DataInterface::setData (ViewDataRecipient* data)
{
    if (mDataRecipient)
        delete mDataRecipient;

    mDataRecipient = data;
}

void DataInterface::setData (ViewDataReturnDate* data)
{
    if (mDataReturnDate)
        delete mDataReturnDate;

    mDataReturnDate = data;
}

void DataInterface::setData (ViewDataScanner* data)
{
    if (mDataScanner)
        delete mDataScanner;

    mDataScanner = data;
}

DataInterface::~DataInterface()
{
    // todo!... segfault

    if (0 != mDataHandover)
    {
        delete mDataHandover;
        mDataHandover = 0;
    }
    if (0 != mDataKeychainStatus)
    {
        delete mDataKeychainStatus;
        mDataKeychainStatus = 0;
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
}

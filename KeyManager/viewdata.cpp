#include "viewdata.h"
#include "viewdatakeychainstatus.h"
#include "viewdatahandover.h"
#include "viewdatarecipient.h"
#include "viewdatareturndate.h"
#include "viewdatascanner.h"

ViewData::ViewData()
{
    mDataHandover = 0;
    mDataKeychainStatus = 0;
    mDataRecipient = 0;
    mDataReturnDate = 0;
    mDataScanner = 0;
}

void ViewData::setData (ViewDataHandover* data)
{
    if (mDataHandover)
        delete mDataHandover;

    mDataHandover = data;
}

void ViewData::setData (ViewDataKeychainStatus* data)
{
    if (mDataKeychainStatus)
        delete mDataKeychainStatus;

    mDataKeychainStatus = data;
}

void ViewData::setData (ViewDataRecipient* data)
{
    if (mDataRecipient)
        delete mDataRecipient;

    mDataRecipient = data;
}

void ViewData::setData (ViewDataReturnDate* data)
{
    if (mDataReturnDate)
        delete mDataReturnDate;

    mDataReturnDate = data;
}

void ViewData::setData (ViewDataScanner* data)
{
    if (mDataScanner)
        delete mDataScanner;

    mDataScanner = data;
}

ViewData::~ViewData()
{
    // todo!... segfault

    // if (0 != mDataHandover)
    // {
    //     delete mDataHandover;
    //     mDataHandover = 0;
    // }
    // if (0 != mDataKeychainStatus)
    // {
    //     delete mDataKeychainStatus;
    //     mDataKeychainStatus = 0;
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
}

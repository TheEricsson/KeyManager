#include "viewdatarecipient.h"
#include "globals.h"

ViewDataRecipient::ViewDataRecipient()
{
    mCurrentDate = "";
    mDeadlineDate = "";
    mName = "";
    mStreet = "";
    mRecipientType = "";
    mStreetNumber = _UNDEFINED;
    mAreaCode = _UNDEFINED;
    mCity = "";
    mSignee = "";
    mNewKeychainStatus = Database::KeychainStatus::Undefined;
}

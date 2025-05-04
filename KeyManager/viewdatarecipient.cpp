#include "viewdatarecipient.h"
#include "globals.h"

ViewDataRecipient::ViewDataRecipient()
{
    mCurrentDate = "";
    mDeadlineDate = "";
    mName = "";
    mStreet = "";
    mRecipientType = "";
    mStreetNumber = "";
    mAreaCode = _UNDEFINED;
    mCity = "";
    mSignee = "";
    mNewKeychainStatus = KeychainStatus::Value::Undefined;
}

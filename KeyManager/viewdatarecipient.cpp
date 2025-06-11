#include "viewdatarecipient.h"
#include "globals.h"

ViewDataRecipient::ViewDataRecipient()
{
    mCurrentDate = "";
    mDeadlineDate = "";
    mName = "";
    mStreet = "";
    mRecipientType = RecipientType::Undefined;
    mStreetNumber = "";
    mAreaCode = _UNDEFINED;
    mCity = "";
    mSignee = "";
    mNewKeychainStatus = KeychainStatus::Undefined;
}

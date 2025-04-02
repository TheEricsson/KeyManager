#include "viewdatakeychain.h"

ViewDataKeychain::ViewDataKeychain()
{
    mInternalLocation = _UNDEFINED;
    mAddressId = _UNDEFINED;
    mImgPath = "";
    mStatusId = Database::KeychainStatus::Undefined;
    mNewStatusId = Database::KeychainStatus::Undefined;
}

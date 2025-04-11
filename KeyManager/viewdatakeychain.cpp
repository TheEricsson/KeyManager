#include "viewdatakeychain.h"

ViewDataKeychain::ViewDataKeychain()
{
    mInternalLocation = _UNDEFINED;
    mAddressId = _UNDEFINED;
    mStatusId = Database::KeychainStatus::Undefined;
    mNewStatusId = Database::KeychainStatus::Undefined;
}

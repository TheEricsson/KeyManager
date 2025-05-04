#include "viewdatakeychain.h"

ViewDataKeychain::ViewDataKeychain()
{
    mInternalLocation = _UNDEFINED;
    mAddressId = _UNDEFINED;
    mStatusId = KeychainStatus::Value::Undefined;
    mNewStatusId = KeychainStatus::Value::Undefined;
}

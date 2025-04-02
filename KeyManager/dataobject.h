#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include "globals.h"

class DataObject
{
    public:
        DataObject();
        void setKeychainId (const int id) {mKeychainId = id;};
        void setInternalLocation (const int id) {mInternalLocation = id;};
        void setKeychainStatus (const Database::KeychainStatus id) {mKeychainStatus = id;};

        int getKeychainId () {return mKeychainId;};
        int getInternalLocation () {return mInternalLocation;};
        Database::KeychainStatus getKeychainStatus () {return mKeychainStatus;};

    private:
        int mKeychainId;
        int mInternalLocation;
        Database::KeychainStatus mKeychainStatus;
};

#endif // DATAOBJECT_H

#ifndef VIEWDATAKEYCHAIN_H
#define VIEWDATAKEYCHAIN_H

#include "globals.h";

class ViewDataKeychain
{
    public:
        ViewDataKeychain();
        void setInternalLocation (const int& id){mInternalLocation = id;};
        void setStatus (const Database::KeychainStatus& statusId){mStatusId = statusId;};
        void setNewStatus (const Database::KeychainStatus& statusId){mNewStatusId = statusId;};
        void setAddressId (const int& id){mAddressId = id;};
        void setImgPath (const QString& path){mImgPath = path;};

        const int& getInternalLocation (){return mInternalLocation;};
        const Database::KeychainStatus& getStatus (){return mStatusId;};
        const Database::KeychainStatus& getNewStatus (){return mNewStatusId;};
        const int& getAddressId () {return mAddressId;};
        const QString& getImgPath () {return mImgPath;};
    private:
        int mInternalLocation;
        int mAddressId;
        QString mImgPath;
        Database::KeychainStatus mStatusId;
        Database::KeychainStatus mNewStatusId;
};

#endif // VIEWDATAKEYCHAIN_H

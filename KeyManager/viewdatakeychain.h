#ifndef VIEWDATAKEYCHAIN_H
#define VIEWDATAKEYCHAIN_H

#include <QImage>
#include "globals.h"

class ViewDataKeychain
{
    public:
        ViewDataKeychain();
        void setInternalLocation (const int& id){mInternalLocation = id;};
        void setStatus (const Database::KeychainStatus& statusId){mStatusId = statusId;};
        void setNewStatus (const Database::KeychainStatus& statusId){mNewStatusId = statusId;};
        void setAddressId (const int& id){mAddressId = id;};
        void setImg (const QImage& img){mImg = img;};

        const int& getInternalLocation (){return mInternalLocation;};
        const Database::KeychainStatus& getStatus (){return mStatusId;};
        const Database::KeychainStatus& getNewStatus (){return mNewStatusId;};
        const int& getAddressId () {return mAddressId;};
        const QImage& getImg () {return mImg;};
    private:
        int mInternalLocation;
        int mAddressId;
        QImage mImg;
        Database::KeychainStatus mStatusId;
        Database::KeychainStatus mNewStatusId;
};

#endif // VIEWDATAKEYCHAIN_H

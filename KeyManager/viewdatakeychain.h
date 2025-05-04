#ifndef VIEWDATAKEYCHAIN_H
#define VIEWDATAKEYCHAIN_H

#include <QImage>
#include "globals.h"

class ViewDataKeychain
{
    public:
        ViewDataKeychain();
        void setInternalLocation (const int& id){mInternalLocation = id;};
        void setStatus (const KeychainStatus::Value& statusId){mStatusId = statusId;};
        void setNewStatus (const KeychainStatus::Value& statusId){mNewStatusId = statusId;};
        void setAddressId (const int& id){mAddressId = id;};
        void setImg (const QImage& img){mImg = img;};

        const int& getInternalLocation (){return mInternalLocation;};
        const KeychainStatus::Value& getStatus (){return mStatusId;};
        const KeychainStatus::Value& getNewStatus (){return mNewStatusId;};
        const int& getAddressId () {return mAddressId;};
        const QImage& getImg () {return mImg;};
    private:
        int mInternalLocation;
        int mAddressId;
        QImage mImg;
        KeychainStatus::Value mStatusId;
        KeychainStatus::Value mNewStatusId;
};

#endif // VIEWDATAKEYCHAIN_H

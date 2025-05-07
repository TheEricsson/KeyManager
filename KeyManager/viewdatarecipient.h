#ifndef VIEWDATARECIPIENT_H
#define VIEWDATARECIPIENT_H

#include <QString>
#include <QImage>
#include "globals.h"

class ViewDataRecipient
{
    public:
        ViewDataRecipient();
        void setCurrentDate (const QString& date){mCurrentDate = date;};
        void setDeadlineDate (const QString& date){mDeadlineDate = date;};
        void setRecipientName (const QString& name) {mName = name;};
        void setRecipientStreet (const QString& street) {mStreet = street;};
        void setRecipientStreetNumber (const QString& streetNumber) {mStreetNumber = streetNumber;};
        void setRecipientAreaCode (const int& areaCode) {mAreaCode = areaCode;};
        void setRecipientCity (const QString& city) {mCity = city;};
        void setRecipientType (const QString& recipientType) {mRecipientType = recipientType;};
        void setSignatureName (const QString& signee) {mSignee = signee;};
        void setSignatureImg (const QImage& sigImage) {mSigImg = sigImage;};
        void setNewKeychainStatus (KeychainStatus::Value newStatus) {mNewKeychainStatus= newStatus;};

        const QString& getCurrentDate (){return mCurrentDate;};
        const QString& getDeadlineDate (){return mDeadlineDate;};
        const QString& getRecipientName () {return mName;};
        const QString& getRecipientStreet () {return mStreet;};
        const QString& getRecipientStreetNumber () {return mStreetNumber;};
        const int& getRecipientAreaCode () {return mAreaCode;};
        const QString& getRecipientCity () {return mCity;};
        const QString& getRecipientType () {return mRecipientType;};
        const QString& getSignatureName () {return mSignee;};
        const QImage& getSignatureImg () {return mSigImg;};
        KeychainStatus::Value getNewKeychainStatus () {return mNewKeychainStatus;};

    private:
        QString mCurrentDate;
        QString mDeadlineDate;
        QString mName;
        QString mStreet;
        QString mRecipientType;
        QString mStreetNumber;
        int mAreaCode;
        QString mCity;
        QString mSignee;
        QImage mSigImg;
        KeychainStatus::Value mNewKeychainStatus;
};

#endif // VIEWDATARECIPIENT_H

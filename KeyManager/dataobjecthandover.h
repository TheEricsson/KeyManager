#ifndef DATAOBJECTHANDOVER_H
#define DATAOBJECTHANDOVER_H

#include "dataobject.h"
#include "qcontainerfwd.h"
#include "globals.h"
#include <QImage>

class DataObjectHandover : public DataObject
{
    public:
        DataObjectHandover();
        void setDateHandover (const QString date) {mDateHandover = date;};
        void setDateDeadline (const QString date) {mDateDeadline = date;};
        void setRecipient (const QString recipient) {mRecipient = recipient;};
        void setRecipientStreet (const QString street) {mStreet = street;};
        void setRecipientStreetNumber (const int number) {mNumber = number;};
        void setRecipientAreaCode (const int areaCode) {mAreaCode = areaCode;};
        void setRecipientCity (const QString city) {mCity = city;};
        void setSignatureName (const QString name) {mSigName = name;};
        void setSignature (const QImage img) {mSigImg = img;};
        void setAnnotation (const QString annotation) {mAnnotation = annotation;};

        QString getDateHandover () {return mDateHandover;};
        QString getDateDeadline () {return mDateDeadline;};
        QString getRecipient () {return mRecipient;};
        QString getRecipientStreet () {return mStreet;};
        int getRecipientStreetNumber () {return mNumber;};
        int getRecipientAreaCode () {return mAreaCode;};
        QString getRecipientCity () {return mCity;};
        QString getSignatureName () {return mSigName;};
        QImage getSignature () {return mSigImg;};
        QString getAnnotation () {return mAnnotation;};

    private:
        QString mDateHandover;
        QString mDateDeadline;
        QString mRecipient;
        QString mStreet;
        int mNumber;
        int mAreaCode;
        QString mCity;
        QString mSigName;
        QImage mSigImg;
        QString mAnnotation;
};

#endif // DATAOBJECTHANDOVER_H

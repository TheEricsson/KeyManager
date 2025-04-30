#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

class ViewDataAnnotation;
class ViewDataHandover;
class ViewDataKeychain;
class ViewDataRecipient;
class ViewDataReturnDate;
class ViewDataScanner;
class DatabaseImpl;
class QSqlRelationalTableModel;

#include <QImage>
#include "globals.h"

class DataInterface
{  
    public:
        DataInterface();
        ~DataInterface();

        DataInterface* getHandle (){return this;};
        Database::KeychainStatus getKeychainStatusId ();
        Database::KeychainStatus getNewKeychainStatusId ();
        int getKeychainAddressId ();
        int getInternalLocation ();
        bool setKeychainInternalLocation (const int location);
        bool setKeychainAddressId (const int addressId);
        const QString getHandoverDate ();
        const QString getDeadlineDate ();
        const QString getRecipientName ();
        const QString getRecipientStreet ();
        const QString getRecipientStreetNumber ();
        int getRecipientAreaCode ();
        const QString getRecipientCity ();
        const QString getRecipientSigName ();
        const QImage getRecipientSignature ();
        const QString getRecipientAnnotation ();
        bool setKeychainStatusId (Database::KeychainStatus status);
        bool setNewKeychainStatusId (Database::KeychainStatus status);

        bool setRecipientName(const QString& name);
        bool setRecipientType(const QString& type);
        bool setRecipientStreet(const QString& street);
        bool setRecipientStreetNumber(const QString& streetNr);
        bool setRecipientAreaCode(const int areaCode);
        bool setRecipientCity(const QString& city);
        bool setRecipientSigName (const QString &name);
        bool setRecipientSigImg (QImage sigImg);
        bool setRecipientCurrentDate (const QString& date);
        bool setRecipientDeadlineDate (const QString& date);

        bool setAnnotationText(const QString& text);
        bool setScannedCode (const int keyCode);

        void resetKeychainData ();
        void resetRecipientData ();
        void resetAnnotationData ();
        void resetHandoverData ();
        void resetReturnDateData ();
        void resetScannerData ();

        const QImage getKeychainImg ();
        int getScannedCode ();

        ViewDataKeychain* getDataKeychain () {return mDataKeychain;};

        void setData (ViewDataAnnotation* data);
        void setData (ViewDataHandover* data);
        void setData (ViewDataKeychain* data);
        void setData (ViewDataRecipient* data);
        void setData (ViewDataReturnDate* data);
        void setData (ViewDataScanner* data);

    private:
        ViewDataHandover* getDataHandover () {return mDataHandover;};
        ViewDataRecipient* getDataRecipient () {return mDataRecipient;};
        ViewDataReturnDate* getDataReturnDate () {return mDataReturnDate;};
        ViewDataScanner* getDataScanner () {return mDataScanner;};

        ViewDataAnnotation *mDataAnnotation;
        ViewDataHandover *mDataHandover;
        ViewDataKeychain *mDataKeychain;
        ViewDataRecipient *mDataRecipient;
        ViewDataReturnDate *mDataReturnDate;
        ViewDataScanner *mDataScanner;
};

#endif // DATAINTERFACE_H

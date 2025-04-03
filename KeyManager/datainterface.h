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
        const QString getHandoverDate ();
        const QString getDeadlineDate ();
        const QString getRecipientName ();
        const QString getRecipientStreet ();
        int getRecipientStreetNumber ();
        int getRecipientAreaCode ();
        const QString getRecipientCity ();
        const QString getRecipientSigName ();
        const QImage getRecipientSignature ();
        const QString getRecipientAnnotation ();
        bool setKeychainStatusId (Database::KeychainStatus status);
        bool setNewKeychainStatusId (Database::KeychainStatus status);
        bool setRecipientSigImg (QImage sigImg);
        bool setDeadlineDate (const QString& date);

        bool resetKeychainData ();
        bool resetRecipientData ();
        const QString getKeychainImgPath ();
        int getScannedCode ();

        void setData (ViewDataAnnotation* data);
        void setData (ViewDataHandover* data);
        void setData (ViewDataKeychain* data);
        void setData (ViewDataRecipient* data);
        void setData (ViewDataReturnDate* data);
        void setData (ViewDataScanner* data);

    private:
        ViewDataHandover* getDataHandover () {return mDataHandover;};
        ViewDataKeychain* getDataKeychain () {return mDataKeychain;};
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

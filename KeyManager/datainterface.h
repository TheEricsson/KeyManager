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

        bool initKeyOverviewModel (QSqlRelationalTableModel *model, const QString& filter);
        bool initKeychainModel (QSqlRelationalTableModel *model, const QString& filter);
        bool initRecipientModel (QSqlRelationalTableModel *model);
        bool findKeycode (int keyCode);
        Database::KeychainStatus getKeychainStatusId ();
        Database::KeychainStatus getNewKeychainStatusId ();
        int getInternalLocation ();
        const QString getKeychainStatusText (int statusId);
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
        bool submitHandover ();

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

        DatabaseImpl *mDatabase;
        ViewDataAnnotation *mDataAnnotation;
        ViewDataHandover *mDataHandover;
        ViewDataKeychain *mDataKeychain;
        ViewDataRecipient *mDataRecipient;
        ViewDataReturnDate *mDataReturnDate;
        ViewDataScanner *mDataScanner;
};

#endif // DATAINTERFACE_H

#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

class ViewDataHandover;
class ViewDataKeychainStatus;
class ViewDataRecipient;
class ViewDataReturnDate;
class ViewDataScanner;
class DatabaseImpl;
class QSqlRelationalTableModel;
#include "globals.h"

class DataInterface
{  
    public:
        DataInterface();
        ~DataInterface();

        bool initKeyOverviewModel (QSqlRelationalTableModel *model, int keyCode);
        bool initKeychainModel (QSqlRelationalTableModel *model, int aId);
        bool initRecipientModel (QSqlRelationalTableModel *model);
        Database::eKeychainStatusId getKeychainStatusId (int aId);

        void setData (ViewDataHandover* data);
        void setData (ViewDataKeychainStatus* data);
        void setData (ViewDataRecipient* data);
        void setData (ViewDataReturnDate* data);
        void setData (ViewDataScanner* data);
        void deleteData ();

        ViewDataHandover* getDataHandover () {return mDataHandover;};
        ViewDataKeychainStatus* getDataKeychainStatus () {return mDataKeychainStatus;};
        ViewDataRecipient* getDataRecipient () {return mDataRecipient;};
        ViewDataReturnDate* getDataReturnDate () {return mDataReturnDate;};
        ViewDataScanner* getDataScanner () {return mDataScanner;};

    private:
        DatabaseImpl *mDatabase;
        ViewDataHandover *mDataHandover;
        ViewDataKeychainStatus *mDataKeychainStatus;
        ViewDataRecipient *mDataRecipient;
        ViewDataReturnDate *mDataReturnDate;
        ViewDataScanner *mDataScanner;
};

#endif // DATAINTERFACE_H

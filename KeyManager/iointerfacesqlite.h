#ifndef IOINTERFACESQLITE_H
#define IOINTERFACESQLITE_H

#include "iointerface.h"
#include <QSqlDatabase>
#include <QList>
#include <QImage>

class DataObject;

class IOInterfaceSQLITE : public IOInterface
{
    public:

        IOInterfaceSQLITE();
        ~IOInterfaceSQLITE();
        bool addKey (const IOInterface::keyData *data);
        bool findKeyCode(int aCode);
        Database::KeychainStatus getKeychainStatusId (const int& keyCode);
        const QString getKeychainStatusText (int statusId);
        int getKeychainInternalLocation (const int& keyCode);
        int getKeychainAddressId (const int& keyCode);
        bool setKeychainData (ViewDataKeychain* data, const int& keyCode);
        int getNumberOfEntries (const QString &tableName);
        QVariant getValue (const QString &tableName, const QString& columnName, int index);
        bool initKeyOverviewModel (QSqlRelationalTableModel *model, const QString &filter);
        bool initKeychainModel (QSqlRelationalTableModel *model, const QString &filter);
        bool initRecipientModel (QSqlRelationalTableModel *model);
        bool initBuildingModel (QSqlRelationalTableModel *model);
        bool addNewRecipient (const IOInterface::recipientData *data);
        const QString getKeychainImgPath (const int& keyCode);
        bool dbInsertHandover (DataInterface *data);
        bool dbInsertKeychain (DataInterface *data);
        bool dbCleanupTable (const QString& tablename, const QString& filter, const int numberOfEntriesToKeep);
        int getFreeInternalLocation ();
        int findInternalLocation (const int internalLoc);

    private:
#ifdef Q_OS_ANDROID
        bool checkPermission ();
#endif
        bool firstStart ();

        QSqlDatabase mDb;

        int mAdressId;
        int mKeychainId;
        int mKeychainInternalLocation;
        QImage mKeychainImg;
        QString mKeychainStatus;
        Database::KeychainStatus mKeychainStatusId;
};

#endif // IOINTERFACESQLITE_H

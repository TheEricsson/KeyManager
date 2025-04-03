#ifndef IOINTERFACESQLITE_H
#define IOINTERFACESQLITE_H

#include "iointerface.h"
#include <QSqlDatabase>
#include <QList>
#include <QImage>

class DataObject;

class IOInterfaceSQLITE : public IOInterface
{
        struct mKey
        {
            int dbId;
            int keychainId;
            QString street;
            QString streetNumber;
            int areaCode;
            QString category;
            QString status;
            QString description;
        };

    public:

        IOInterfaceSQLITE();
        ~IOInterfaceSQLITE();
        bool findKeyCode(int aCode);
        Database::KeychainStatus getKeychainStatusId (const int& keyCode);
        const QString getKeychainStatusText (int statusId);
        int getKeychainInternalLocation (const int& keyCode);
        int getKeychainAddressId (const int& keyCode);
        bool setKeychainData (ViewDataKeychain* data, const int& keyCode);
        bool initKeyOverviewModel (QSqlRelationalTableModel *model, const QString &filter);
        bool initKeychainModel (QSqlRelationalTableModel *model, const QString &filter);
        bool initRecipientModel (QSqlRelationalTableModel *model);
        bool initBuildingModel (QSqlRelationalTableModel *model);
        bool addNewRecipient(const Database::RecipientType& type, const QString& name, const QString& street, const QString& number, const QString& areaCode, const QString& city);
        bool addNewRecipient (const RecipientData& data);
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
        QList <mKey> mKeychainItems;
        Database::KeychainStatus mKeychainStatusId;
};

#endif // IOINTERFACESQLITE_H

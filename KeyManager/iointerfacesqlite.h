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
        int countDbTables();
        bool initFirstStart();
        bool addKey (const IOInterface::keyData *data);
        bool findKeyCode(unsigned int code);
        KeychainStatus::Value getKeychainStatusId (const int& keyCode);
        const QString getKeychainStatusText (int statusId);
        int getKeychainInternalLocation (const int& keyCode);
        int getKeychainAddressId (const int& keyCode);
        const QString getAddressStreet (const int& addressId);
        const QString getAddressStreetNumber (const int& addressId);
        int getAddressAreaCode (const int& addressId);
        const QString getAddressCity (const int& addressId);
        bool setKeychainData (ViewDataKeychain* data, const int& keyCode);
        int getNumberOfEntries (const QString &tableName);
        QVariant getValue (const QString &tableName, const QString& columnName, int index);
        QList<QVariant> getTableColumn (const QString &tableName, const QString& columnName);
        bool initKeyOverviewModel (QSqlRelationalTableModel *model, const QString &filter);
        bool initKeychainModel (QSqlRelationalTableModel *model, const QString &filter);
        bool initKeychainHistoryModel (QSqlRelationalTableModel *model, const QString &filter = "");
        bool initRecipientModel (QSqlRelationalTableModel *model);
        bool initCustomerModel (QSqlRelationalTableModel *model);
        bool initSearchViewModel (QSqlRelationalTableModel *model, const QString &filter = "");
        bool addNewRecipient (const IOInterface::recipientData *data);
        bool addNewCustomer (const IOInterface::customerData *data);
        bool getKeychainImg (const int aId, QImage& img);
        bool dbInsertHandover (DataInterface *data);
        unsigned int getLastHandoverId();
        bool dbInsertKeychain (DataInterface *data);
        bool dbInsertKeychainImg (const unsigned int keyCode, const QImage& img);
        bool dbCleanupTable (const QString& tablename, const QString& filter, const int numberOfEntriesToKeep);
        unsigned int getFreeKeycode (const unsigned int lockerId);
        unsigned int getFreeInternalLocation (const unsigned int lockerId);
        unsigned int getKeycodeFromInternalLocation (const unsigned int lockerId, const unsigned int internalLoc);
        const QString dbGetLastError();
        void byteArrayToImage (QByteArray imgBa, QImage& img);

    private:
#ifdef Q_OS_ANDROID
        bool checkPermission ();
#endif
        bool initTables ();
        bool initDefaultValues();
        QByteArray imageToByteArray (QImage img);

        QSqlDatabase mDb;

        int mAdressId;
        int mKeychainId;
        int mKeychainInternalLocation;
        QImage mKeychainImg;
        QString mKeychainStatus;
        KeychainStatus::Value mKeychainStatusId;
};

#endif // IOINTERFACESQLITE_H

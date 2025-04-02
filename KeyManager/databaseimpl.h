#ifndef DATABASEIMPL_H
#define DATABASEIMPL_H

#include <QSqlDatabase>
#include <QImage>
#include "globals.h"

class QSqlQueryModel;
class QSqlRelationalTableModel;
class DataObject;
class DataObjectHandover;
class ViewDataScanner;
class ViewDataKeychain;
class DataInterface;

class DatabaseImpl
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

    DatabaseImpl();

    //keychain data
    Database::KeychainStatus getKeychainStatusId (const int& keyCode);
    const QString getKeychainStatusText (int statusId);
    int getKeychainInternalLocation (const int& keyCode);
    int getKeychainAddressId (const int& keyCode);
    const QString getKeychainImgPath (const int& keyCode);
    bool setKeychainData (ViewDataKeychain* data, const int& keyCode);

    bool setKeychainStatusId (const int& keyCode, int status);
    bool setKeychainInternalLocation (const int& keyCode, int location);
    bool setKeychainAddressId (const int& keyCode, int addressId);
    bool setKeychainImgPath (const int& keyCode, const QString& imgPath);
    //bool addKeychain (KeychainData *data);

    //key categories
    QString getKeyCategory (const int& catId);
    bool addKeyCategory (const QString& catName);

    //keyaddresses
    const QString& getStreetName (const int& addressId);
    const QString& getStreetNumber (const int& addressId);
    const int& getAreaCode (const int& addressId);
    const QString& getCityName (const int& addressId);
    //bool addAddress (AddressData *data);

    //keys
    int getKeychainId (const int& keyId);
    int getCategoryId (const int& keyId);
    int getStatusId (const int& keyId);
    const QString& getDescription (const int& keyId);

    //key status
    const QString& getKeyStatus (const int& id);
    bool addKeyStatus (const QString& keyStatus);

    //recipient addresses
    const QString& getRecipientName (const int& recipientId);
    const int& getRecipientTypeId (const int& recipientId);
    const QString& getRecipientStreet (const int& recipientId);
    const QString& getRecipientStreetNumber (const int& recipientId);
    const int& getRecipientAreaCode (const int& recipientId);
    const QString& getRecipientCity (const int& recipientId);

    bool setRecipientName (const int& recipientId, const QString& name);
    bool setRecipientType (const int& recipientId, const int& type);
    bool setRecipientStreet (const int& recipientId, const QString& street);
    bool setRecipientStreetNumber (const int& recipientId, const QString& streetNumber);
    bool setRecipientAreaCode (const int& recipientId, const int& areaCode);
    bool setRecipientCity (const int& recipientId, const QString& city);

    // recipient types
    const QString& getRecipientType (const int& recipientTypeId);
    bool addRecipientType (const QString& recipientType);

    bool findKeyCode(int aCode);
    bool setKeyCode(int aCode);
    bool initKeyOverviewModel (QSqlRelationalTableModel *model, const QString &filter);
    bool initKeychainModel (QSqlRelationalTableModel *model, const QString &filter);
    bool initRecipientModel (QSqlRelationalTableModel *model);
    bool addNewRecipient(const Database::RecipientType &type, const QString& name, const QString& street, const QString& number, const QString& areaCode, const QString& city);
    bool addNewRecipient (const RecipientData& data);
    bool dataUpdate (DataObject *data);
    bool update (ViewDataScanner *data);
    const QString getKeychainImagePath (int aId);
    bool dbInsertHandover (DataInterface *data);
    bool dbCleanupTable (const QString& tablename, const int numberOfEntriesToKeep);
    //bool initializeKeyOverviewModel (QSqlQueryModel *model, int aCode);
    //const QSqlQueryModel& getKeysModel (int aId);

private:
    bool firstStart ();
    bool dataUpdate (DataObjectHandover *data);

#ifdef Q_OS_ANDROID
    bool checkPermission();
#endif
    QSqlDatabase mDb;

    int mAdressId;
    int mKeychainId;
    int mKeychainInternalLocation;
    QImage mKeychainImg;
    QString mKeychainStatus;
    QList <mKey> mKeychainItems;
    Database::KeychainStatus mKeychainStatusId;
};

#endif // DATABASEIMPL_H

#include "databaseimpl.h"
#include <QSettings>
#include <QSqlQuery>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QImage>

DatabaseImpl::DatabaseImpl()
{
    // QSettings settings (QString("res/config.ini"), QSettings::IniFormat);
    // QString dbType = settings.value("database/dbType", "error").toString();
    // QString dbHostName = settings.value("database/dbHostName", "error").toString();
    // QString dbDatabaseName = settings.value("database/dbDatabaseName", "error").toString();
    // QString dbUserName = settings.value("database/dbUserName", "error").toString();
    // QString dbPassword = settings.value("database/dbPassword", "error").toString();

    // qDebug () << "opening database -> parameters:" << dbType;
    // qDebug () << "dbType" << dbType;
    // qDebug () << "dbHostName" << dbType;
    // qDebug () << "dbDatabaseName" << dbType;
    // qDebug () << "dbUserName" << dbType;
    // qDebug () << "dbPassword" << dbType;

    // QSqlDatabase db = QSqlDatabase::addDatabase(dbType);
    // db.setHostName(dbHostName);
    // db.setDatabaseName(dbDatabaseName);
    // db.setUserName(dbUserName);
    // db.setPassword(dbPassword);

    QSqlDatabase mDb = QSqlDatabase::addDatabase("QSQLITE");

    // todo: location is hardcoded by now..
#ifdef Q_OS_ANDROID
    QString dbLocation = "/storage/emulated/0/Android/data/org.qtproject.example.KeyManager/files/db.sqlite";
#endif
#ifdef Q_OS_WIN64
    QString dbLocation = "db.sqlite";
#endif

    qDebug () << "dbLocation: " << dbLocation;

    // todo: path is wrong ->
    // const auto standardPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // const auto dbName = standardPath + "/db.sqlite";

    mDb.setDatabaseName(dbLocation);

    bool ok = mDb.open ();

    if(!ok)
    {
        qDebug () << "AppDataLocation + filename = " << dbLocation;
        qDebug () << "db.open () returned: " << ok;
        qDebug () << mDb.isOpen();
        qDebug () << mDb.isOpenError();

        qDebug () << "mDb.databaseName(): " << mDb.databaseName();
        qDebug () << "mDb.connectionName(): " << mDb.connectionName();
    }

    QStringList tables = mDb.tables();

    qDebug () << "number of tables: " << tables.count();

    for (int i=0;i<tables.count();i++)
    {
        qDebug () << "table: " << tables [i];
    }
}

bool DatabaseImpl::findKeyCode(int aClientId, int aKeyId)
{
    mDb.transaction();

    QSqlQuery query;
    query.prepare("SELECT id FROM keychains WHERE barcodeLocale = ? AND barcodeKeychainId = ?");
    query.bindValue(0, aClientId);
    query.bindValue(1, aKeyId);
    query.exec();

    if (query.next())
    {
        qDebug () << "found the Barcode!";
        return true;
    }

    qDebug () << "DatabaseImpl::findBarcode: Barcode not found: " << "Client: " << aClientId << ", Key: " << aKeyId;
    return false;
}

bool DatabaseImpl::setKeyCode (int aClientId, int aKeyId)
{
    qDebug () << "DatabaseImpl::setKeyCode (int aClientId, int aKeyId)";

    mDb.transaction();

    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("SELECT id, keychainStatusId, internalLocation, image FROM keychains WHERE barcodeLocale = ? AND barcodeKeychainId = ?");
    query.bindValue(0, aClientId);
    query.bindValue(1, aKeyId);
    query.exec();

    if (query.next())
    {
        qDebug () << "ok";
        mKeychainId = query.value(0).toInt();
        mKeychainInternalLocation = query.value(2).toInt();
        mKeychainImg = QImage::fromData(query.value(3).toByteArray(), "jpg");

        int keychainStatusId = query.value(1).toInt();
        query.prepare("SELECT status FROM keychainStates WHERE id = ?");
        query.bindValue(0, keychainStatusId);
        query.exec();

        // set keychain status
        if (query.next())
        {
            qDebug () << "ok";
            mKeychainStatus = query.value(0).toString();
        }
        else
        {
            return false;
        }

        // get all keys of the keychain
        query.prepare("SELECT id, addressId, categoryId, statusId, description FROM keys WHERE keychainId = :ref_id");
        query.bindValue(":ref_id", mKeychainId);
        query.exec();

        int count = 0;

        mKeychainItems.clear();

        while (query.next()) // todo: always returns only 1 value
        {
            count++;
            qDebug () << "ok: " << count;

            int keyId = query.value(0).toInt();
            int addressId = query.value(1).toInt();
            int categoryId = query.value(2).toInt();
            int statusId = query.value(3).toInt();
            QString description = query.value(4).toString();

            QSqlQuery queryKeyContents;

            //get address from id
            queryKeyContents.prepare("SELECT street, streetNumber, areaCode, city FROM keyAddresses WHERE id = ?");
            queryKeyContents.bindValue(0, addressId);
            queryKeyContents.exec();

            QString street = "";
            QString streetNumber = "";
            int areaCode = 0;
            QString city = "";

            if (queryKeyContents.next())
            {
                qDebug () << "okA";
                street = queryKeyContents.value(0).toString();
                streetNumber = queryKeyContents.value(1).toString();
                areaCode = queryKeyContents.value(2).toInt();
                city = queryKeyContents.value(3).toString();
            }
            else
                return false;

            // get category
            queryKeyContents.prepare("SELECT category FROM keyCategories WHERE id = ?");
            queryKeyContents.bindValue(0, categoryId);
            queryKeyContents.exec();

            QString category ("");

            if (queryKeyContents.next())
            {
                qDebug () << "okB";
                category = queryKeyContents.value(0).toString();
            }
            else
                return false;

            // get status
            queryKeyContents.prepare("SELECT status FROM keyStates WHERE id = ?");
            queryKeyContents.bindValue(0, statusId);
            queryKeyContents.exec();

            QString status ("");

            if (queryKeyContents.next())
            {
                qDebug () << "okC";
                status = queryKeyContents.value(0).toString();
            }
            else
                return false;

            mKey key;
            key.street = street;
            key.streetNumber = streetNumber;
            key.areaCode = areaCode;
            key.category = category;
            key.dbId = keyId;
            key.description = description;
            key.keychainId = mKeychainId;
            key.status = status;

            mKeychainItems.append(key);
        }
    }
    else
        return false;
    {
        for (int i = 0; i < mKeychainItems.count(); i++)
        {
            qDebug () << "mKeychainItems[i].street: " << mKeychainItems[i].street;
            qDebug () << "mKeychainItems[i].streetNumber: " << mKeychainItems[i].streetNumber;
            qDebug () << "mKeychainItems[i].areaCode: " << mKeychainItems[i].areaCode;
            qDebug () << "mKeychainItems[i].category: " << mKeychainItems[i].category;
            qDebug () << "mKeychainItems[i].dbId: " << mKeychainItems[i].dbId;
            qDebug () << "mKeychainItems[i].description: " << mKeychainItems[i].description;
            qDebug () << "mKeychainItems[i].keychainId: " << mKeychainItems[i].keychainId;
            qDebug () << "mKeychainItems[i].status: " << mKeychainItems[i].status;

        }
    }
    return true;
}

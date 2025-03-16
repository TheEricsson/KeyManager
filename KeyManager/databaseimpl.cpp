#include "databaseimpl.h"
#include <QSettings>
#include <QSqlQuery>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QImage>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include <QMessageBox>

#ifdef Q_OS_ANDROID
    #include <QCoreApplication>
    #include <QtCore/private/qandroidextras_p.h>
#endif

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

    qDebug () << "paths:";
    qDebug () << "QStandardPaths::AppConfigLocation" << QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    qDebug () << "QStandardPaths::AppDataLocation" << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    qDebug () << "QStandardPaths::AppLocalDataLocation" << QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    qDebug () << "QStandardPaths::ApplicationsLocation" << QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
    qDebug () << "QStandardPaths::RuntimeLocation" << QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation);
    qDebug () << "QStandardPaths::DocumentsLocation" << QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    mKeychainStatusId = -1;

    QSqlDatabase mDb = QSqlDatabase::addDatabase("QSQLITE");

    // todo: location is hardcoded by now..
#ifdef Q_OS_ANDROID
    //QString dbLocation = "/storage/emulated/0/Android/data/org.qtproject.example.KeyManager/files/db.sqlite";

    //QString dbLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString dbLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    dbLocation.append("/db.sqlite");

    if (!checkPermission())
        qDebug () << "DatabaseImpl::checkPermissions failed!";

#endif
#ifdef Q_OS_WIN64
    QString dbLocation = "db.sqlite";
#endif

    qDebug () << "dbLocation: " << dbLocation;

    mDb.setDatabaseName(dbLocation);

    bool ok = mDb.open ();

    if(!ok)
    {
        QMessageBox::critical(0, "Cannot open database",
                              "Unable to establish a database connection.\n"
                                 "This example needs SQLite support. Please read "
                                 "the Qt SQL driver documentation for information how "
                                 "to build it.", QMessageBox::Cancel);

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

    // database is empty -> create
    if (0 == tables.count())
    {
        QSqlQuery query;

        query.exec ("CREATE TABLE keyAddresses ( \
                   id           INTEGER PRIMARY KEY AUTOINCREMENT \
                   UNIQUE, \
                   street       TEXT, \
                   streetNumber TEXT, \
                   areaCode     INTEGER, \
                   city         TEXT)");

        // query.exec ("CREATE TABLE handovers ( \
        //     id            INTEGER PRIMARY KEY AUTOINCREMENT \
        //         UNIQUE, \
        //     keychainId    INTEGER REFERENCES keychains (barcode), \
        //     dateHandover  TEXT, \
        //     dateDeadline  TEXT, \
        //     recipientId   INTEGER REFERENCES recipientAddresses (id), \
        //     signatureName TEXT, \
        //     signature     BLOB)");
    }
}

#ifdef Q_OS_ANDROID
bool DatabaseImpl::checkPermission()
{
    QList<bool> permissions;

    auto r = QtAndroidPrivate::checkPermission("android.permission.READ_EXTERNAL_STORAGE").result();
    if (r != QtAndroidPrivate::Authorized)
    {
        r = QtAndroidPrivate::requestPermission("android.permission.READ_EXTERNAL_STORAGE").result();
        if (r == QtAndroidPrivate::Denied)
            permissions.append(false);
    }
    r = QtAndroidPrivate::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE").result();
    if (r != QtAndroidPrivate::Authorized)
    {
        r = QtAndroidPrivate::requestPermission("android.permission.WRITE_EXTERNAL_STORAGE").result();
        if (r == QtAndroidPrivate::Denied)
            permissions.append(false);
    }
    r = QtAndroidPrivate::checkPermission("android.permission.READ_MEDIA_IMAGES").result();
    if (r != QtAndroidPrivate::Authorized)
    {
        r = QtAndroidPrivate::requestPermission("android.permission.READ_MEDIA_IMAGES").result();
        if (r == QtAndroidPrivate::Denied)
            permissions.append(false);
    }

    for (int i = 0; i < permissions.count(); i++)
    {
        qDebug () << "permissions[i]" << permissions.at(i);
    }
    return (permissions.count() != 3);
}
#endif

bool DatabaseImpl::findKeyCode(int aCode)
{
    mDb.transaction();

    QSqlQuery query;
    query.prepare("SELECT barcode FROM keychains WHERE barcode = ?");
    query.bindValue(0, aCode);
    query.exec();

    if (query.next())
    {
        qDebug () << "found the Barcode!";
        return true;
    }
    return false;
}

bool DatabaseImpl::setKeyCode (int aCode)
{
    qDebug () << "DatabaseImpl::setKeyCode";

    mDb.transaction();

    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("SELECT barcode, keychainStatusId, addressId, internalLocation, image FROM keychains WHERE barcode = ?");
    query.bindValue(0, aCode);
    query.exec();

    if (query.next())
    {
        qDebug () << "ok";
        mKeychainId = query.value(0).toInt();
        qDebug () << "mKeychainId: " << mKeychainId;
        mKeychainInternalLocation = query.value(3).toInt();
        mKeychainImg = QImage::fromData(query.value(4).toByteArray(), "jpg");
        int addressId = query.value(2).toInt();

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
        query.prepare("SELECT id, categoryId, statusId, description FROM keys WHERE keychainId = ?");
        query.bindValue(0, mKeychainId);
        query.exec();

        int count = 0;

        mKeychainItems.clear();

        while (query.next()) // todo: always returns only 1 value
        {
            count++;
            qDebug () << "ok: " << count;

            int keyId = query.value(0).toInt();
            int categoryId = query.value(1).toInt();
            int statusId = query.value(2).toInt();
            QString description = query.value(3).toString();

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

int DatabaseImpl::getKeychainStatusId (int aId)
{
    qDebug () << "DatabaseImpl::getKeychainStatusId (int aId): " << aId;

    mDb.transaction();

    QSqlQuery query;
    query.setForwardOnly(true);

    // get all keys of the keychain
    query.prepare("SELECT keychainStatusId FROM keychains WHERE barcode = ?");
    query.bindValue(0, aId);
    query.exec();

    // set keychain status
    if (query.next())
    {
        mKeychainStatusId = query.value(0).toInt();
    }
    qDebug () << "mKeychainStatusId: " << mKeychainStatusId;
    return mKeychainStatusId;
}

bool DatabaseImpl::initKeyOverviewModel (QSqlRelationalTableModel *model, int aId)
{
    if (model)
    {
        model->setTable("keys");
        model->setRelation(2, QSqlRelation ("keyCategories", "id", "category"));
        model->setRelation(3, QSqlRelation("keyStates", "id", "status"));
        model->setFilter(QString::number(aId));
        model->select();
        return true;
    }
    else return false;
}

bool DatabaseImpl::initKeychainModel (QSqlRelationalTableModel *model, int aId)
{
    if (model)
    {
        model->setTable("keychains");
        model->setRelation(1, QSqlRelation ("keychainStates", "id", "status"));
        model->setRelation(3, QSqlRelation("keyAddresses", "id", "street, streetNumber, areaCode, city"));
        model->setFilter(QString::number(aId));
        model->select();
        return true;
    }
    else return false;
}

bool DatabaseImpl::initRecipientModel (QSqlRelationalTableModel *model)
{
    if (model)
    {
        model->setTable("recipientAddresses");
        model->setRelation(1, QSqlRelation ("recipientTypes", "id", "type"));
        model->select();
        return true;
    }
    else
        return false;
}

bool DatabaseImpl::addNewRecipient(const RecipientType& type, const QString& name, const QString& street, const QString& number, const QString& areaCode, const QString& city)
{
    mDb.transaction();

    QSqlQuery query;
    query.prepare("INSERT INTO recipientAddresses (type, name, street, houseNr, areaCode, city) \
                    VALUES (?, ?, ?, ?, ?, ?)");
    query.bindValue(0, type);
    query.bindValue(2, name);
    query.bindValue(3, street);
    query.bindValue(4, number);
    query.bindValue(5, areaCode);
    query.bindValue(6, city);

    return query.exec();
}

bool DatabaseImpl::addNewRecipient (const RecipientData& data)
{
    mDb.transaction();

    QSqlQuery query;
    query.prepare("INSERT INTO recipientAddresses (type, name, street, houseNr, areaCode, city) \
                    VALUES (?, ?, ?, ?, ?, ?)");
    query.bindValue(0, data.type);
    query.bindValue(1, data.name);
    query.bindValue(2, data.street);
    query.bindValue(3, data.number);
    query.bindValue(4, data.areaCode);
    query.bindValue(5, data.city);

    return query.exec();
}

// bool DatabaseImpl::initializeKeyOverviewModel (QSqlQueryModel *model, int aCode)
// {
//     qDebug () << "initializeKeyOverviewModel (QSqlQueryModel *model, int aClientId, int aKeyId)";

//     if (model)
//     {
//         qDebug () << "ok";

//         if (setKeyCode (aCode))
//         {
//             qDebug () << "blabla";

//             mDb.transaction();

//             QSqlQuery query;
//             query.prepare("SELECT * FROM keys WHERE keychainId = ? INNER JOIN keyCategories ON keys.categoryId = keyCategories.category");
//             query.bindValue(0, aCode);

//             //model->setQuery("SELECT * FROM keys WHERE keychainId = 1");
//             model->setQuery("SELECT category, status, description \
//                             FROM keys \
//                             INNER JOIN keyCategories ON keyCategories.id = keys.categoryId \
//                             INNER JOIN keyStates ON keyStates.id = keys.statusId \
//                             WHERE keychainId = 1");

//             return true;
//         }
//     }
//     return false;
// }

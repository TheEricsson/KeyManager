#include "iointerfacesqlite.h"
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
#include <QBuffer>
//#include "dataobjecthandover.h"
//#include "viewdatascanner.h"
#include "viewdatakeychain.h"
#include "datainterface.h"
#include "globals.h"
#include "viewdatarecipient.h"

#ifdef Q_OS_ANDROID
    #include <QCoreApplication>
    #include <QtCore/private/qandroidextras_p.h>
#endif

IOInterfaceSQLITE::IOInterfaceSQLITE()
{
    qDebug () << "paths:";
    qDebug () << "QStandardPaths::AppConfigLocation" << QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    qDebug () << "QStandardPaths::AppDataLocation" << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    qDebug () << "QStandardPaths::AppLocalDataLocation" << QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    qDebug () << "QStandardPaths::ApplicationsLocation" << QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
    qDebug () << "QStandardPaths::RuntimeLocation" << QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation);
    qDebug () << "QStandardPaths::DocumentsLocation" << QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    qDebug () << "QStandardPaths::GenericDataLocation" << QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    qDebug () << "QStandardPaths::HomeLocation" << QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    qDebug () << "QStandardPaths::RuntimeLocation" << QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation);

    mKeychainStatusId = KeychainStatus::Undefined;
    mDb = QSqlDatabase::addDatabase("QSQLITE");

#ifdef Q_OS_ANDROID
    mDbLocation = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    mDbLocation.append("/db.sqlite");

    qDebug () << "mDbLocation: " << mDbLocation;

    if (!checkPermission())
        qDebug () << "IOInterfaceSQLITE::checkPermissions failed!";

#endif
#ifdef Q_OS_WIN64
    //mDbLocation = "db.sqlite";
    mDbLocation = "C:/QtProjekte/KeyManager/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/db.sqlite";
#endif

    mDb.setDatabaseName(mDbLocation);

    bool ok = mDb.open ();

    if(!ok)
    {
        QMessageBox msgBox;
        msgBox.setStandardButtons(QMessageBox::Abort);
        msgBox.setText ("Fehler!");
        msgBox.setInformativeText("Datenbank konnte nicht geöffnet werden.");
        msgBox.setDetailedText(mDb.lastError().text());
        msgBox.exec();
    }
}

#ifdef Q_OS_ANDROID
bool IOInterfaceSQLITE::checkPermission()
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

int IOInterfaceSQLITE::countDbTables()
{
    if (!mDb.isValid())
        qDebug () << "db invalid";

    if (!mDb.isOpen())
        qDebug () << "db not opened";

    QStringList tables = mDb.tables();

    qDebug () << "number of tables: " << tables.count();

    for (int i=0;i<tables.count();i++)
    {
        qDebug () << "table: " << tables [i];
    }

    return tables.count();
}

bool IOInterfaceSQLITE::initFirstStart ()
{
    if (!initTables())
        return false;
    if (!initDefaultValues())
        return false;

    return true;
}

const QString IOInterfaceSQLITE::getDatabaseLocation()
{
    qDebug () << "IOInterfaceSQLITE::getDatabaseLocation()" << mDbLocation;
    return mDbLocation;
}

bool IOInterfaceSQLITE::openDatabase()
{
    return mDb.open();
}

bool IOInterfaceSQLITE::closeDatabase()
{
    bool retVal = false;

    if (mDb.isOpen())
    {
        mDb.close();
        retVal = true;
    }

    return retVal;
}

bool IOInterfaceSQLITE::doDbBackup(const QString& fileName)
{
    QString dbLocation = getDatabaseLocation();
    QString backupLocation = dbLocation + "_" + fileName;

    if (QFile::exists(backupLocation))
    {
        QFile::remove(backupLocation);
    }
    qDebug () << "IOInterfaceSQLITE::doDbBackup";
    qDebug () << "dbLocation: " << dbLocation;
    qDebug () << "backupLocation: " << backupLocation;

    return QFile::copy(dbLocation, backupLocation);
}

bool IOInterfaceSQLITE::initTables()
{
    // add all tables for first start and fill with default data
    QSqlQuery query;  

    bool retVal = false;

    retVal = query.exec ("CREATE TABLE recipientTypes (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, type TEXT UNIQUE, dbStatus INTEGER DEFAULT (0))");
    query.finish();

    if(!retVal)
        return false;

    retVal = query.exec ("CREATE TABLE recipientAddresses (\
                        id       INTEGER PRIMARY KEY AUTOINCREMENT\
                        UNIQUE,\
                        name     TEXT,\
                        type     INTEGER,\
                        street   TEXT,\
                        houseNr  TEXT,\
                        areaCode INTEGER,\
                        city     TEXT, \
                        dbStatus INTEGER DEFAULT (0) )");
    query.finish();

    if(!retVal)
        return false;

    retVal = query.exec ("CREATE TABLE keyStates (\
                    id     INTEGER PRIMARY KEY AUTOINCREMENT\
                    UNIQUE,\
                    status TEXT, \
                    dbStatus INTEGER DEFAULT (0) )");
    query.finish();

    if(!retVal)
        return false;

    retVal = query.exec ("CREATE TABLE keyCategories (\
                    id       INTEGER PRIMARY KEY AUTOINCREMENT\
                    UNIQUE,\
                    category TEXT, \
                    dbStatus INTEGER DEFAULT (0) )");
    query.finish();

    if(!retVal)
        return false;

    retVal = query.exec ("CREATE TABLE keychainStates (\
                    id     INTEGER PRIMARY KEY AUTOINCREMENT\
                    UNIQUE,\
                    status TEXT, \
                    dbStatus INTEGER DEFAULT (0) )");
    query.finish();

    if(!retVal)
        return false;

    retVal = query.exec ("CREATE TABLE keyAddresses (\
                    id           INTEGER PRIMARY KEY AUTOINCREMENT\
                    UNIQUE,\
                    street       TEXT,\
                    streetNumber TEXT,\
                    areaCode     INTEGER,\
                    city         TEXT, \
                    dbStatus     INTEGER DEFAULT (0) )");
    query.finish();

    if(!retVal)
        return false;

    retVal = query.exec ("CREATE TABLE keychains (\
                    id               INTEGER NOT NULL\
                    PRIMARY KEY\
                    UNIQUE,\
                    keychainStatusId INTEGER REFERENCES keychainStates (id) \
                    NOT NULL,\
                    internalLocation INTEGER,\
                    addressId        INTEGER REFERENCES keyAddresses (id),\
                    image        BLOB,\
                    dbStatus INTEGER DEFAULT (0) )");
    query.finish();

    if(!retVal)
        return false;

    retVal = query.exec ("CREATE TABLE handovers (\
                     id                    INTEGER PRIMARY KEY AUTOINCREMENT\
                     UNIQUE,\
                     keychainId            INTEGER REFERENCES keychains (id),\
                     dateHandover          TEXT,\
                     dateDeadline          TEXT,\
                     duration              TEXT,\
                     recipient             TEXT,\
                     recipientStreet       TEXT,\
                     recipientStreetNumber TEXT,\
                     recipientAreaCode     INTEGER,\
                     recipientCity         TEXT,\
                     signatureName         TEXT,\
                     signature             BLOB,\
                     annotation            TEXT)");
    query.finish();

    if(!retVal)
        return false;

    retVal = query.exec ("CREATE TABLE keys (\
                    id          INTEGER PRIMARY KEY AUTOINCREMENT\
                    UNIQUE\
                    NOT NULL,\
                    keychainId  INTEGER REFERENCES keychains (id),\
                    categoryId  INTEGER REFERENCES keyCategories (id),\
                    statusId    INTEGER REFERENCES keyStates (id),\
                    description TEXT,\
                    dbStatus INTEGER DEFAULT (0) )");
    query.finish();

    retVal = query.exec ("CREATE TABLE appSettings (\
        id                     INTEGER PRIMARY KEY AUTOINCREMENT\
        UNIQUE\
        NOT NULL,\
        cameraId               INTEGER,\
        companyNameFirstLine   TEXT,\
        companyNameSecondLine  TEXT,\
        companyNameThirdLine   TEXT,\
        allowKeyhookDuplicates INTEGER)");
    query.finish();

    return retVal;
}

bool IOInterfaceSQLITE::initDefaultValues()
{
    QSqlQuery query;

    //key categories
    QList <QString> values;
    values.append("Wohnung");
    values.append("Haustür");
    values.append("Briefkasten");
    values.append("Keller");
    values.append("Hof");
    values.append("Technik");
    values.append("Haustür");
    values.append("General");
    values.append("Zentral");
    values.append("E-Raum");
    values.append("Hausmeister");
    values.append("Sonstige");

    for (int i = 0; i < values.count(); i++)
    {
        query.prepare ("INSERT INTO keyCategories (category, dbStatus) VALUES (?, ?)");
        query.bindValue(0, values.at(i));
        query.bindValue(1, 0);
        query.exec();
        query.finish();
    }

     //keychain states
    values.clear();
    values.append("Verfügbar");
    values.append("Temporär ausgegeben");
    values.append("Dauerhaft ausgegeben");
    values.append("Abgabe zum Verwaltungsende");
    values.append("Verloren");

    for (int i = 0; i < values.count(); i++)
    {
        query.prepare ("INSERT INTO keychainStates (status, dbStatus) VALUES (?, ?)");
        query.bindValue(0, values.at(i));
        query.bindValue(1, 0);
        query.exec();
        query.finish();
    }

    //key states
    values.clear();
    values.append("In Ordnung");
    values.append("Passt nicht");
    values.append("Defekt");
    values.append("Verloren");

    for (int i = 0; i < values.count(); i++)
    {
        query.prepare ("INSERT INTO keyStates (status, dbStatus) VALUES (?, ?)");
        query.bindValue(0, values.at(i));
        query.bindValue(1, 0);
        query.exec();
        query.finish();
    }

    //recipient types
    values.clear();
    values.append("Unternehmen");
    values.append("Mitarbeiter");
    values.append("Privatperson");

    for (int i = 0; i < values.count(); i++)
    {
        query.prepare ("INSERT INTO recipientTypes (type, dbStatus) VALUES (?, ?)");
        query.bindValue(0, values.at(i));
        query.bindValue(1, 0);
        query.exec();
        query.finish();
    }

    //app settings
    query.prepare("INSERT INTO appSettings (cameraId, companyNameFirstLine, companyNameSecondLine, companyNameThirdLine, allowKeyhookDuplicates) VALUES (?, ?, ?, ?, ?)");
    query.bindValue(0, -1);
    query.bindValue(1, "Musterfirma Name");
    query.bindValue(2, "Musterfirma Straße");
    query.bindValue(3, "Musterfirma PLZ Ort");
    query.bindValue(4, false);

    query.exec();

    //todo: catch db query failures instead of returning true everytime
    return true;
}

QByteArray IOInterfaceSQLITE::imageToByteArray (QImage img)
{
    QByteArray baImage;
    QBuffer buffer(&baImage);
    buffer.open(QIODevice::WriteOnly);
    QPixmap pixmap(QPixmap::fromImage(img));
    pixmap.save(&buffer, "jpg", 60);

    return baImage;
}

bool IOInterfaceSQLITE::addKey (const IOInterface::keyData *data)
{
    if (!data)
        return false;

    QSqlQuery query;
    query.prepare("INSERT INTO keys (keychainId, categoryId, statusId, description, dbStatus) \
                    VALUES (?, ?, ?, ?, ?)");
    query.bindValue(0, data->keychainId);
    query.bindValue(1, data->categoryId);
    query.bindValue(2, data->statusId);
    query.bindValue(3, data->description);
    query.bindValue(4, 0);

    return query.exec();
}

bool IOInterfaceSQLITE::findKeyCode(unsigned int code)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM keychains WHERE id = ?");
    query.bindValue(0, code);
    query.exec();

    if (query.next())
    {
        qDebug () << "found the Barcode!";
        return true;
    }
    return false;
}

QList<QVariant> IOInterfaceSQLITE::getKeyIdsByKeycode(unsigned int keyCode)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM keys WHERE keychainId = ?");
    query.bindValue(0, keyCode);
    query.setForwardOnly(true);
    query.exec();

    QList<QVariant> tableEntries;

    while (query.next())
    {
        tableEntries << query.value(0).toUInt();
    }

    return tableEntries;
}

unsigned int IOInterfaceSQLITE::getKeyCategoryId(unsigned int keyId)
{
    QSqlQuery query;
    query.prepare("SELECT categoryId FROM keys WHERE id = ?");
    query.bindValue(0, keyId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toUInt();
    }
    else
        return 0;
}

unsigned int IOInterfaceSQLITE::getKeyStatusId(unsigned int keyId)
{
    QSqlQuery query;
    query.prepare("SELECT statusId FROM keys WHERE id = ?");
    query.bindValue(0, keyId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toUInt();
    }
    else
        return 0;
}

QString IOInterfaceSQLITE::getKeyDescription(unsigned int keyId)
{
    QSqlQuery query;
    query.prepare("SELECT description FROM keys WHERE id = ?");
    query.bindValue(0, keyId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }
    else
        return "";
}

const QString IOInterfaceSQLITE::getKeyCategoryString(unsigned int keyCategoryId)
{
    QSqlQuery query;
    query.prepare("SELECT category FROM keyCategories WHERE id = ?");
    query.bindValue(0, keyCategoryId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }
    else
        return "";
}

const QString IOInterfaceSQLITE::getKeyStatusString(unsigned int keyStatusId)
{
    QSqlQuery query;
    query.prepare("SELECT status FROM keyStates WHERE id = ?");
    query.bindValue(0, keyStatusId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }
    else
        return "";
}

//keychain data
KeychainStatus::Value IOInterfaceSQLITE::getKeychainStatusId (const int& keyCode)
{
    QSqlQuery query;
    query.prepare("SELECT keychainStatusId FROM keychains WHERE id = ?");
    query.bindValue(0, keyCode);
    query.exec();

    if (query.next())
    {
        return (KeychainStatus::Value) query.value(0).toInt();
    }
    else
        return KeychainStatus::Undefined;
}

const QString IOInterfaceSQLITE::getKeychainStatusText (int statusId)
{
    QSqlQuery query;
    query.prepare("SELECT status FROM keychainStates WHERE id = ?");
    query.bindValue(0, statusId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }
    else
        return "Fehler: Status unbekannt!";
}

int IOInterfaceSQLITE::getKeychainInternalLocation (const int& keyCode)
{
    QSqlQuery query;
    query.prepare("SELECT internalLocation FROM keychains WHERE id = ?");
    query.bindValue(0, keyCode);
    query.exec();

    if (query.next())
    {
        return query.value(0).toInt();
    }
    else
        return 0;
}

int IOInterfaceSQLITE::getKeychainAddressId (const int& keyCode)
{
    QSqlQuery query;
    query.prepare("SELECT addressId FROM keychains WHERE id = ?");
    query.bindValue(0, keyCode);
    query.exec();

    if (query.next())
    {
        return query.value(0).toInt();
    }
    else
        return _UNDEFINED;
}

const QString IOInterfaceSQLITE::getAddressStreet (const int& addressId)
{
    QSqlQuery query;
    query.prepare("SELECT street FROM keyAddresses WHERE id = ?");
    query.bindValue(0, addressId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }
    else
        return "";
}

const QString IOInterfaceSQLITE::getAddressStreetNumber (const int& addressId)
{
    QSqlQuery query;
    query.prepare("SELECT streetNumber FROM keyAddresses WHERE id = ?");
    query.bindValue(0, addressId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }
    else
        return "";
}

int IOInterfaceSQLITE::getAddressAreaCode (const int& addressId)
{
    QSqlQuery query;
    query.prepare("SELECT areaCode FROM keyAddresses WHERE id = ?");
    query.bindValue(0, addressId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toInt();
    }
    else
        return _UNDEFINED;
}

const QString IOInterfaceSQLITE::getAddressCity (const int& addressId)
{
    QSqlQuery query;
    query.prepare("SELECT city FROM keyAddresses WHERE id = ?");
    query.bindValue(0, addressId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }
    else
        return "";
}

bool IOInterfaceSQLITE::setKeychainData (ViewDataKeychain* data, const int& keyCode)
{
    if (!data)
        return false;

    QSqlQuery query;

    query.prepare("SELECT keychainStatusId, internalLocation, addressId, image FROM keychains WHERE id = ?");
    query.bindValue(0, keyCode);
    query.exec();

    if (query.next())
    {
        qDebug () << "ok";

        data->setStatus ((KeychainStatus::Value) query.value(0).toInt());
        data->setInternalLocation(query.value(1).toInt());
        data->setAddressId (query.value(2).toInt());
        QImage img;
        byteArrayToImage (query.value(3).toByteArray(), img);
        data->setImg(img);

        return true;
    }
    return false;
}

// KeychainStatus::Value IOInterfaceSQLITE::getKeychainStatusId (const int& keyCode)
// {
//

//     QSqlQuery query;
//     query.setForwardOnly(true);

//     // get all keys of the keychain
//     query.prepare("SELECT keychainStatusId FROM keychains WHERE barcode = ?");
//     query.bindValue(0, keyCode);
//     query.exec();

//     // set keychain status
//     if (query.next())
//     {
//         mKeychainStatusId = (KeychainStatus::Value) query.value(0).toInt();
//     }

//     return mKeychainStatusId;
// }

bool IOInterfaceSQLITE::initKeyOverviewModel (QSqlRelationalTableModel *model, const QString &filter)
{
    if (model)
    {
        qDebug () << "IOInterfaceSQLITE::initKeyOverviewModel";
        model->setTable("keys");
        model->setRelation(2, QSqlRelation ("keyCategories", "id", "category"));
        model->setRelation(3, QSqlRelation("keyStates", "id", "status"));
        model->setFilter(filter);
        model->select();
        qDebug() << "initKeyOverviewModel OK";
        return true;
    }
    else
        qDebug() << "initKeyOverviewModel NOK";
        return false;
}

bool IOInterfaceSQLITE::initKeychainModel (QSqlRelationalTableModel *model, const QString &filter)
{
    if (model)
    {
        model->setTable("keychains");
        model->setRelation(1, QSqlRelation ("keychainStates", "id", "status"));
        model->setRelation(3, QSqlRelation("keyAddresses", "id", "street, streetNumber, areaCode, city"));
        model->setFilter (filter);
        model->select();
        return true;
    }
    else
        return false;
}

bool IOInterfaceSQLITE::initKeychainHistoryModel (QSqlRelationalTableModel *model, const QString &filter)
{
    if (model)
    {
        model->setTable("handovers");
        model->setFilter (filter);
        model->select();
        return true;
    }
    else
        return false;
}

int IOInterfaceSQLITE::getNumberOfEntries (const QString &tableName)
{
    QString queryString = "SELECT id FROM ";
    queryString.append (tableName);


    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare(queryString);

    if (!query.exec())
        return false;

    //query.size() not supported by sqlite -> workaround to retrieve number of rows
    int numberOfResults = 0;
    if (query.last())
    {
        numberOfResults = query.at() + 1;
    }
    qDebug () << "IOInterfaceSQLITE::getNumberOfEntries: " << numberOfResults;
    return numberOfResults;
}

QVariant IOInterfaceSQLITE::getValue (const QString &tableName, const QString& columnName, int index)
{
    QString queryString = "SELECT ";
    queryString.append(columnName);
    queryString.append(" FROM ");
    queryString.append(tableName);
    queryString.append(" WHERE id = ");
    queryString.append(QString::number(index));


    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare(queryString);
    query.exec();

    if (query.next())
        return query.value(0);

    return _UNDEFINED;
}

QList<QVariant> IOInterfaceSQLITE::getTableColumn (const QString &tableName, const QString& columnName, const QString& filter)
{
    QString queryString = "SELECT ";
    queryString.append(columnName);
    queryString.append(" FROM ");
    queryString.append(tableName);

    // a filter is set
    if ("" != filter)
    {
        queryString.append(" WHERE ");
        queryString.append(filter);
    }

    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare(queryString);
    query.exec();

    QList<QVariant> tableItems;

    while (query.next())
    {
        tableItems << query.value(0);
    }

    return tableItems;
}

QStringList IOInterfaceSQLITE::getTableColumnStringList (const QString &tableName, const QString& columnName)
{
    QString queryString = "SELECT ";
    queryString.append(columnName);
    queryString.append(" FROM ");
    queryString.append(tableName);

    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare(queryString);
    query.exec();

    QStringList tableItems;

    while (query.next())
    {
        tableItems << query.value(0).toString();
    }

    return tableItems;
}

QStringList IOInterfaceSQLITE::getTableColumns (const QString &tableName)
{
    QString queryString = "SELECT name from pragma_table_info(?) as tblInfo;";

    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(0, tableName);
    query.exec();

    QStringList items;

    while (query.next())
    {
        items << query.value(0).toString();
    }

    return items;
}

bool IOInterfaceSQLITE::initRecipientModel (QSqlRelationalTableModel *model)
{
    if (model)
    {
        model->setTable("recipientAddresses");
        model->setRelation(2, QSqlRelation ("recipientTypes", "id", "type"));
        model->select();
        return true;
    }
    else
        return false;
}

bool IOInterfaceSQLITE::initCustomerModel (QSqlRelationalTableModel *model)
{
    bool retVal = false;

    if (model)
    {
        model->setTable("keyAddresses");
        model->select();
        retVal = true;
    }
    return retVal;
}

bool IOInterfaceSQLITE::updateRecipient (unsigned int id, ViewDataRecipient *data)
{
    bool retVal = false;

    if (data)
    {
        QSqlQuery query;
        query.prepare("UPDATE recipientAddresses SET name = ?, type = ?, street = ?, houseNr = ?, areaCode = ?, city = ? WHERE id = ?");
        query.bindValue(0, data->getRecipientName());
        query.bindValue(1, data->getRecipientType());
        query.bindValue(2, data->getRecipientStreet());
        query.bindValue(3, data->getRecipientStreetNumber());
        query.bindValue(4, data->getRecipientAreaCode());
        query.bindValue(5, data->getRecipientCity());
        query.bindValue(6, id);

        retVal = query.exec();
    }
    return retVal;
}

bool IOInterfaceSQLITE::disableRecipient(unsigned int recipientId)
{
    QSqlQuery query;
    query.prepare("UPDATE recipientAddresses SET dbStatus = ? WHERE id = ?");

    DbEntryStatus::Value status = DbEntryStatus::Disabled;

    query.bindValue(0, status);
    query.bindValue(1, recipientId);

    return query.exec();
}

RecipientType::Value IOInterfaceSQLITE::getRecipientTypeId (const QString& recipientType)
{
    QSqlQuery query;

    query.prepare("SELECT id FROM recipientTypes WHERE type = ?");
    query.bindValue(0, recipientType);
    query.exec();

    if (query.next())
    {
        return (RecipientType::Value)query.value(0).toInt();
    }
    else
        return RecipientType::Undefined;
}

bool IOInterfaceSQLITE::addNewRecipient (ViewDataRecipient *data)
{
    if (data)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO recipientAddresses (name, type, street, houseNr, areaCode, city, dbStatus) \
                        VALUES (?, ?, ?, ?, ?, ?, ?)");
        query.bindValue(0, data->getRecipientName());
        query.bindValue(1, data->getRecipientType());
        query.bindValue(2, data->getRecipientStreet());
        query.bindValue(3, data->getRecipientStreetNumber());
        query.bindValue(4, data->getRecipientAreaCode());
        query.bindValue(5, data->getRecipientCity());
        query.bindValue(6, 0);

        return query.exec();
    }
}

bool IOInterfaceSQLITE::addNewCustomer (const IOInterface::customerData *data)
{
    if (!data)
        return false;



    QSqlQuery query;
    query.prepare("INSERT INTO keyAddresses (street, streetNumber, areaCode, city, dbStatus) \
                    VALUES (?, ?, ?, ?, ?)");
    query.bindValue(0, data->street);
    query.bindValue(1, data->number);
    query.bindValue(2, data->areaCode);
    query.bindValue(3, data->city);
    query.bindValue(4, 0);

    return query.exec();
}

bool IOInterfaceSQLITE::getKeychainImg (const int aId, QImage& img)
{
    QSqlQuery query;

    query.prepare("SELECT image FROM keychains WHERE id = ?");
    query.bindValue(0, aId);
    query.exec();

    // set keychain status
    if (query.next())
    {
        byteArrayToImage(query.value(0).toByteArray(), img);
        return true;
    }
    else
        return false;
}

bool IOInterfaceSQLITE::dbInsertHandover (DataInterface *data)
{
    qDebug () << "IOInterfaceSQLITE::handoverInsertEntry:";

    bool queryOk = false;
    mDb.transaction();

    if (data)
    {
        qDebug () << data->getScannedCode();
        qDebug () << getKeychainStatusText(data->getNewKeychainStatusId());
        qDebug () << data->getInternalLocation ();
        qDebug () << data->getRecipientName();
        qDebug () << data->getRecipientStreet();
        qDebug () << data->getRecipientStreetNumber();
        qDebug () << data->getRecipientAreaCode();
        qDebug () << data->getRecipientCity();
        qDebug () << data->getHandoverDate();
        qDebug () << data->getDeadlineDate();
        qDebug () << data->getRecipientAnnotation();
        qDebug () << data->getRecipientSigName();


        QSqlQuery query;

        // insert new handout entry
        query.prepare("INSERT INTO handovers (keychainId, dateHandover, dateDeadline, duration, recipient, recipientStreet, recipientStreetNumber, recipientAreaCode, recipientCity, signatureName, signature, annotation) \
                        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        query.bindValue(0, data->getScannedCode());
        query.bindValue(1, data->getHandoverDate());
        query.bindValue(2, data->getDeadlineDate());
        query.bindValue(3, getKeychainStatusText(data->getNewKeychainStatusId()));
        query.bindValue(4, data->getRecipientName());
        query.bindValue(5, data->getRecipientStreet());
        query.bindValue(6, data->getRecipientStreetNumber());
        query.bindValue(7, data->getRecipientAreaCode());
        query.bindValue(8, data->getRecipientCity());
        query.bindValue(9, data->getRecipientSigName());

        QByteArray img = imageToByteArray (data->getRecipientSignature());
        query.bindValue(10, img);

        query.bindValue(11, data->getRecipientAnnotation());

        queryOk = query.exec();

        if (queryOk)
        {
            query.prepare("UPDATE keychains SET keychainStatusId = ? WHERE id = ?");
            query.bindValue(0, data->getNewKeychainStatusId());
            query.bindValue(1, data->getScannedCode());

            queryOk = query.exec();
        }
    }

    if (queryOk)
        mDb.commit();
    else
        mDb.rollback();

    return queryOk;
}

unsigned int IOInterfaceSQLITE::getLastHandoverId()
{
    QSqlQuery query;

    query.prepare("SELECT id FROM handovers");

    if(query.exec())
    {
        if (!query.next())
        {
            return 0;
        }
        else
        {
            query.last();
            unsigned int id = query.value(0).toUInt();
            qDebug () << "IOInterfaceSQLITE::getLastHandoverId(): " << id;
            return id;
        }
    }
}

bool IOInterfaceSQLITE::dbInsertKeychain (DataInterface *data)
{
    qDebug() << "IOInterfaceSQLITE::dbInsertKeychain";

    bool queryOk = false;

    if (data)
    {

        QSqlQuery query;

        // insert new keychain entry
        query.prepare("INSERT INTO keychains (id, keychainStatusId, internalLocation, addressId, image, dbStatus) \
                        VALUES (?, ?, ?, ?, ?, ?)");
        query.bindValue(0, data->getScannedCode());
        query.bindValue(1, data->getNewKeychainStatusId());
        query.bindValue(2, data->getInternalLocation());
        query.bindValue(3, data->getKeychainAddressId ());
        query.bindValue(4, data->getKeychainImg());
        query.bindValue(5, 0);

        queryOk = query.exec();

        qDebug () << "query.lastError().databaseText():" << query.lastError().databaseText();
    }

    return queryOk;
}

bool IOInterfaceSQLITE::dbInsertKeychainImg (const unsigned int keyCode, const QImage& img)
{
    qDebug() << "IOInterfaceSQLITE::dbInsertKeychainImg";

    QByteArray BaImg;

    if (!img.isNull())
        BaImg = imageToByteArray(img);
    else
        BaImg = 0;

    bool queryOk = false;
    QSqlQuery query;

    // insert new keychain entry
    query.prepare("UPDATE keychains SET image = ? WHERE id = ?");

    query.bindValue(0, BaImg);
    query.bindValue(1, keyCode);

    queryOk = query.exec();

    return queryOk;
}

bool IOInterfaceSQLITE::dbCleanupTable (const QString& tablename, const QString& filter, const int numberOfEntriesToKeep)
{
    QSqlQuery searchQuery;
    mDb.transaction();
    //query.setForwardOnly(true);

    // int firstId = _UNDEFINED;
    // int lastId = _UNDEFINED;
    // int numberOfEntries = _UNDEFINED;

    qDebug () << "dbCleanupTable (const QString& tablename, const int numberOfEntriesToKeep): " << tablename << ", " << numberOfEntriesToKeep;

    //get first and last index
    QString queryString = "SELECT id FROM ";
    queryString.append(tablename);

    if(filter != "")
    {
        queryString.append(" WHERE ");
        queryString.append(filter);
    }

    searchQuery.prepare(queryString);

    qDebug () << "IOInterfaceSQLITE::dbCleanupTable -> queryString SELECT: " << queryString;

    if (!searchQuery.exec())
    {
        mDb.rollback();
        return false;
    }

    //query.size() not supported by sqlite -> workaround to retrieve number of rows
    int numberOfResults = 0;
    if (searchQuery.last())
    {
        numberOfResults = searchQuery.at() + 1;
    }
    qDebug () << "numberOfResults: " << numberOfResults;
    qDebug () << "numberOfEntriesToKeep: " << numberOfEntriesToKeep;

    int rowsToDelete = numberOfResults - numberOfEntriesToKeep;

    QSqlQuery deleteQuery;
    QString deleteQueryString;
    deleteQueryString = "DELETE FROM ";
    deleteQueryString.append(tablename);
    deleteQueryString.append(" WHERE ");
    deleteQueryString.append(filter);
    deleteQueryString.append(" AND id = ");

    if (rowsToDelete > 0)
    {
        searchQuery.first();

        for (int i=0; i < rowsToDelete && searchQuery.isValid(); i++)
        {
            //delete selection
            QString index (searchQuery.value(0).toString());
            int indexLength = index.length();

            deleteQueryString.append(index);

            deleteQuery.prepare(deleteQueryString);
            qDebug () << "queryString: " << deleteQueryString;
            deleteQuery.exec();
            deleteQueryString.chop(indexLength);
            searchQuery.next();
        }
        qDebug () << "dbCleanupTable - OK";
        mDb.commit();
        return true;
    }
    mDb.rollback();
    return false;
}

unsigned int IOInterfaceSQLITE::getFreeKeycode (const unsigned int lockerId)
{
    QSqlQuery query;

    unsigned int shiftedLockerId = lockerId * 10000;
    unsigned int minKeycode = shiftedLockerId + 1;
    unsigned int maxKeycode = shiftedLockerId + 9999;

    for (unsigned int i = minKeycode; i < maxKeycode; i++)
    {
        query.prepare("SELECT id FROM keychains WHERE id = ?");
        query.bindValue(0, i);

        if(query.exec())
        {
            if (!query.next())
            {
                i -= shiftedLockerId;
                qDebug () << "IOInterfaceSQLITE::getFreeKeycode (): " << i;
                return i;
            }
        }
    }

    return 0;
}

QList<QVariant> IOInterfaceSQLITE::getFreeKeycodes (const unsigned int lockerId, const unsigned int quantity)
{
    QSqlQuery query;
    QList<QVariant> values;

    unsigned int shiftedLockerId = lockerId * 10000;
    unsigned int minKeycode = shiftedLockerId + 1;
    unsigned int maxKeycode = shiftedLockerId + 9999;
    int freeCode = -1;

    for (unsigned int i = minKeycode; i < maxKeycode && values.size() < quantity; i++)
    {
        query.prepare("SELECT id FROM keychains WHERE id = ?");
        query.bindValue(0, i);

        if(query.exec())
        {
            if (!query.next())
            {
                freeCode = i - shiftedLockerId;
                qDebug () << "IOInterfaceSQLITE::getFreeKeycodes (): " << i;
                values << freeCode;
            }
        }
    }

    return values;
}

unsigned int IOInterfaceSQLITE::getFreeInternalLocation (const unsigned int lockerId)
{
    if (lockerId > 9999)
        return 0;

    unsigned int shiftedLockerId = lockerId * 10000;
    unsigned int keycodeRangeMin = shiftedLockerId + 1;
    unsigned int keycodeRangeMax = shiftedLockerId + 9999;

    QSqlQuery query;

    for (int i = 1; i<9999; i++)
    {
        query.prepare("SELECT internalLocation FROM keychains WHERE id >= ? AND id <= ? AND internalLocation = ?");
        query.bindValue(0, keycodeRangeMin);
        query.bindValue(1, keycodeRangeMax);
        query.bindValue(2, i);

        if(query.exec())
        {
            if (!query.next())
            {
                qDebug () << "IOInterfaceSQLITE::getFreeInternalLocation - free internal location: " << i;
                return i;
            }
        }
    }

    return 0;
}

unsigned int IOInterfaceSQLITE::getKeycodeFromInternalLocation (const unsigned int lockerId, const unsigned int internalLoc)
{
    qDebug() << "IOInterfaceSQLITE::getKeycodeFromInternalLocation";
    qDebug() << "lockerId: " << lockerId;
    qDebug() << "internalLoc: " << internalLoc;

    if (lockerId > 9999)
        return 0;

    unsigned int shiftedLockerId = lockerId * 10000;
    unsigned int keycodeRangeMin = shiftedLockerId + 1;
    unsigned int keycodeRangeMax = shiftedLockerId + 9999;

    qDebug () << "shiftedLockerId: " << shiftedLockerId;
    qDebug () << "keycodeRangeMin: " << keycodeRangeMin;
    qDebug () << "keycodeRangeMax: " << keycodeRangeMax;

    QSqlQuery query;

    query.prepare("SELECT id FROM keychains WHERE internalLocation = ? AND id > ? AND id < ?");
    query.bindValue(0, internalLoc);
    query.bindValue(1, keycodeRangeMin);
    query.bindValue(2, keycodeRangeMax);

    if(query.exec())
    {
        if (!query.next ())
            return 0;
        else
        {
            qDebug () << "IOInterfaceSQLITE::getKeycodeFromInternalLocation: " << query.value(0).toInt();
            return query.value(0).toInt();
        }
    }
    else
        return 0;
}

const QString IOInterfaceSQLITE::dbGetLastError()
{
    return mDb.lastError().text();
}

void IOInterfaceSQLITE::byteArrayToImage (QByteArray imgBa, QImage& img)
{
    img = QImage::fromData(imgBa, "jpg");
}

bool IOInterfaceSQLITE::setDefaultCameraId(int id)
{
    QSqlQuery query;

    qDebug () << "IOInterfaceSQLITE::setDefaultCameraId - ID: " << id;

    // update setting
    query.prepare("UPDATE appSettings SET cameraId = ? WHERE id = 1");
    query.bindValue(0, id);

    bool queryOk = query.exec();

    qDebug () << "queryOk: " << queryOk;

    return queryOk;
}

bool IOInterfaceSQLITE::setCompanyNameFirstLine(const QString value)
{
    QSqlQuery query;

    // insert new keychain entry
    query.prepare("UPDATE appSettings SET companyNameFirstLine = ? WHERE id = 1");

    query.bindValue(0, value);

    bool queryOk = query.exec();

    return queryOk;
}

bool IOInterfaceSQLITE::setCompanyNameSecondLine(const QString value)
{
    QSqlQuery query;

    // insert new keychain entry
    query.prepare("UPDATE appSettings SET companyNameSecondLine = ? WHERE id = 1");

    query.bindValue(0, value);

    bool queryOk = query.exec();

    return queryOk;
}

bool IOInterfaceSQLITE::setCompanyNameThirdLine(const QString value)
{
    QSqlQuery query;

    // insert new keychain entry
    query.prepare("UPDATE appSettings SET companyNameThirdLine = ? WHERE id = 1");

    query.bindValue(0, value);

    bool queryOk = query.exec();

    return queryOk;
}

bool IOInterfaceSQLITE::setKeyhookDuplicatesAllowed(bool value)
{
    QSqlQuery query;

    // insert new keychain entry
    query.prepare("UPDATE appSettings SET allowKeyhookDuplicates = ? WHERE id = 1");

    query.bindValue(0, value);

    bool queryOk = query.exec();

    return queryOk;
}

int IOInterfaceSQLITE::getDefaultCameraId()
{
    QSqlQuery query;
    int returnValue = 0;

    qDebug () << "IOInterfaceSQLITE::getDefaultCameraId()";

    query.prepare("SELECT cameraId FROM appSettings WHERE id = 1");

    if(query.exec())
    {
        returnValue = query.value(0).toInt();
        qDebug () << "query ok, cam id value: " << returnValue;
    }
    qDebug () << "query Nok";

    return returnValue;
}

const QString IOInterfaceSQLITE::getCompanyNameFirstLine()
{
    QSqlQuery query;
    QString returnValue = "";

    query.prepare("SELECT companyFirstLine FROM appSettings WHERE id = 1");

    if(query.exec())
    {
        returnValue = query.value(0).toString();
    }

    return returnValue;
}

const QString IOInterfaceSQLITE::getCompanyNameSecondLine()
{
    QSqlQuery query;
    QString returnValue = "";

    query.prepare("SELECT companySecondLine FROM appSettings WHERE id = 1");

    if(query.exec())
    {
        returnValue = query.value(0).toString();
    }

    return returnValue;
}

const QString IOInterfaceSQLITE::getCompanyNameThirdLine()
{
    QSqlQuery query;
    QString returnValue = "";

    query.prepare("SELECT companyThirdLine FROM appSettings WHERE id = 1");

    if(query.exec())
    {
        returnValue = query.value(0).toString();
    }

    return returnValue;
}

bool IOInterfaceSQLITE::keyhookDuplicatesAllowed()
{
    QSqlQuery query;
    bool returnValue = false;

    query.prepare("SELECT allowKeyhookDuplicates FROM appSettings WHERE id = 1");

    if(query.exec())
    {
        returnValue = query.value(0).toBool();
    }

    return returnValue;
}

IOInterfaceSQLITE::~IOInterfaceSQLITE()
{

}

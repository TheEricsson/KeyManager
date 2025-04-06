// #include "databaseimpl.h"
// #include <QSettings>
// #include <QSqlQuery>
// #include <QDir>
// #include <QFile>
// #include <QStandardPaths>
// #include <QImage>
// #include <QSqlQueryModel>
// #include <QSqlRelationalTableModel>
// #include <QSqlError>
// #include <QMessageBox>
// #include "dataobjecthandover.h"
// #include "viewdatascanner.h"
// #include "viewdatakeychain.h"

// #ifdef Q_OS_ANDROID
//     #include <QCoreApplication>
//     #include <QtCore/private/qandroidextras_p.h>
// #endif

// DatabaseImpl::DatabaseImpl()
// {
//     // QSettings settings (QString("res/config.ini"), QSettings::IniFormat);
//     // QString dbType = settings.value("database/dbType", "error").toString();
//     // QString dbHostName = settings.value("database/dbHostName", "error").toString();
//     // QString dbDatabaseName = settings.value("database/dbDatabaseName", "error").toString();
//     // QString dbUserName = settings.value("database/dbUserName", "error").toString();
//     // QString dbPassword = settings.value("database/dbPassword", "error").toString();

//     // qDebug () << "opening database -> parameters:" << dbType;
//     // qDebug () << "dbType" << dbType;
//     // qDebug () << "dbHostName" << dbType;
//     // qDebug () << "dbDatabaseName" << dbType;
//     // qDebug () << "dbUserName" << dbType;
//     // qDebug () << "dbPassword" << dbType;

//     // QSqlDatabase db = QSqlDatabase::addDatabase(dbType);
//     // db.setHostName(dbHostName);
//     // db.setDatabaseName(dbDatabaseName);
//     // db.setUserName(dbUserName);
//     // db.setPassword(dbPassword);

//     qDebug () << "paths:";
//     qDebug () << "QStandardPaths::AppConfigLocation" << QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
//     qDebug () << "QStandardPaths::AppDataLocation" << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//     qDebug () << "QStandardPaths::AppLocalDataLocation" << QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
//     qDebug () << "QStandardPaths::ApplicationsLocation" << QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
//     qDebug () << "QStandardPaths::RuntimeLocation" << QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation);
//     qDebug () << "QStandardPaths::DocumentsLocation" << QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

//     mKeychainStatusId = Database::Undefined;

//     QSqlDatabase mDb = QSqlDatabase::addDatabase("QSQLITE");

//     // todo: location is hardcoded by now..
// #ifdef Q_OS_ANDROID
//     //QString dbLocation = "/storage/emulated/0/Android/data/org.qtproject.example.KeyManager/files/db.sqlite";

//     //QString dbLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//     QString dbLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
//     dbLocation.append("/db.sqlite");

//     if (!checkPermission())
//         qDebug () << "DatabaseImpl::checkPermissions failed!";

// #endif
// #ifdef Q_OS_WIN64
//     QString dbLocation = "db.sqlite";
// #endif

//     qDebug () << "dbLocation: " << dbLocation;

//     mDb.setDatabaseName(dbLocation);

//     bool ok = mDb.open ();

//     if(!ok)
//     {
//         QMessageBox::critical(0, "Cannot open database",
//                               "Unable to establish a database connection.\n"
//                                  "This example needs SQLite support. Please read "
//                                  "the Qt SQL driver documentation for information how "
//                                  "to build it.", QMessageBox::Cancel);

//         qDebug () << "AppDataLocation + filename = " << dbLocation;
//         qDebug () << "db.open () returned: " << ok;
//         qDebug () << mDb.isOpen();
//         qDebug () << mDb.isOpenError();

//         qDebug () << "mDb.databaseName(): " << mDb.databaseName();
//         qDebug () << "mDb.connectionName(): " << mDb.connectionName();
//     }

//     QStringList tables = mDb.tables();

//     qDebug () << "number of tables: " << tables.count();

//     for (int i=0;i<tables.count();i++)
//     {
//         qDebug () << "table: " << tables [i];
//     }

//     // database is empty -> create
//     if (0 == tables.count())
//     {
//         firstStart ();
//     }
// }

// #ifdef Q_OS_ANDROID
// bool DatabaseImpl::checkPermission()
// {
//     QList<bool> permissions;

//     auto r = QtAndroidPrivate::checkPermission("android.permission.READ_EXTERNAL_STORAGE").result();
//     if (r != QtAndroidPrivate::Authorized)
//     {
//         r = QtAndroidPrivate::requestPermission("android.permission.READ_EXTERNAL_STORAGE").result();
//         if (r == QtAndroidPrivate::Denied)
//             permissions.append(false);
//     }
//     r = QtAndroidPrivate::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE").result();
//     if (r != QtAndroidPrivate::Authorized)
//     {
//         r = QtAndroidPrivate::requestPermission("android.permission.WRITE_EXTERNAL_STORAGE").result();
//         if (r == QtAndroidPrivate::Denied)
//             permissions.append(false);
//     }
//     r = QtAndroidPrivate::checkPermission("android.permission.READ_MEDIA_IMAGES").result();
//     if (r != QtAndroidPrivate::Authorized)
//     {
//         r = QtAndroidPrivate::requestPermission("android.permission.READ_MEDIA_IMAGES").result();
//         if (r == QtAndroidPrivate::Denied)
//             permissions.append(false);
//     }

//     for (int i = 0; i < permissions.count(); i++)
//     {
//         qDebug () << "permissions[i]" << permissions.at(i);
//     }
//     return (permissions.count() != 3);
// }
// #endif

// bool DatabaseImpl::firstStart ()
// {
//     QSqlQuery query;

//     // TODO before deployment!!! add all tables for first start

//     bool retVal = false;

//     retVal = query.exec ("CREATE TABLE keyAddresses ( \
//                id           INTEGER PRIMARY KEY AUTOINCREMENT \
//                UNIQUE, \
//                street       TEXT, \
//                streetNumber TEXT, \
//                areaCode     INTEGER, \
//                city         TEXT)");

//     // query.exec ("CREATE TABLE handovers ( \
//     //     id            INTEGER PRIMARY KEY AUTOINCREMENT \
//     //         UNIQUE, \
//     //     keychainId    INTEGER REFERENCES keychains (barcode), \
//     //     dateHandover  TEXT, \
//     //     dateDeadline  TEXT, \
//     //     recipientId   INTEGER REFERENCES recipientAddresses (id), \
//     //     signatureName TEXT, \
//     //     signature     BLOB)");

//     return retVal;
// }

// bool DatabaseImpl::findKeyCode(int aCode)
// {
//     mDb.transaction();

//     QSqlQuery query;
//     query.prepare("SELECT barcode FROM keychains WHERE barcode = ?");
//     query.bindValue(0, aCode);
//     query.exec();

//     if (query.next())
//     {
//         qDebug () << "found the Barcode!";
//         return true;
//     }
//     return false;
// }

// //keychain data
// Database::KeychainStatus DatabaseImpl::getKeychainStatusId (const int& keyCode)
// {
//     mDb.transaction();

//     QSqlQuery query;
//     query.prepare("SELECT keychainStatusId FROM keychains WHERE barcode = ?");
//     query.bindValue(0, keyCode);
//     query.exec();

//     if (query.next())
//     {
//         return (Database::KeychainStatus) query.value(0).toInt();
//     }
//     else
//         return Database::Undefined;
// }

// const QString DatabaseImpl::getKeychainStatusText (int statusId)
// {
//     mDb.transaction();

//     QSqlQuery query;
//     query.prepare("SELECT status FROM keychainStates WHERE id = ?");
//     query.bindValue(0, statusId);
//     query.exec();

//     if (query.next())
//     {
//         return query.value(0).toString();
//     }
//     else
//         return "Fehler: Status unbekannt!";
// }

// int DatabaseImpl::getKeychainInternalLocation (const int& keyCode)
// {
//     mDb.transaction();

//     QSqlQuery query;
//     query.prepare("SELECT internalLocation FROM keychains WHERE barcode = ?");
//     query.bindValue(0, keyCode);
//     query.exec();

//     if (query.next())
//     {
//         return query.value(0).toInt();
//     }
//     else
//         return _UNDEFINED;
// }

// int DatabaseImpl::getKeychainAddressId (const int& keyCode)
// {
//     mDb.transaction();

//     QSqlQuery query;
//     query.prepare("SELECT addressId FROM keychains WHERE barcode = ?");
//     query.bindValue(0, keyCode);
//     query.exec();

//     if (query.next())
//     {
//         return query.value(0).toInt();
//     }
//     else
//         return _UNDEFINED;
// }

// const QString DatabaseImpl::getKeychainImgPath (const int& keyCode)
// {
//     mDb.transaction();

//     QSqlQuery query;
//     query.prepare("SELECT imagePath FROM keychains WHERE barcode = ?");
//     query.bindValue(0, keyCode);
//     query.exec();

//     if (query.next())
//     {
//         return query.value(0).toString();
//     }
//     else
//         return "UNKNOWN";
// }

// bool DatabaseImpl::setKeychainData (ViewDataKeychain* data, const int& keyCode)
// {
//     if (!data)
//         return false;

//     mDb.transaction();

//     QSqlQuery query;

//     query.prepare("SELECT keychainStatusId, internalLocation, addressId, imagePath FROM keychains WHERE barcode = ?");
//     query.bindValue(0, keyCode);
//     query.exec();

//     if (query.next())
//     {
//         qDebug () << "ok";

//         data->setStatus ((Database::KeychainStatus) query.value(0).toInt());
//         data->setInternalLocation(query.value(1).toInt());
//         data->setAddressId (query.value(2).toInt());
//         data->setImgPath (query.value(3).toString());

//         return true;
//     }
//     return false;
// }

// bool DatabaseImpl::setKeyCode (int aCode)
// {
//     qDebug () << "DatabaseImpl::setKeyCode";

//     return false;

//     // mDb.transaction();

//     // QSqlQuery query;
//     // query.setForwardOnly(true);
//     // query.prepare("SELECT keychainStatusId, internalLocation, addressId, imagePath FROM keychains WHERE barcode = ?");
//     // query.bindValue(0, aCode);
//     // query.exec();

//     // if (query.next())
//     // {
//     //     qDebug () << "ok";

//     //     mKeychainId = query.value(0).toInt();
//     //     mKeychainInternalLocation = query.value(1).toInt();
//     //     int addressId = query.value(2).toInt();
//     //     mKeychainImgPath = query.value(4).toString();


//     //     int keychainStatusId = query.value(1).toInt();
//     //     qDebug () << "mKeychainId: " << mKeychainId;

//     //     query.prepare("SELECT status FROM keychainStates WHERE id = ?");
//     //     query.bindValue(0, keychainStatusId);
//     //     query.exec();

//     //     // set keychain status
//     //     if (query.next())
//     //     {
//     //         qDebug () << "ok";
//     //         mKeychainStatus = query.value(0).toString();
//     //     }
//     //     else
//     //     {
//     //         return false;
//     //     }

//     //     // get all keys of the keychain
//     //     query.prepare("SELECT id, categoryId, statusId, description FROM keys WHERE keychainId = ?");
//     //     query.bindValue(0, mKeychainId);
//     //     query.exec();

//     //     int count = 0;

//     //     mKeychainItems.clear();

//     //     while (query.next()) // todo: always returns only 1 value
//     //     {
//     //         count++;
//     //         qDebug () << "ok: " << count;

//     //         int keyId = query.value(0).toInt();
//     //         int categoryId = query.value(1).toInt();
//     //         int statusId = query.value(2).toInt();
//     //         QString description = query.value(3).toString();

//     //         QSqlQuery queryKeyContents;

//     //         //get address from id
//     //         queryKeyContents.prepare("SELECT street, streetNumber, areaCode, city FROM keyAddresses WHERE id = ?");
//     //         queryKeyContents.bindValue(0, addressId);
//     //         queryKeyContents.exec();

//     //         QString street = "";
//     //         QString streetNumber = "";
//     //         int areaCode = 0;
//     //         QString city = "";

//     //         if (queryKeyContents.next())
//     //         {
//     //             qDebug () << "okA";
//     //             street = queryKeyContents.value(0).toString();
//     //             streetNumber = queryKeyContents.value(1).toString();
//     //             areaCode = queryKeyContents.value(2).toInt();
//     //             city = queryKeyContents.value(3).toString();
//     //         }
//     //         else
//     //             return false;

//     //         // get category
//     //         queryKeyContents.prepare("SELECT category FROM keyCategories WHERE id = ?");
//     //         queryKeyContents.bindValue(0, categoryId);
//     //         queryKeyContents.exec();

//     //         QString category ("");

//     //         if (queryKeyContents.next())
//     //         {
//     //             qDebug () << "okB";
//     //             category = queryKeyContents.value(0).toString();
//     //         }
//     //         else
//     //             return false;

//     //         // get status
//     //         queryKeyContents.prepare("SELECT status FROM keyStates WHERE id = ?");
//     //         queryKeyContents.bindValue(0, statusId);
//     //         queryKeyContents.exec();

//     //         QString status ("");

//     //         if (queryKeyContents.next())
//     //         {
//     //             qDebug () << "okC";
//     //             status = queryKeyContents.value(0).toString();
//     //         }
//     //         else
//     //             return false;

//     //         mKey key;
//     //         key.street = street;
//     //         key.streetNumber = streetNumber;
//     //         key.areaCode = areaCode;
//     //         key.category = category;
//     //         key.dbId = keyId;
//     //         key.description = description;
//     //         key.keychainId = mKeychainId;
//     //         key.status = status;

//     //         mKeychainItems.append(key);
//     //     }
//     // }
//     // else
//     //     return false;
//     // {
//     //     for (int i = 0; i < mKeychainItems.count(); i++)
//     //     {
//     //         qDebug () << "mKeychainItems[i].street: " << mKeychainItems[i].street;
//     //         qDebug () << "mKeychainItems[i].streetNumber: " << mKeychainItems[i].streetNumber;
//     //         qDebug () << "mKeychainItems[i].areaCode: " << mKeychainItems[i].areaCode;
//     //         qDebug () << "mKeychainItems[i].category: " << mKeychainItems[i].category;
//     //         qDebug () << "mKeychainItems[i].dbId: " << mKeychainItems[i].dbId;
//     //         qDebug () << "mKeychainItems[i].description: " << mKeychainItems[i].description;
//     //         qDebug () << "mKeychainItems[i].keychainId: " << mKeychainItems[i].keychainId;
//     //         qDebug () << "mKeychainItems[i].status: " << mKeychainItems[i].status;

//     //     }
//     // }
//     return true;
// }

// // Database::KeychainStatus DatabaseImpl::getKeychainStatusId (const int& keyCode)
// // {
// //     mDb.transaction();

// //     QSqlQuery query;
// //     query.setForwardOnly(true);

// //     // get all keys of the keychain
// //     query.prepare("SELECT keychainStatusId FROM keychains WHERE barcode = ?");
// //     query.bindValue(0, keyCode);
// //     query.exec();

// //     // set keychain status
// //     if (query.next())
// //     {
// //         mKeychainStatusId = (Database::KeychainStatus) query.value(0).toInt();
// //     }

// //     return mKeychainStatusId;
// // }

// bool DatabaseImpl::initKeyOverviewModel (QSqlRelationalTableModel *model, const QString &filter)
// {
//     if (model)
//     {
//         model->setTable("keys");
//         model->setRelation(2, QSqlRelation ("keyCategories", "id", "category"));
//         model->setRelation(3, QSqlRelation("keyStates", "id", "status"));
//         model->setFilter(filter);
//         model->select();
//         return true;
//     }
//     else return false;
// }

// bool DatabaseImpl::initKeychainModel (QSqlRelationalTableModel *model, const QString &filter)
// {
//     if (model)
//     {
//         model->setTable("keychains");
//         model->setRelation(1, QSqlRelation ("keychainStates", "id", "status"));
//         model->setRelation(3, QSqlRelation("keyAddresses", "id", "street, streetNumber, areaCode, city"));
//         model->setFilter (filter);
//         model->select();
//         return true;
//     }
//     else return false;
// }

// bool DatabaseImpl::initRecipientModel (QSqlRelationalTableModel *model)
// {
//     if (model)
//     {
//         model->setTable("recipientAddresses");
//         model->setRelation(2, QSqlRelation ("recipientTypes", "id", "type"));
//         model->select();
//         return true;
//     }
//     else
//         return false;
// }

// bool DatabaseImpl::initBuildingModel (QSqlRelationalTableModel *model)
// {
//     if (model)
//     {
//         model->setTable("keyAddresses");
//         model->select();
//         return true;
//     }
//     else
//         return false;
// }

// bool DatabaseImpl::addNewRecipient(const Database::RecipientType& type, const QString& name, const QString& street, const QString& number, const QString& areaCode, const QString& city)
// {
//     mDb.transaction();

//     QSqlQuery query;
//     query.prepare("INSERT INTO recipientAddresses (name, type, street, houseNr, areaCode, city) \
//                     VALUES (?, ?, ?, ?, ?, ?)");
//     query.bindValue(0, name);
//     query.bindValue(1, type);
//     query.bindValue(2, street);
//     query.bindValue(3, number);
//     query.bindValue(4, areaCode);
//     query.bindValue(5, city);

//     return query.exec();
// }

// bool DatabaseImpl::addNewRecipient (const RecipientData& data)
// {
//     mDb.transaction();

//     QSqlQuery query;
//     query.prepare("INSERT INTO recipientAddresses (name, type, street, houseNr, areaCode, city) \
//                     VALUES (?, ?, ?, ?, ?, ?)");
//     query.bindValue(0, data.name);
//     query.bindValue(1, data.type);
//     query.bindValue(2, data.street);
//     query.bindValue(3, data.number);
//     query.bindValue(4, data.areaCode);
//     query.bindValue(5, data.city);

//     return query.exec();
// }

// bool DatabaseImpl::dataUpdate (DataObject *data)
// {
//     // check NULL pointer
//     if (!data)
//         return false;

//     DataObjectHandover *dataObj = (DataObjectHandover*)data;

//     if (dataObj)
//     {
//         return dataUpdate (dataObj);
//     }
//     return false;
// }

// bool DatabaseImpl::update (ViewDataScanner *data)
// {
//     if (!data)
//         return false;

//     return setKeyCode(data->getBarcode());
// }

// const QString DatabaseImpl::getKeychainImagePath (int aId)
// {
//     mDb.transaction();
//     QSqlQuery query;

//     // get all keys of the keychain
//     query.prepare("SELECT imagePath FROM keychains WHERE barcode = ?");
//     query.bindValue(0, aId);
//     query.exec();

//     QString result ("");

//     // set keychain status
//     if (query.next())
//     {
//         result = query.value(0).toString();
//     }
//     qDebug () << "getKeychainImagePath: " << result;
//     return result;
// }

// bool DatabaseImpl::dbInsertHandover (DataInterface *data)
// {
//     qDebug () << "DatabaseImpl::handoverInsertEntry:";

//     bool queryOk = false;

//     if (data)
//     {
//         qDebug () << data->getScannedCode();
//         qDebug () << getKeychainStatusText(data->getNewKeychainStatusId());
//         qDebug () << data->getInternalLocation ();
//         qDebug () << data->getRecipientName();
//         qDebug () << data->getRecipientStreet();
//         qDebug () << data->getRecipientStreetNumber();
//         qDebug () << data->getRecipientAreaCode();
//         qDebug () << data->getRecipientCity();
//         qDebug () << data->getHandoverDate();
//         qDebug () << data->getDeadlineDate();
//         qDebug () << data->getRecipientAnnotation();
//         qDebug () << data->getRecipientSigName();

//         mDb.transaction();
//         QSqlQuery query;

//         // insert new handout entry
//         query.prepare("INSERT INTO handovers (keychainId, dateHandover, dateDeadline, duration, recipient, recipientStreet, recipientStreetNumber, recipientAreaCode, recipientCity, signatureName, signature, annotation) \
//                         VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
//         query.bindValue(0, data->getScannedCode());
//         query.bindValue(1, data->getHandoverDate());
//         query.bindValue(2, data->getDeadlineDate());
//         query.bindValue(3, getKeychainStatusText(data->getNewKeychainStatusId()));
//         query.bindValue(4, data->getRecipientName());
//         query.bindValue(5, data->getRecipientStreet());
//         query.bindValue(6, data->getRecipientStreetNumber());
//         query.bindValue(7, data->getRecipientAreaCode());
//         query.bindValue(8, data->getRecipientCity());
//         query.bindValue(9, data->getRecipientSigName());
//         query.bindValue(10, "image.png");
//         //query.bindValue(10, data->getRecipientSignature ()); TODO: bytearray shit
//         query.bindValue(11, data->getRecipientAnnotation());

//         queryOk = query.exec();

//         if (queryOk)
//         {
//             query.prepare("UPDATE keychains SET keychainStatusId = ? WHERE barcode = ?");
//             query.bindValue(0, data->getNewKeychainStatusId());
//             query.bindValue(1, data->getScannedCode());

//             queryOk = query.exec();
//         }

//         return queryOk;
//     }
//     return queryOk;
// }

// bool DatabaseImpl::dbInsertKeychain (DataInterface *data)
// {
//     bool queryOk = false;

//     if (data)
//     {
//         mDb.transaction();
//         QSqlQuery query;

//         // insert new keychain entry
//         query.prepare("INSERT INTO keychains (barcode, keychainStatusId, internalLocation, addressId, imagePath) \
//                         VALUES (?, ?, ?, ?, ?)");
//         query.bindValue(0, data->getScannedCode());
//         query.bindValue(1, data->getNewKeychainStatusId());
//         query.bindValue(2, data->getInternalLocation());
//         query.bindValue(3, data->getKeychainAddressId ());
//         query.bindValue(4, data->getKeychainImgPath());

//         queryOk = query.exec();
//     }
//     return queryOk;
// }

// bool DatabaseImpl::dbCleanupTable (const QString& tablename, const QString& filter, const int numberOfEntriesToKeep)
// {
//     mDb.transaction();
//     QSqlQuery searchQuery;
//     //query.setForwardOnly(true);

//     int firstId = _UNDEFINED;
//     int lastId = _UNDEFINED;
//     int numberOfEntries = _UNDEFINED;

//     qDebug () << "dbCleanupTable (const QString& tablename, const int numberOfEntriesToKeep): " << tablename << ", " << numberOfEntriesToKeep;

//     //get first and last index
//     QString queryString = "SELECT id FROM ";
//     queryString.append(tablename);

//     if(filter != "")
//     {
//         queryString.append(" WHERE ");
//         queryString.append(filter);
//     }

//     searchQuery.prepare(queryString);

//     qDebug () << "DatabaseImpl::dbCleanupTable -> queryString SELECT: " << queryString;

//     if (!searchQuery.exec())
//         return false;

//     //query.size() not supported by sqlite -> workaround to retrieve number of rows
//     int numberOfResults = 0;
//     if (searchQuery.last())
//     {
//         numberOfResults = searchQuery.at() + 1;
//     }
//     qDebug () << "numberOfResults: " << numberOfResults;
//     qDebug () << "numberOfEntriesToKeep: " << numberOfEntriesToKeep;

//     int rowsToDelete = numberOfResults - numberOfEntriesToKeep;

//     QSqlQuery deleteQuery;
//     QString deleteQueryString;
//     deleteQueryString = "DELETE FROM ";
//     deleteQueryString.append(tablename);
//     deleteQueryString.append(" WHERE ");
//     deleteQueryString.append(filter);
//     deleteQueryString.append(" AND id = ");

//     if (rowsToDelete > 0)
//     {
//         searchQuery.first();

//         for (int i=0; i < rowsToDelete && searchQuery.isValid(); i++)
//         {
//             //delete selection
//             QString index (searchQuery.value(0).toString());
//             int indexLength = index.length();

//             deleteQueryString.append(index);

//             deleteQuery.prepare(deleteQueryString);
//             qDebug () << "queryString: " << deleteQueryString;
//             deleteQuery.exec();
//             deleteQueryString.chop(indexLength);
//             searchQuery.next();
//         }
//         qDebug () << "dbCleanupTable - OK";
//         return true;
//     }

//     return false;
// }

// int DatabaseImpl::getFreeInternalLocation ()
// {
//     mDb.transaction();
//     QSqlQuery query;

//     for (int i = 1; i<99999; i++)
//     {
//         query.prepare("SELECT internalLocation FROM keychains WHERE internalLocation = ?");
//         query.bindValue(0, i);

//         if(query.exec())
//         {
//             if (!query.next())
//                 return i;
//         }
//         else
//             return _UNDEFINED;
//     }
// }

// int DatabaseImpl::findInternalLocation (const int internalLoc)
// {
//     mDb.transaction();
//     QSqlQuery query;

//     query.prepare("SELECT barcode FROM keychains WHERE internalLocation = ?");
//     query.bindValue(0, internalLoc);

//     if(query.exec())
//     {
//         if (!query.next ())
//             return 0;
//         else
//             return query.value(0).toInt();
//     }
//     else
//         return _UNDEFINED;
// }

// bool DatabaseImpl::dataUpdate (DataObjectHandover *data)
// {
//     if (!data)
//         return false;

//     qDebug () << "DatabaseImpl::dataUpdate:";

//     qDebug () << data->getKeychainId();
//     qDebug () << data->getKeychainStatus();
//     qDebug () << data->getInternalLocation();
//     qDebug () << data->getRecipient();
//     qDebug () << data->getRecipientStreet();
//     qDebug () << data->getRecipientStreetNumber();
//     qDebug () << data->getRecipientAreaCode();
//     qDebug () << data->getRecipientCity();
//     qDebug () << data->getDateHandover();
//     qDebug () << data->getDateDeadline();
//     qDebug () << data->getAnnotation();
//     qDebug () << data->getSignatureName();

//     mDb.transaction();
//     QSqlQuery query;

//     // get all keys of the keychain
//     query.prepare("SELECT status FROM keychainStates WHERE id = ?");
//     query.bindValue(0, data->getKeychainStatus());
//     bool queryOk = query.exec();

//     // get keychain status text
//     QString keychainStatusText;

//     if (query.next())
//     {
//         keychainStatusText = query.value(0).toString();
//     }
//     else
//         queryOk = false;

//     if (queryOk)
//     {
//         // insert new handout entry
//         query.prepare("INSERT INTO handovers (keychainId, dateHandover, dateDeadline, duration, recipient, recipientStreet, recipientStreetNumber, recipientAreaCode, recipientCity, signatureName, signature, annotation) \
//                         VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
//         query.bindValue(0, data->getKeychainId());
//         query.bindValue(1, data->getDateHandover());
//         query.bindValue(2, data->getDateDeadline());
//         query.bindValue(3, keychainStatusText);
//         query.bindValue(4, data->getRecipient());
//         query.bindValue(5, data->getRecipientStreet());
//         query.bindValue(6, data->getRecipientStreetNumber());
//         query.bindValue(7, data->getRecipientAreaCode());
//         query.bindValue(8, data->getRecipientCity());
//         query.bindValue(9, data->getSignatureName());
//         query.bindValue(10, data->getSignature());
//         query.bindValue(11, data->getAnnotation());

//         queryOk = query.exec();
//     }

//     if (queryOk)
//     {
//         query.prepare("UPDATE keychains SET keychainStatusId = ? WHERE barcode = ?");
//         query.bindValue(0, data->getKeychainStatus());
//         query.bindValue(1, data->getKeychainId());

//         queryOk = query.exec();
//     }

//     return queryOk;
// }

// // bool DatabaseImpl::initializeKeyOverviewModel (QSqlQueryModel *model, int aCode)
// // {
// //     qDebug () << "initializeKeyOverviewModel (QSqlQueryModel *model, int aClientId, int aKeyId)";

// //     if (model)
// //     {
// //         qDebug () << "ok";

// //         if (setKeyCode (aCode))
// //         {
// //             qDebug () << "blabla";

// //             mDb.transaction();

// //             QSqlQuery query;
// //             query.prepare("SELECT * FROM keys WHERE keychainId = ? INNER JOIN keyCategories ON keys.categoryId = keyCategories.category");
// //             query.bindValue(0, aCode);

// //             //model->setQuery("SELECT * FROM keys WHERE keychainId = 1");
// //             model->setQuery("SELECT category, status, description \
// //                             FROM keys \
// //                             INNER JOIN keyCategories ON keyCategories.id = keys.categoryId \
// //                             INNER JOIN keyStates ON keyStates.id = keys.statusId \
// //                             WHERE keychainId = 1");

// //             return true;
// //         }
// //     }
// //     return false;
// // }

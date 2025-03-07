#include "databaseimpl.h"
#include <QSettings>
#include <QSqlQuery>
#include <QDir>
#include <QFile>
#include <QStandardPaths>

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

bool DatabaseImpl::findKey(int aKeyId)
{
    mDb.transaction();

    QSqlQuery query;
    QString command;
    command.append("SELECT * FROM keys WHERE id = '");
    command.append(QString::number(aKeyId));
    command.append("'");
    query.exec(command);

    qDebug () << "DatabaseImpl::findKey(int aKeyId)";
    qDebug () << "command: " << command;

    if (query.next())
    {
        return true;
    }

    return false;
}

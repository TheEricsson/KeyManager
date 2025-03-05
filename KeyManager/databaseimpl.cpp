#include "databaseimpl.h"
#include <QSettings>
#include <QSqlQuery>
#include <QDir>

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
    mDb.setDatabaseName("db.sqlite");

    bool ok = mDb.open ();

    qDebug () << QDir::currentPath();

    qDebug () << "db.open () returned: " << ok;
    qDebug () << mDb.isOpen();
    qDebug () << mDb.isOpenError();

    qDebug () << "mDb.databaseName(): " << mDb.databaseName();
    qDebug () << "mDb.connectionName(): " << mDb.connectionName();
    QStringList tables = mDb.tables();

    qDebug () << "number of entries: " << tables.count();

    for (int i=0;i<tables.count();i++)
    {
        qDebug () << "entry: " << tables [i];
    }
}

bool DatabaseImpl::findKeyId(const QString& aKeyId)
{
    mDb.transaction();

    QSqlQuery query;
    QString command;
    command.append("SELECT * FROM keys WHERE id = '");
    command.append(aKeyId);
    command.append("'");
    query.exec(command);

    if (query.next())
    {
        return true;
    }

    return false;
}

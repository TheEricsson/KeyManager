#ifndef DATABASEIMPL_H
#define DATABASEIMPL_H

#include <QSqlDatabase>
#include <QImage>
class QSqlQueryModel;
class QSqlRelationalTableModel;

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
    int getKeychainStatusId (int aId);
    bool findKeyCode(int aCode);
    bool setKeyCode(int aCode);
    bool initKeyOverviewModel (QSqlRelationalTableModel *model, int aCode);
    bool initKeychainModel (QSqlRelationalTableModel *model, int aId);
    //bool initializeKeyOverviewModel (QSqlQueryModel *model, int aCode);
    //const QSqlQueryModel& getKeysModel (int aId);

private:
#ifdef Q_OS_ANDROID
    bool checkPermission();
#endif
    QSqlDatabase mDb;

    int mKeychainId;
    int mKeychainInternalLocation;
    QImage mKeychainImg;
    QString mKeychainStatus;
    QList <mKey> mKeychainItems;
    int mKeychainStatusId;
};

#endif // DATABASEIMPL_H

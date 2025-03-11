#ifndef DATABASEIMPL_H
#define DATABASEIMPL_H

#include <QSqlDatabase>
#include <QImage>
class QSqlQueryModel;

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
    bool findKeyCode(int aClientId, int aKeyId);
    bool setKeyCode(int aClientId, int aKeyId);
    bool initializeKeyOverviewModel (QSqlQueryModel *model, int aClientId, int aKeyId);

private:
    QSqlDatabase mDb;

    int mKeychainId;
    int mKeychainInternalLocation;
    QImage mKeychainImg;
    QString mKeychainStatus;
    QList <mKey> mKeychainItems;
};

#endif // DATABASEIMPL_H

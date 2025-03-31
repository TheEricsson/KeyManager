#ifndef DATABASEIMPL_H
#define DATABASEIMPL_H

#include <QSqlDatabase>
#include <QImage>
#include "globals.h"

class QSqlQueryModel;
class QSqlRelationalTableModel;
class DataObject;
class DataObjectHandover;

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
    Database::eKeychainStatusId getKeychainStatusId (int aId);
    bool findKeyCode(int aCode);
    bool setKeyCode(int aCode);
    bool initKeyOverviewModel (QSqlRelationalTableModel *model, int aCode);
    bool initKeychainModel (QSqlRelationalTableModel *model, int aId);
    bool initRecipientModel (QSqlRelationalTableModel *model);
    bool addNewRecipient(const RecipientType& type, const QString& name, const QString& street, const QString& number, const QString& areaCode, const QString& city);
    bool addNewRecipient (const RecipientData& data);
    bool dataUpdate (DataObject *data);
    const QString getKeychainImagePath (int aId);
    //bool initializeKeyOverviewModel (QSqlQueryModel *model, int aCode);
    //const QSqlQueryModel& getKeysModel (int aId);

private:
    bool dataUpdate (DataObjectHandover *data);

#ifdef Q_OS_ANDROID
    bool checkPermission();
#endif
    QSqlDatabase mDb;

    int mKeychainId;
    int mKeychainInternalLocation;
    QImage mKeychainImg;
    QString mKeychainStatus;
    QList <mKey> mKeychainItems;
    Database::eKeychainStatusId mKeychainStatusId;
};

#endif // DATABASEIMPL_H

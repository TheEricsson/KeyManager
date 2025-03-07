#ifndef DATABASEIMPL_H
#define DATABASEIMPL_H

#include <QSqlDatabase>

class DatabaseImpl
{
public:
    DatabaseImpl();
    bool findBarcode(int aClientId, int aKeyId);

private:
    QSqlDatabase mDb;
};

#endif // DATABASEIMPL_H

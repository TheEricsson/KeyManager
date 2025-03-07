#ifndef DATABASEIMPL_H
#define DATABASEIMPL_H

#include <QSqlDatabase>

class DatabaseImpl
{
public:
    DatabaseImpl();
    bool findKey(int aKeyId);
private:
    QSqlDatabase mDb;
};

#endif // DATABASEIMPL_H

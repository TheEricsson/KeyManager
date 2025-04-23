#ifndef IOINTERFACE_H
#define IOINTERFACE_H

class QSqlRelationalTableModel;
class QString;
class ViewDataKeychain;
class DataInterface;
class ViewDataScanner;

#include <QVariant>
#include "globals.h"

class IOInterface
{
public:
    struct keyData
    {
        int keychainId = _UNDEFINED;
        int categoryId = _UNDEFINED;
        int statusId = _UNDEFINED;
        QString description = "";
    };

    struct recipientData
    {
        Database::RecipientType type;
        QString name;
        QString street;
        QString number;
        QString areaCode;
        QString city;
    };

    struct customerData
    {
        QString street;
        QString number;
        QString areaCode;
        QString city;
    };

    IOInterface();
    virtual ~IOInterface();
    virtual int countDbTables()                                                                 {return _UNDEFINED;};
    virtual bool initFirstStart()                                                               {return false;};
    virtual bool addKey (const IOInterface::keyData *data)                                      {Q_UNUSED(data); return false;};
    virtual bool findKeyCode(unsigned int keyCode)                                              {Q_UNUSED(keyCode); return false;};
    virtual unsigned int getLockerIdByKeycode (unsigned int keyCode)                            {Q_UNUSED(keyCode); return 0;};
    virtual unsigned int getKeyIdByKeycode (unsigned int keyCode)                               {Q_UNUSED(keyCode); return 0;};
    virtual Database::KeychainStatus getKeychainStatusId (const int& keyCode)                   {Q_UNUSED(keyCode); return Database::Undefined;};
    virtual const QString getKeychainStatusText (int statusId)                                  {Q_UNUSED(statusId); return "IOInterface::getKeychainStatusText - not implemented.";};
    virtual int getKeychainInternalLocation (const int& keyCode)                                {Q_UNUSED(keyCode); return _UNDEFINED;};
    virtual int getKeychainAddressId (const int& keyCode)                                       {Q_UNUSED(keyCode); return _UNDEFINED;};
    virtual const QString getAddressStreet (const int& addressId)                               {Q_UNUSED(addressId); return "IOInterface::getAddressStreet - not implemented.";};
    virtual const QString getAddressStreetNumber (const int& addressId)                         {Q_UNUSED(addressId); return "IOInterface::getAddressStreetNumber - not implemented.";};
    virtual int getAddressAreaCode (const int& addressId)                                       {Q_UNUSED(addressId); return _UNDEFINED;};
    virtual const QString getAddressCity (const int& addressId)                                 {Q_UNUSED(addressId); return "IOInterface::getAddressCity - not implemented.";};
    virtual bool getKeychainImg (const int keyCode, QImage& img)                                {Q_UNUSED(keyCode); Q_UNUSED(img); return false;};
    virtual bool setKeychainData (ViewDataKeychain* data, const int& keyCode)                   {Q_UNUSED(keyCode); Q_UNUSED(data); return false;};
    virtual int getNumberOfEntries (const QString &tableName)                                         {Q_UNUSED(tableName); return _UNDEFINED;};
    virtual QVariant getValue (const QString &tableName, const QString& columnName, int index)  {Q_UNUSED(tableName); Q_UNUSED(columnName); Q_UNUSED(index); return _UNDEFINED;};
    virtual bool initKeyOverviewModel (QSqlRelationalTableModel *model, const QString &filter)  {Q_UNUSED(model); Q_UNUSED(filter); return false;};
    virtual bool initKeychainModel (QSqlRelationalTableModel *model, const QString &filter)     {Q_UNUSED(model); Q_UNUSED(filter); return false;};
    virtual bool initKeychainHistoryModel (QSqlRelationalTableModel *model, const QString &filter) {Q_UNUSED(model); Q_UNUSED(filter); return false;};
    virtual bool initRecipientModel (QSqlRelationalTableModel *model)                           {Q_UNUSED(model); return false;};
    virtual bool initCustomerModel (QSqlRelationalTableModel *model)                            {Q_UNUSED(model); return false;};
    virtual bool addNewRecipient (const IOInterface::recipientData *data)                       {Q_UNUSED(data); return false;};
    virtual bool addNewCustomer (const IOInterface::customerData *data)                         {Q_UNUSED(data); return false;};
    virtual bool dbInsertHandover (DataInterface *data)                                         {Q_UNUSED(data); return false;};
    virtual bool dbInsertKeychain (DataInterface *data)                                         {Q_UNUSED(data); return false;};
    virtual bool dbInsertKeychainImg (const unsigned int keyCode, const QImage& img)            {Q_UNUSED(keyCode); Q_UNUSED(img); return false;};
    virtual bool dbCleanupTable (const QString& tablename, \
                                const QString& filter, \
                                const int numberOfEntriesToKeep)                                {Q_UNUSED(tablename); Q_UNUSED(filter); Q_UNUSED(numberOfEntriesToKeep); return false;};
    virtual unsigned int getFreeKeycode (const unsigned int lockerId)                           {return _UNDEFINED;};
    virtual unsigned int getFreeInternalLocation (const unsigned int lockerId)                  {return _UNDEFINED;};
    virtual unsigned int getKeycodeFromInternalLocation (const unsigned int lockerId, const unsigned int internalLoc) {Q_UNUSED(internalLoc); return _UNDEFINED;};
    virtual void byteArrayToImage (QByteArray imgBa, QImage& img)                               {Q_UNUSED(imgBa); Q_UNUSED(img);}
#ifdef Q_OS_ANDROID
    virtual bool checkPermission(){};
#endif
};

#endif // IOINTERFACE_H

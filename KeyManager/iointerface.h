#ifndef IOINTERFACE_H
#define IOINTERFACE_H

class QSqlRelationalTableModel;
class QString;
class ViewDataKeychain;
class DataInterface;
class ViewDataScanner;
class ViewDataRecipient;

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
        RecipientType::Value type;
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

    //general
    virtual int countDbTables()                                                                 {return _UNDEFINED;};
    virtual bool initFirstStart()                                                               {return false;};
    virtual const QString getDatabaseLocation()                                                 {return "";};
    virtual bool openDatabase()                                                                 {return false;};
    virtual bool closeDatabase()                                                                {return false;};
    virtual bool doDbBackup(const QString& fileName)                                            {Q_UNUSED(fileName); return false;};

    virtual int getNumberOfEntries (const QString &tableName)                                   {Q_UNUSED(tableName); return _UNDEFINED;};
    virtual QVariant getValue (const QString &tableName, const QString& columnName, int index)  {Q_UNUSED(tableName); Q_UNUSED(columnName); Q_UNUSED(index); return _UNDEFINED;};
    virtual QList<QVariant> getTableColumn (const QString &tableName, \
                                            const QString& columnName, \
                                            const QString& filter = "")                         {Q_UNUSED(tableName); Q_UNUSED(columnName); Q_UNUSED(filter);};
    virtual QStringList getTableColumnStringList (const QString &tableName, const QString& columnName)    {Q_UNUSED(tableName); Q_UNUSED(columnName);};
    virtual QStringList getTableColumns (const QString &tableName)                              {Q_UNUSED(tableName);};
    virtual bool dbCleanupTable (const QString& tablename, \
                                const QString& filter, \
                                const int numberOfEntriesToKeep)                                {Q_UNUSED(tablename); Q_UNUSED(filter); Q_UNUSED(numberOfEntriesToKeep); return false;};
    virtual unsigned int getFreeKeycode (const unsigned int lockerId)                           {Q_UNUSED(lockerId); return _UNDEFINED;};
    virtual unsigned int getFreeInternalLocation (const unsigned int lockerId)                  {Q_UNUSED(lockerId); return _UNDEFINED;};
    virtual const QString dbGetLastError()                                                      {return "IOInterface::dbGetLastError - not implemented.";};
    virtual void byteArrayToImage (QByteArray imgBa, QImage& img)                               {Q_UNUSED(imgBa); Q_UNUSED(img);}

    //keychains
    virtual bool findKeyCode(unsigned int keyCode)                                              {Q_UNUSED(keyCode); return false;};
    virtual KeychainStatus::Value getKeychainStatusId (const int& keyCode)                      {Q_UNUSED(keyCode); return KeychainStatus::Undefined;};
    virtual const QString getKeychainStatusText (int statusId)                                  {Q_UNUSED(statusId); return "IOInterface::getKeychainStatusText - not implemented.";};
    virtual int getKeychainInternalLocation (const int& keyCode)                                {Q_UNUSED(keyCode); return _UNDEFINED;};
    virtual unsigned int getLockerIdByKeycode (unsigned int keyCode)                            {Q_UNUSED(keyCode); return 0;};
    virtual int getKeychainAddressId (const int& keyCode)                                       {Q_UNUSED(keyCode); return _UNDEFINED;};
    virtual bool getKeychainImg (const int keyCode, QImage& img)                                {Q_UNUSED(keyCode); Q_UNUSED(img); return false;};
    virtual bool setKeychainData (ViewDataKeychain* data, const int& keyCode)                   {Q_UNUSED(keyCode); Q_UNUSED(data); return false;};
    virtual bool dbInsertKeychain (DataInterface *data)                                         {Q_UNUSED(data); return false;};
    virtual bool dbInsertKeychainImg (const unsigned int keyCode, const QImage& img)            {Q_UNUSED(keyCode); Q_UNUSED(img); return false;};
    virtual unsigned int getKeycodeFromInternalLocation (const unsigned int lockerId, \
                                                        const unsigned int internalLoc)         {Q_UNUSED(lockerId); Q_UNUSED(internalLoc); return _UNDEFINED;};

    //keys
    virtual bool addKey (const IOInterface::keyData *data)                                      {Q_UNUSED(data); return false;};
    virtual QList<QVariant> getKeyIdsByKeycode (unsigned int keyCode)                           {Q_UNUSED(keyCode);};
    virtual unsigned int getKeyCategoryId(unsigned int keyId)                                   {Q_UNUSED(keyId); return 0;};
    virtual unsigned int getKeyStatusId(unsigned int keyId)                                     {Q_UNUSED(keyId); return 0;};
    virtual QString getKeyDescription(unsigned int keyId)                                       {Q_UNUSED(keyId); return "";};
    virtual const QString getKeyCategoryString(unsigned int keyCategoryId)                      {Q_UNUSED(keyCategoryId); return "";};
    virtual const QString getKeyStatusString(unsigned int keyStatusId)                          {Q_UNUSED(keyStatusId); return "";};

    //addresses
    virtual const QString getAddressStreet (const int& addressId)                               {Q_UNUSED(addressId); return "IOInterface::getAddressStreet - not implemented.";};
    virtual const QString getAddressStreetNumber (const int& addressId)                         {Q_UNUSED(addressId); return "IOInterface::getAddressStreetNumber - not implemented.";};
    virtual int getAddressAreaCode (const int& addressId)                                       {Q_UNUSED(addressId); return _UNDEFINED;};
    virtual const QString getAddressCity (const int& addressId)                                 {Q_UNUSED(addressId); return "IOInterface::getAddressCity - not implemented.";};

    //models
    virtual bool initKeyOverviewModel (QSqlRelationalTableModel *model, const QString &filter)  {Q_UNUSED(model); Q_UNUSED(filter); return false;};
    virtual bool initKeychainModel (QSqlRelationalTableModel *model, const QString &filter = "") {Q_UNUSED(model); Q_UNUSED(filter); return false;};
    virtual bool initKeychainHistoryModel (QSqlRelationalTableModel *model, const QString &filter) {Q_UNUSED(model); Q_UNUSED(filter); return false;};
    virtual bool initRecipientModel (QSqlRelationalTableModel *model)                           {Q_UNUSED(model); return false;};
    virtual bool initCustomerModel (QSqlRelationalTableModel *model)                            {Q_UNUSED(model); return false;};

    //recipients
    virtual bool addNewRecipient (ViewDataRecipient *data)                                      {Q_UNUSED(data); return false;};
    virtual RecipientType::Value getRecipientTypeId (const QString& recipientType)              {Q_UNUSED(recipientType); return RecipientType::Undefined;};
    virtual bool updateRecipient (unsigned int id, ViewDataRecipient *data)                     {Q_UNUSED(id);Q_UNUSED(data); return false;};
    virtual bool disableRecipient(unsigned int recipientId)                                     {Q_UNUSED(recipientId); return false;};

    //personal data

    virtual bool addNewCustomer (const IOInterface::customerData *data)                         {Q_UNUSED(data); return false;};

    //handovers
    virtual bool dbInsertHandover (DataInterface *data)                                         {Q_UNUSED(data); return false;};
    virtual unsigned int getLastHandoverId()                                                    {return 0;};

    //app settings
    virtual bool setDefaultCameraId(int id)                                                     {Q_UNUSED(id); return false;};
    virtual bool setCompanyNameFirstLine(const QString value)                                   {Q_UNUSED(value); return false;};
    virtual bool setCompanyNameSecondLine(const QString value)                                  {Q_UNUSED(value); return false;};
    virtual bool setCompanyNameThirdLine(const QString value)                                   {Q_UNUSED(value); return false;};
    virtual bool setKeyhookDuplicatesAllowed(bool value)                                        {Q_UNUSED(value); return false;};

    virtual int getDefaultCameraId()                                                            {return -1;};
    virtual const QString getCompanyNameFirstLine()                                             {return "IOInterface: not implemented!";};
    virtual const QString getCompanyNameSecondLine()                                            {return "IOInterface: not implemented!";};
    virtual const QString getCompanyNameThirdLine()                                             {return "IOInterface: not implemented!";};
    virtual bool keyhookDuplicatesAllowed()                                                     {return false;};



#ifdef Q_OS_ANDROID
    virtual bool checkPermission()                                                              {return false;};
#endif
};

#endif // IOINTERFACE_H

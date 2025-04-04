#ifndef IOINTERFACE_H
#define IOINTERFACE_H

class QSqlRelationalTableModel;
class QString;
class ViewDataKeychain;
class DataInterface;
class ViewDataScanner;

#include "globals.h"

class IOInterface
{
public:
    IOInterface();
    virtual ~IOInterface();
    virtual bool findKeyCode(int aCode)                                                         {Q_UNUSED(aCode); return false;};
    virtual Database::KeychainStatus getKeychainStatusId (const int& keyCode)                   {Q_UNUSED(keyCode); return Database::Undefined;};
    virtual const QString getKeychainStatusText (int statusId)                                  {Q_UNUSED(statusId); return "IOInterface::getKeychainStatusText - not implemented.";};
    virtual int getKeychainInternalLocation (const int& keyCode)                                {Q_UNUSED(keyCode); return _UNDEFINED;};
    virtual int getKeychainAddressId (const int& keyCode)                                       {Q_UNUSED(keyCode); return _UNDEFINED;};
    virtual const QString getKeychainImgPath (const int& keyCode)                               {Q_UNUSED(keyCode); return "IOInterface::getKeychainImgPath - not implemented.";};
    virtual bool setKeychainData (ViewDataKeychain* data, const int& keyCode)                   {Q_UNUSED(keyCode); Q_UNUSED(data); return false;};
    virtual bool initKeyOverviewModel (QSqlRelationalTableModel *model, const QString &filter)  {Q_UNUSED(model); Q_UNUSED(filter); return false;};
    virtual bool initKeychainModel (QSqlRelationalTableModel *model, const QString &filter)     {Q_UNUSED(model); Q_UNUSED(filter); return false;};
    virtual bool initRecipientModel (QSqlRelationalTableModel *model)                           {Q_UNUSED(model); return false;};
    virtual bool initBuildingModel (QSqlRelationalTableModel *model)                            {Q_UNUSED(model); return false;};
    virtual bool addNewRecipient(const Database::RecipientType& type, \
                                 const QString& name, \
                                 const QString& street, \
                                 const QString& number, \
                                 const QString& areaCode, \
                                 const QString& city)                                           {Q_UNUSED(type); Q_UNUSED(name); Q_UNUSED(street); Q_UNUSED(number); Q_UNUSED(areaCode); Q_UNUSED(city); return false;};
    virtual bool addNewRecipient (const RecipientData& data)                                    {Q_UNUSED(data); return false;};
    virtual bool dbInsertHandover (DataInterface *data)                                         {Q_UNUSED(data); return false;};
    virtual bool dbInsertKeychain (DataInterface *data)                                         {Q_UNUSED(data); return false;};
    virtual bool dbCleanupTable (const QString& tablename, \
                                const QString& filter, \
                                const int numberOfEntriesToKeep)                                {Q_UNUSED(tablename); Q_UNUSED(filter); Q_UNUSED(numberOfEntriesToKeep); return false;};
    virtual int getFreeInternalLocation ()                                                      {return _UNDEFINED;};
    virtual int findInternalLocation (const int internalLoc)                                    {Q_UNUSED(internalLoc); return _UNDEFINED;};
private:
    virtual bool firstStart ()                                                                  {return false;};
#ifdef Q_OS_ANDROID
    virtual bool checkPermission(){};
#endif
};

#endif // IOINTERFACE_H

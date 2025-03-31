#ifndef VIEWDATAKEYCHAINSTATUS_H
#define VIEWDATAKEYCHAINSTATUS_H

class ViewDataKeychainStatus
{
public:
    ViewDataKeychainStatus();
    void setInternalLocation (const int& id){mInternalLocation = id;};
    void setStatus (const int& statusId){mStatusId = statusId;};

    const int& getInternalLocation (){return mInternalLocation;};
    const int& getStatus (){return mStatusId;};
private:
    int mInternalLocation = 0;
    int mStatusId = 0;
};

#endif // VIEWDATAKEYCHAINSTATUS_H

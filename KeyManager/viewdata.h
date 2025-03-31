#ifndef VIEWDATA_H
#define VIEWDATA_H

class ViewDataHandover;
class ViewDataKeychainStatus;
class ViewDataRecipient;
class ViewDataReturnDate;
class ViewDataScanner;

class ViewData
{  
    public:
        ViewData();
        ~ViewData();

        void setData (ViewDataHandover* data);
        void setData (ViewDataKeychainStatus* data);
        void setData (ViewDataRecipient* data);
        void setData (ViewDataReturnDate* data);
        void setData (ViewDataScanner* data);
        void deleteData ();

        ViewDataHandover* getDataHandover () {return mDataHandover;};
        ViewDataKeychainStatus* getDataKeychainStatus () {return mDataKeychainStatus;};
        ViewDataRecipient* getDataRecipient () {return mDataRecipient;};
        ViewDataReturnDate* getDataReturnDate () {return mDataReturnDate;};
        ViewDataScanner* getDataScanner () {return mDataScanner;};

    private:
        ViewDataHandover *mDataHandover;
        ViewDataKeychainStatus *mDataKeychainStatus;
        ViewDataRecipient *mDataRecipient;
        ViewDataReturnDate *mDataReturnDate;
        ViewDataScanner *mDataScanner;
};

#endif // VIEWDATA_H

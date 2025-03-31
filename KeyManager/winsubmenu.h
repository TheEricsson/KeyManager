#ifndef WINSUBMENU_H
#define WINSUBMENU_H

#include <QWidget>
#include "globals.h"

class QVBoxLayout;
class QLabel;
class QPushButton;
class DataObject;
class ViewDataInterface;
class ViewDataScanner;
class ViewDataKeychainStatus;
class ViewDataRecipient;
class ViewDataReturnDate;
class ViewDataHandover;
class DataInterface;
class DatabaseImpl;
class QHBoxLayout;

class WinSubmenu : public QWidget
{
    Q_OBJECT
public:
    explicit WinSubmenu (QWidget *parent = nullptr);
    ~WinSubmenu ();
    //void activate ();
    //void setDataObject (DataObject *data);
    void setDataInterface (DataInterface *data);
    //void setDatabaseHandle (DatabaseImpl *db);
    void setData (ViewDataScanner *data);
    //DatabaseImpl* getDatabaseHandle (){return mDatabase;};

protected:
    /*void setData (ViewDataScanner *scanData);
    void setData (ViewDataKeychainStatus *scanData);
    void setData (ViewDataRecipient *scanData);
    void setData (ViewDataReturnDate *scanData);
    void setData (ViewDataHandover *scanData);*/

    DataInterface* dataInterface (){return mDataInterface;};
    void setHeader (const QString& label);
    void setButtonText (int column, const QString &btnText);
    void setMenuButtons (const QList<Gui::MenuButton> &buttons);
    void disableButton (int column, bool disable);
    void enableButton (int column, bool enable);

signals:
    void menuButtonClicked (Gui::MenuButton btnType);
    //void viewDataChanged (ViewDataInterface *data);

private slots:
    void onMenuBtnClicked(Gui::MenuButton btnType);

private:

    QList <QPushButton*> mMenuButtons;

    //DatabaseImpl *mDatabase;
    DataObject *mDataObject;
    DataInterface *mDataInterface;

    QVBoxLayout *mLayout;
    QLabel *mHeaderLabel;
    bool mButtonsSet;

    QHBoxLayout *mButtonLayout;
};

#endif // WINSUBMENU_H

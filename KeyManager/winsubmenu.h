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
class ViewData;
class DatabaseImpl;
class QHBoxLayout;

class WinSubmenu : public QWidget
{
    Q_OBJECT
public:
    explicit WinSubmenu (QWidget *parent = nullptr);
    ~WinSubmenu ();
    void activate ();
    void setDataObject (DataObject *data);
    void setDataObject (ViewData *data);
    void setDatabaseHandle (DatabaseImpl *db);
    void setData (ViewDataScanner *data);
    DatabaseImpl* getDatabaseHandle (){return mDatabase;};

protected:
    /*void setData (ViewDataScanner *scanData);
    void setData (ViewDataKeychainStatus *scanData);
    void setData (ViewDataRecipient *scanData);
    void setData (ViewDataReturnDate *scanData);
    void setData (ViewDataHandover *scanData);*/

    ViewData* getViewData (){return mViewData;};
    void setHeader (const QString& label);
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

    DatabaseImpl *mDatabase;
    DataObject *mDataObject;
    ViewData *mViewData;

    QVBoxLayout *mLayout;
    QLabel *mHeaderLabel;
    bool mButtonsSet;

    QHBoxLayout *mButtonLayout;
};

#endif // WINSUBMENU_H

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
class IOInterface;
class QGridLayout;
class QBoxLayout;

class WinSubmenu : public QWidget
{
    Q_OBJECT
public:
    explicit WinSubmenu (QWidget *parent = nullptr);
    ~WinSubmenu ();
    void setDataInterface (DataInterface *data);
    void setIOInterface (IOInterface *ioData);
    void setData (ViewDataScanner *data);

protected:

    void setTopLayout (QLayout *layout);
    void setCentralLayout (QLayout *layout);
    void setBottomLayout (QLayout *layout);

    QLayout* getTopLayout ();
    QLayout* getCentralLayout ();
    QLayout* getBottomLayout ();

    DataInterface* dataInterface (){return mDataInterface;};
    IOInterface* ioInterface (){return mIOInterface;};
    void setHeader (const QString& label);
    void setButtonText (int column, const QString &btnText);
    void setMenuButtons (const QList<Gui::MenuButton> &buttons);
    void disableButton (int column, bool disable);
    void enableButton (int column, bool enable);
    void hideButton (int column, bool disable);
    void showButton (int column, bool enable);

signals:
    void menuButtonClicked (Gui::MenuButton btnType);

private slots:
    void onMenuBtnClicked(Gui::MenuButton btnType);

private:
    virtual void keyReleaseEvent (QKeyEvent *event);
    QList <QPushButton*> mMenuButtons;
    QLabel *mHeaderLabel;

    QBoxLayout *mBaseLayout;
    QBoxLayout *mTopLayout;
    QBoxLayout *mCentralLayout;
    QBoxLayout *mBottomLayout;

    DataInterface *mDataInterface;
    IOInterface *mIOInterface;
};

#endif // WINSUBMENU_H

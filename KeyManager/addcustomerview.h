#ifndef ADDCUSTOMERVIEW_H
#define ADDCUSTOMERVIEW_H

#include <QWidget>
#include "winsubmenu.h"

class QLineEdit;

class AddCustomerView : public WinSubmenu
{
    Q_OBJECT
public:
    explicit AddCustomerView(QWidget *parent = nullptr);
private slots:
    virtual void onMenuBtnClicked (Gui::MenuButton btnType);

private:
    void reset ();
    bool checkSelections ();
    void showEvent(QShowEvent *);
    void updateForm();

    QLineEdit *mStreet;
    QLineEdit *mStreetNr;
    QLineEdit *mAreaCode;
    QLineEdit *mCity;
};

#endif // ADDCUSTOMERVIEW_H

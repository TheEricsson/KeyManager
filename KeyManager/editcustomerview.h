#ifndef EDITCUSTOMERVIEW_H
#define EDITCUSTOMERVIEW_H

#include "winsubmenu.h"
#include <QWidget>

class QSqlRelationalTableModel;
class QTableView;

class EditCustomerView : public WinSubmenu
{
    Q_OBJECT
    public:
        EditCustomerView(QWidget *parent = nullptr);
    private:
        void showEvent(QShowEvent *);
        void reset();
        bool setCustomersModel(QSqlRelationalTableModel* model);
        QSqlRelationalTableModel *mCustomerModel;
        QTableView *mCustomerView;
};

#endif // EDITCUSTOMERVIEW_H

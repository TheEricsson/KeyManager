#ifndef ADDKEYVIEW_H
#define ADDKEYVIEW_H

#include <QWidget>
#include "winsubmenu.h"

class QSqlRelationalTableModel;

class AddKeyView : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit AddKeyView(QWidget *parent = nullptr);
    private:
        QSqlRelationalTableModel *mViewModel;
    private slots:
        void updateView ();
};

#endif // ADDKEYVIEW_H

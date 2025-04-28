#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QObject>
#include <QWidget>

#include "winsubmenu.h"

class QSqlRelationalTableModel;
class QSortFilterProxyModel;
class QTableView;

class TableView : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit TableView(const QString &tableName, QWidget *parent = nullptr);

    private:
        virtual void showEvent(QShowEvent *);
        bool setKeychainModel (QSqlRelationalTableModel* model);
        QSqlRelationalTableModel *mKeychainModel;
        QSortFilterProxyModel *mFilteredKeychainModel;
        QTableView *mKeychain;
};

#endif // TABLEVIEW_H

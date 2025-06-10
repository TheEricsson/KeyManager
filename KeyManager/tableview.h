#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QObject>
#include <QWidget>

#include "winsubmenu.h"

class QSqlRelationalTableModel;
class QSortFilterProxyModel;
class QTableWidget;

class TableView : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit TableView(const QString &tableName, QWidget *parent = nullptr);

    private:
        void getData();
        virtual void showEvent(QShowEvent *);
        QTableWidget *mTable;
        QString mTableName;
};

#endif // TABLEVIEW_H

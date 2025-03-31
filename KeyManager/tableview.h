#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QObject>
#include <QWidget>
#include "winsubmenu.h"

class QSqlTableModel;

class TableView : public WinSubmenu
{
    Q_OBJECT
public:
    explicit TableView(const QString &tableName, QWidget *parent = nullptr);

signals:
    void previousButtonClicked ();
private:
    QSqlTableModel *mAdresses;
    QSqlTableModel *mKeys;

private slots:
    void onPreviousBtnClicked();
};

#endif // TABLEVIEW_H

#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QObject>
#include <QWidget>

class QSqlTableModel;

class TableView : public QWidget
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

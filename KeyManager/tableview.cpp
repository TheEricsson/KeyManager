#include "tableview.h"

#include <QSqlTableModel>
#include <QTableView>
#include <QHBoxLayout>

TableView::TableView(const QString &tableName, QWidget *parent)
    : QWidget{parent}
{
    mAdresses = new QSqlTableModel(this);
    mAdresses->setTable(tableName);
    mAdresses->setEditStrategy(QSqlTableModel::OnFieldChange);
    mAdresses->select();

    mAdresses->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mAdresses->setHeaderData(1, Qt::Horizontal, tr("Straße"));
    mAdresses->setHeaderData(2, Qt::Horizontal, tr("Hausnummer"));
    mAdresses->setHeaderData(3, Qt::Horizontal, tr("PLZ"));
    mAdresses->setHeaderData(4, Qt::Horizontal, tr("Ort"));

    QTableView *view = new QTableView;
    view->setModel(mAdresses);
    view->resizeColumnsToContents();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(view);
    //mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(tr("Übersicht"));
    show();

    qDebug () <<"TableView::TableView called";
}

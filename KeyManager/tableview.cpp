#include "tableview.h"

#include <QSqlTableModel>
#include <QTableView>
#include <QHBoxLayout>

TableView::TableView(const QString &tableName, QWidget *parent)
    : QWidget{parent}
{
    mTableModel = new QSqlTableModel(this);
    mTableModel->setTable(tableName);
    //mTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    mTableModel->select();

    mTableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mTableModel->setHeaderData(1, Qt::Horizontal, tr("Straße"));
    mTableModel->setHeaderData(2, Qt::Horizontal, tr("Hausnummer"));
    mTableModel->setHeaderData(3, Qt::Horizontal, tr("PLZ"));
    mTableModel->setHeaderData(4, Qt::Horizontal, tr("Ort"));

    QTableView *view = new QTableView;
    view->setModel(mTableModel);
    view->resizeColumnsToContents();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(view);
    //mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(tr("Übersicht"));
    show();

    qDebug () <<"TableView::TableView called";
}

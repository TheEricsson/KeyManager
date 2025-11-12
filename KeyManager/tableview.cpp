#include "tableview.h"

#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include "iointerface.h"

TableView::TableView(const QString &tableName, QWidget *parent)
    : WinSubmenu {parent}
{
    setHeader("Tabelle");

    mTable = new QTableWidget ();
    mTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->addWidget(mTable);
    setCentralLayout(centralLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    setMenuButtons(menuButtons);

    mTableName = tableName;
}

void TableView::showEvent(QShowEvent *)
{
    showFullScreen();
    getData();
}

void TableView::getData()
{
    if (ioInterface())
    {
        int rows = ioInterface()->getNumberOfEntries (mTableName);
        QStringList columnHeaders = ioInterface()->getTableColumns (mTableName);

        mTable->setColumnCount(columnHeaders.size());
        mTable->setRowCount(rows);

        mTable->setHorizontalHeaderLabels(columnHeaders);

        //step through rows
        for (int i = 0; i < rows; i++)
        {
            //step through columns
            int j = 0;
            for (; j < columnHeaders.size(); j++)
            {
                QString entry = ioInterface()->getValue (mTableName, columnHeaders.at(j), i+1).toString();
                QTableWidgetItem *item = new QTableWidgetItem(entry);
                mTable->setItem(i, j, item);
            }
        }
    }
}

#include "tableview.h"

#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QHBoxLayout>
#include <QPushButton>
#include "iointerface.h"

TableView::TableView(const QString &tableName, QWidget *parent)
    : WinSubmenu {parent}
{
    mKeychainModel = 0;
    mFilteredKeychainModel = 0;
    //setHeader("Suchfunktion");

    mKeychain = new QTableView (this);
    mKeychain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *centralLayout = new QHBoxLayout();
    centralLayout->addWidget(mKeychain);
    setCentralLayout(centralLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    setMenuButtons(menuButtons);
}

void TableView::showEvent(QShowEvent *)
{
    if (!mKeychainModel)
        mKeychainModel = new QSqlRelationalTableModel (this);

    if (!mFilteredKeychainModel)
        mFilteredKeychainModel = new QSortFilterProxyModel (this);

    ioInterface()->initKeychainModel(mKeychainModel);
    setKeychainModel(mKeychainModel);
    mFilteredKeychainModel->setSourceModel(mKeychainModel);
    mKeychain->setModel(mFilteredKeychainModel);
}

bool TableView::setKeychainModel (QSqlRelationalTableModel* model)
{
    if (model)
    {
        model->setHeaderData(0, Qt::Horizontal, tr("Barcode"), Qt::DisplayRole);
        model->setHeaderData(1, Qt::Horizontal, tr("Ausgabestatus"), Qt::DisplayRole);
        model->setHeaderData(2, Qt::Horizontal, tr("Schlüsselhaken"), Qt::DisplayRole);
        model->setHeaderData(3, Qt::Horizontal, tr("Straße"), Qt::DisplayRole);
        model->setHeaderData(4, Qt::Horizontal, tr("Hausnummer"), Qt::DisplayRole);
        model->setHeaderData(5, Qt::Horizontal, tr("PLZ"), Qt::DisplayRole);
        model->setHeaderData(6, Qt::Horizontal, tr("Ort"), Qt::DisplayRole);

        if (!mFilteredKeychainModel)
            return false;

        if (mKeychain)
        {
            mFilteredKeychainModel->setSourceModel(model);
            mKeychain->hideColumn(7); // hide image column
            mKeychain->setEditTriggers(QTableView::NoEditTriggers);
            mKeychain->setSelectionMode(QTableView::NoSelection);
            //mKeychain->verticalHeader()->hide();

            mKeychain->show();

            //mKeychain->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
            //mKeychain->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

            //mKeychain->resizeColumnsToContents();

            return true;
        }
        return false;
    }
    return false;
}

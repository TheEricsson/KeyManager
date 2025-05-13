#include "searchview.h"
#include <QSpacerItem>
#include <QLayout>
#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>
#include "iointerfacesqlite.h"

SearchView::SearchView (QWidget *parent)
    : WinSubmenu {parent}
{
    mDataView = new QTreeView (this);
    mDataView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mDataView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    mStandardModel = new QStandardItemModel (this);
    mDataView->setModel(mStandardModel);

    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->addWidget(mDataView);
    setCentralLayout(centralLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    setMenuButtons(menuButtons);
}

void SearchView::showEvent(QShowEvent *)
{
    setTreeData();

    /*
    QList<QStandardItem *> preparedRow =prepareRow("first", "second", "third");
    QStandardItem *item = mStandardModel->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item->appendRow(preparedRow);

    QList<QStandardItem *> secondRow =prepareRow("111", "222", "333");
    // adding a row to an item starts a subtree
    preparedRow.first()->appendRow(secondRow);*/
}

void SearchView::setTreeData()
{
    unsigned int numberOfKeychains = ioInterface()->getNumberOfEntries("keychains");
    unsigned int numberOfKeys = ioInterface()->getNumberOfEntries("keys");

    //keychain data as row: id, status, internalLocation, address

    QList keychainColumnId = ioInterface()->getTableColumn ("keychains", "id");

    for (int i = 0; i < keychainColumnId.size(); i++)
    {
        unsigned int id = keychainColumnId.at(i).toUInt();
        KeychainStatus::Value status = ioInterface()->getKeychainStatusId(id);
        unsigned int internalLoc = ioInterface()->getKeychainInternalLocation(id);
        unsigned int addressId = ioInterface()->getKeychainAddressId(id);

        QString addressStreet = ioInterface()->getAddressStreet(addressId);
        QString addressStreetNr = ioInterface()->getAddressStreetNumber(addressId);
        QString addressAreaCode = QString::number(ioInterface()->getAddressAreaCode(addressId));
        QString addressCity = ioInterface()->getAddressCity(addressId);

        QString statusText = ioInterface()->getKeychainStatusText(status);
        QString readableId = Database::normaliseKeycode(id);
        QString internalLocString = QString::number(internalLoc);

        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(readableId);
        rowItems << new QStandardItem(internalLocString);
        rowItems << new QStandardItem(addressStreet);
        rowItems << new QStandardItem(addressStreetNr);
        rowItems << new QStandardItem(addressAreaCode);
        rowItems << new QStandardItem(addressCity);
        rowItems << new QStandardItem(statusText);

        QStandardItem *item = mStandardModel->invisibleRootItem();
        // adding a row to the invisible root item produces a root element
        item->appendRow(rowItems);
    }
}

//test...
QList<QStandardItem *> SearchView::prepareRow(const QString &first,
                                              const QString &second,
                                              const QString &third)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(first);
    rowItems << new QStandardItem(second);
    rowItems << new QStandardItem(third);
    return rowItems;
}

SearchView::~SearchView()
{
    if (0 != mStandardModel)
        delete mStandardModel;
}

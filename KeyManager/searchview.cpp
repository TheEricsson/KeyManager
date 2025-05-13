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
    mStandardModel->clear();
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

    if (0 < keychainColumnId.size())
    {
        //add row
        unsigned int id = 0;
        KeychainStatus::Value status = KeychainStatus::Undefined;
        unsigned int internalLoc = 0;
        unsigned int addressId = 0;

        QString addressStreet ("");
        QString addressStreetNr ("");
        QString addressAreaCode ("");
        QString addressCity ("");

        QString statusText ("");
        QString readableId ("");
        QString internalLocString ("");

        for (int i = 0; i < keychainColumnId.size(); i++)
        {
            id = keychainColumnId.at(i).toUInt();
            status = ioInterface()->getKeychainStatusId(id);
            internalLoc = ioInterface()->getKeychainInternalLocation(id);
            addressId = ioInterface()->getKeychainAddressId(id);

            addressStreet = ioInterface()->getAddressStreet(addressId);
            addressStreetNr = ioInterface()->getAddressStreetNumber(addressId);
            addressAreaCode = QString::number(ioInterface()->getAddressAreaCode(addressId));
            addressCity = ioInterface()->getAddressCity(addressId);

            statusText = ioInterface()->getKeychainStatusText(status);
            readableId = Database::normaliseKeycode(id);
            internalLocString = QString::number(internalLoc);

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

            //add row child items (keys)
            QList keys = ioInterface()->getKeyIdsByKeycode(id);
            unsigned int keyCatId = 0;
            unsigned int keyStatusId = 0;
            QString keyDescription ("");
            unsigned int currentKeyId = 0;

            for (unsigned j = 0; j < keys.size(); j++)
            {
                currentKeyId = keys.at(j).toUInt();
                qDebug() << "current key id: " << currentKeyId;
                keyCatId = ioInterface()->getKeyCategoryId(currentKeyId);
                keyStatusId = ioInterface()->getKeyStatusId(currentKeyId);
                keyDescription = ioInterface()->getKeyDescription(currentKeyId);

                //set key infos
                QPixmap logo (":/images/key.png");
                QIcon iconPic(logo);
                QStandardItem *keyIcon = new QStandardItem();
                keyIcon->setIcon(iconPic);

                QList<QStandardItem*> childItems;
                childItems << keyIcon; //new QStandardItem("Schlüssel");
                childItems << new QStandardItem(QString::number(keyCatId));
                childItems << new QStandardItem(QString::number(keyStatusId));
                childItems << new QStandardItem(keyDescription);

                rowItems.first()->appendRow(childItems);
            }
        }
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

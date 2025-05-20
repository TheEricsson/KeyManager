#include "searchview.h"
#include <QSpacerItem>
#include <QLayout>
#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QRegularExpression>

#include "iointerfacesqlite.h"
#include "qheaderview.h"

SearchView::SearchView (QWidget *parent)
    : WinSubmenu {parent}
{
    mDataView = new QTreeView (this);
    mDataView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    mDataView->setContentsMargins(0, 0, 0, 0);
    //mDataView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mDataView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mDataView->setIndentation(30);
    mDataView->setAnimated(true);
    mDataView->setWordWrap(true);
    mDataView->setDragEnabled(false);

    mStandardModel = new QStandardItemModel (this);

    //filter for each line edit -> these are chained one by another
    mFilterProperty = new QSortFilterProxyModel(this);
    mFilterProperty->setSourceModel(mStandardModel);

    mFilterQrCode  = new QSortFilterProxyModel(this);
    mFilterQrCode->setSourceModel(mFilterProperty);

    mFilterKeyPosition  = new QSortFilterProxyModel(this);
    mFilterKeyPosition->setSourceModel(mFilterQrCode);

    mFilterStatus = new QSortFilterProxyModel(this);
    mFilterStatus->setSourceModel(mFilterKeyPosition);

    mFilterInfo  = new QSortFilterProxyModel(this);
    mFilterInfo->setSourceModel(mFilterStatus);

    mFilterInfo->setRecursiveFilteringEnabled(true);
    mDataView->setModel(mFilterInfo);

    // line edits for filters
    mPropertyLineEdit = new QLineEdit();
    mQrCodeLineEdit = new QLineEdit();
    mKeyPositionLineEdit = new QLineEdit();
    mKeyStatusLineEdit = new QLineEdit();
    mKeyInfoLineEdit = new QLineEdit();

    QGroupBox *filters = new QGroupBox(tr("Suchfilter"));
    QFormLayout *formLayout = new QFormLayout();
    QPushButton *resetFilters = new QPushButton (tr("Alle Filter zurücksetzen"));

    formLayout->addRow(tr("&Liegenschaft:"), mPropertyLineEdit);
    formLayout->addRow(tr("&QR-Code:"), mQrCodeLineEdit);
    formLayout->addRow(tr("&Schlüsselhaken:"), mKeyPositionLineEdit);
    formLayout->addRow(tr("&Status:"), mKeyStatusLineEdit);
    formLayout->addRow(tr("&Schlüssel Zusatzinfo:"), mKeyInfoLineEdit);
    formLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

    filters->setLayout(formLayout);

    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->addWidget(mDataView, Qt::AlignCenter);
    centralLayout->addWidget(filters);
    centralLayout->addWidget(resetFilters);
    setCentralLayout(centralLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    setMenuButtons(menuButtons);

    //signals+slots
    connect (mPropertyLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onPropertyFilterSet(QString)));
    connect (mQrCodeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onKeycodeFilterSet(QString)));
    connect (mKeyPositionLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onKeyPositionFilterSet(QString)));
    connect (mKeyStatusLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onKeyStatusFilterSet(QString)));
    connect (mKeyInfoLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onKeyInfoFilterSet(QString)));
    connect (resetFilters, SIGNAL(clicked()), this, SLOT(onResetFiltersCLicked()));
}

void SearchView::showEvent(QShowEvent *)
{
    resetFilters();

    //as suggested in QTreeView help, sorting should be enabled after inserting tree data
    mDataView->setSortingEnabled(false);

    //clear table
    mStandardModel->clear();

    //clear also removed the header labels
    QStringList headerLabels;
    headerLabels << "Liegenschaft" << "Schlüsselcode" << "Schlüsselhaken" << "Status" << "Schlüsseltyp" << "Schlüsselstatus" << "Schlüssel\nZusatzinfo";
    mStandardModel->setHorizontalHeaderLabels(headerLabels);

    //set data from db
    //setTreeData();
    setTreeAddressData();
    resizeTreeColumnsToContent();

    //as suggested in QTreeView help, sorting should be enabled after inserting tree data
    mDataView->setSortingEnabled(true);
}

void SearchView::onPropertyFilterSet(QString txt)
{
    mFilterProperty->setFilterRegularExpression(QRegularExpression(txt, QRegularExpression::CaseInsensitiveOption));
    mFilterProperty->setFilterKeyColumn(0);
}

void SearchView::onKeycodeFilterSet(QString txt)
{
    mFilterQrCode->setFilterRegularExpression(QRegularExpression(txt, QRegularExpression::CaseInsensitiveOption));
    mFilterQrCode->setFilterKeyColumn(1);
}

void SearchView::onKeyPositionFilterSet(QString txt)
{
    mFilterKeyPosition->setFilterRegularExpression(QRegularExpression(txt, QRegularExpression::CaseInsensitiveOption));
    mFilterKeyPosition->setFilterKeyColumn(2);
}

void SearchView::onKeyStatusFilterSet(QString txt)
{
    mFilterStatus->setFilterRegularExpression(QRegularExpression(txt, QRegularExpression::CaseInsensitiveOption));
    mFilterStatus->setFilterKeyColumn(3);
}

void SearchView::onKeyInfoFilterSet(QString txt)
{
    mFilterInfo->setFilterRegularExpression(QRegularExpression(txt, QRegularExpression::CaseInsensitiveOption));
    mFilterInfo->setFilterKeyColumn(6);
}

void SearchView::onResetFiltersCLicked()
{
    resetFilters();
}

void SearchView::resetFilters()
{
    mPropertyLineEdit->setText("");
    mQrCodeLineEdit->setText("");
    mKeyPositionLineEdit->setText("");
    mKeyStatusLineEdit->setText("");
    mKeyInfoLineEdit->setText("");
}

void SearchView::setTreeAddressData()
{
    // get all address Ids
    QList addressIds = ioInterface()->getTableColumn ("keyAddresses", "id");

    if (0 < addressIds.size())
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

        unsigned int keyCatId = 0;
        unsigned int keyStatusId = 0;
        QString keyCatString ("");
        QString keyStatusString ("");
        QString keyDescription ("");
        unsigned int currentKeyId = 0;

        //add addresses (root items)
        for (int i = 0; i < addressIds.size(); i++)
        {
            addressStreet = ioInterface()->getAddressStreet(addressIds.at(i).toUInt());
            addressStreetNr = ioInterface()->getAddressStreetNumber(addressIds.at(i).toUInt());
            addressAreaCode = QString::number(ioInterface()->getAddressAreaCode(addressIds.at(i).toUInt()));
            addressCity = ioInterface()->getAddressCity(addressIds.at(i).toUInt());

            QStandardItem *item = mStandardModel->invisibleRootItem();
            // adding a row to the invisible root item produces a root element
            //item->appendRow(rowItems);

            //add keychains to address, as child items
            QString filter = "addressId = ";
            filter.append(addressIds.at(i).toString());
            QList keychainsCurrentAddress = ioInterface()->getTableColumn ("keychains", "id", filter);
            for (int j=0; j < keychainsCurrentAddress.size(); j++)
            {
                QList<QStandardItem*> rowItems;
                rowItems << new QStandardItem(addressStreet + " " + addressStreetNr + ", " + addressAreaCode + " " + addressCity);

                //add a nice key icon
                QPixmap logo (":/images/keyRing.png");
                QIcon iconPic(logo);
                QStandardItem *keyIcon = new QStandardItem();
                keyIcon->setIcon(iconPic);
                unsigned int keyCode = keychainsCurrentAddress.at(j).toUInt();
                QString keyCodeReadable = Database::normaliseKeycode(keyCode);
                unsigned int keyChainStatusId = ioInterface()->getKeychainStatusId(keyCode);
                QString keyChainStatusString = ioInterface()->getKeychainStatusText(keyChainStatusId);
                QString internalLocation = QString::number(ioInterface()->getKeychainInternalLocation(keyCode));

                QList<QStandardItem*> childItems;
                childItems << rowItems;
                childItems << new QStandardItem(keyCodeReadable);
                childItems << new QStandardItem(internalLocation);
                childItems << new QStandardItem(keyChainStatusString);
                //rowItems.first()->appendRow(childItems);
                item->appendRow(childItems);

                // add keys to keychain, as child items
                QList keys = ioInterface()->getKeyIdsByKeycode(keyCode);
                for (unsigned k = 0; k < keys.size(); k++)
                {
                    currentKeyId = keys.at(k).toUInt();
                    qDebug() << "current key id: " << currentKeyId;
                    keyCatId = ioInterface()->getKeyCategoryId(currentKeyId);
                    keyCatString = ioInterface()->getKeyCategoryString(keyCatId);
                    keyStatusId = ioInterface()->getKeyStatusId(currentKeyId);
                    keyStatusString = ioInterface()->getKeyStatusString(keyStatusId);
                    keyDescription = ioInterface()->getKeyDescription(currentKeyId);

                    //a nice key icon
                    QPixmap logo (":/images/key.png");
                    QIcon iconPic(logo);
                    QStandardItem *keyIcon = new QStandardItem();
                    keyIcon->setIcon(iconPic);
                    keyIcon->setText(keyCatString);

                    QList<QStandardItem*> childItems2;
                    childItems2 << new QStandardItem();
                    childItems2 << new QStandardItem();
                    childItems2 << new QStandardItem();
                    childItems2 << new QStandardItem();
                    childItems2 << keyIcon;
                    childItems2 << new QStandardItem(keyStatusString);
                    childItems2 << new QStandardItem(keyDescription);

                    childItems.first()->appendRow(childItems2);
                }
            }
        }
    }
}

void SearchView::setTreeData()
{
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

        unsigned int keyCatId = 0;
        unsigned int keyStatusId = 0;
        QString keyCatString ("");
        QString keyStatusString ("");
        QString keyDescription ("");
        unsigned int currentKeyId = 0;

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

            //add child items for each row (keys)
            QList keys = ioInterface()->getKeyIdsByKeycode(id);

            for (unsigned j = 0; j < keys.size(); j++)
            {
                currentKeyId = keys.at(j).toUInt();
                qDebug() << "current key id: " << currentKeyId;
                keyCatId = ioInterface()->getKeyCategoryId(currentKeyId);
                keyCatString = ioInterface()->getKeyCategoryString(keyCatId);
                keyStatusId = ioInterface()->getKeyStatusId(currentKeyId);
                keyStatusString = ioInterface()->getKeyStatusString(keyStatusId);
                keyDescription = ioInterface()->getKeyDescription(currentKeyId);

                //a nice key icon
                QPixmap logo (":/images/key.png");
                QIcon iconPic(logo);
                QStandardItem *keyIcon = new QStandardItem();
                keyIcon->setIcon(iconPic);

                QList<QStandardItem*> childItems;
                childItems << keyIcon;
                childItems << new QStandardItem("Typ: " + keyCatString);
                childItems << new QStandardItem("Status: " + keyStatusString);
                childItems << new QStandardItem("Infos: " + keyDescription);

                rowItems.first()->appendRow(childItems);
            }
        }
    }
}

void SearchView::resizeTreeColumnsToContent()
{
    for (int i = 0; i < mDataView->model()->columnCount(); i++)
        mDataView->resizeColumnToContents(i);
}

SearchView::~SearchView()
{
    if (0 != mStandardModel)
        delete mStandardModel;
}

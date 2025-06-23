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
#include <QItemSelection>
#include <QTableWidget>
#include "iointerfacesqlite.h"
#include "qheaderview.h"
#include "keychainstatusview.h"
#include "datainterface.h"

SearchView::SearchView (QWidget *parent)
    : WinSubmenu {parent}
{
    mKeychainStatusView = 0;
    mSelectedKeycode = 0;

    setHeader("Suche");

    // mDataTable = new QTableWidget(this);
    // QStringList headerLabels;
    // headerLabels << "Liegenschaft" << "Schlüsselcode" << "Schlüsselhaken" << "Status" << "Schlüsseltyp" << "Schlüsselstatus" << "Schlüssel\nZusatzinfo";
    // mDataTable->setColumnCount(7);
    // mDataTable->setHorizontalHeaderLabels(headerLabels);
    // mDataTable->setSortingEnabled(true);
    // mDataTable->setWordWrap(true);
    // mDataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    menuButtons.append(Gui::Edit);
    setMenuButtons(menuButtons);
    disableButton(1, true);

    //signals+slots
    connect (mPropertyLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onPropertyFilterSet(QString)));
    connect (mQrCodeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onKeycodeFilterSet(QString)));
    connect (mKeyPositionLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onKeyPositionFilterSet(QString)));
    connect (mKeyStatusLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onKeyStatusFilterSet(QString)));
    connect (mKeyInfoLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onKeyInfoFilterSet(QString)));
    connect (resetFilters, SIGNAL(clicked()), this, SLOT(onResetFiltersCLicked()));
    connect (mDataView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SearchView::onTreeViewSelectionChanged);
}

void SearchView::showEvent(QShowEvent *)
{
    resetFilters();

    //setData();

    //set data from db
    //setTreeData();
    setTreeData();
}

void SearchView::onMenuBtnClicked (Gui::MenuButton btnType)
{
    switch (btnType)
    {
        case Gui::Edit:
            showKeychainStatusView();
            break;
        default:
            emit menuButtonClicked(btnType);
    }
}

void SearchView::onKeychainStatusMenuButtonClicked (Gui::MenuButton btnType)
{
    switch (btnType)
    {
    case Gui::Back:
        if (mKeychainStatusView)
        {
            mKeychainStatusView->hide();
            delete mKeychainStatusView;
            mKeychainStatusView = 0;
        }
        break;
    default:
        // do nothing
        break;
    }
}

void SearchView::onTableItemClicked(QTableWidgetItem *item)
{
    if (item)
    {

    }
}

void SearchView::onTreeViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (!selected.empty())
    {
        disableButton(1, false);

        QModelIndex currentIndex = selected.indexes().at(1);
        QVariant data = mDataView->model()->data(currentIndex);
        QString keyCode = data.toString();

        //convert keycode to integer
        QString barcodeAsNumber = keyCode.mid (0, 4);
        barcodeAsNumber.append(keyCode.mid(5, 4));
        mSelectedKeycode = barcodeAsNumber.toUInt();
    }
    else
        mSelectedKeycode = 0;
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

void SearchView::setData()
{
    // //clear table contents
    // mDataTable->clearContents();

    // // get number of rows and set row count to data table
    // QList keys = ioInterface()->getTableColumn("keys", "id");
    // mDataTable->setRowCount(keys.size());

    // // get all address Ids
    // QList addressIds = ioInterface()->getTableColumn ("keyAddresses", "id");

    // if (0 < addressIds.size())
    // {
    //     unsigned int id = 0;
    //     KeychainStatus::Value status = KeychainStatus::Undefined;
    //     unsigned int internalLoc = 0;
    //     unsigned int addressId = 0;

    //     QString addressStreet ("");
    //     QString addressStreetNr ("");
    //     QString addressAreaCode ("");
    //     QString addressCity ("");

    //     QString statusText ("");
    //     QString readableId ("");
    //     QString internalLocString ("");

    //     unsigned int keyCatId = 0;
    //     unsigned int keyStatusId = 0;
    //     QString keyCatString ("");
    //     QString keyStatusString ("");
    //     QString keyDescription ("");
    //     unsigned int currentKeyId = 0;

    //     unsigned int tableIndex = 0;

    //     //add rows
    //     for (int i = 0; i < addressIds.size(); i++)
    //     {
    //         addressStreet = ioInterface()->getAddressStreet(addressIds.at(i).toUInt());
    //         addressStreetNr = ioInterface()->getAddressStreetNumber(addressIds.at(i).toUInt());
    //         addressAreaCode = QString::number(ioInterface()->getAddressAreaCode(addressIds.at(i).toUInt()));
    //         addressCity = ioInterface()->getAddressCity(addressIds.at(i).toUInt());

    //         //add keychains to address, as child items
    //         QString filter = "addressId = ";
    //         filter.append(addressIds.at(i).toString());
    //         QList keychainsCurrentAddress = ioInterface()->getTableColumn ("keychains", "id", filter);

    //         for (int j=0; j < keychainsCurrentAddress.size(); j++)
    //         {
    //             //headerLabels << "Liegenschaft" << "Schlüsselcode" << "Schlüsselhaken" << "Status" << "Schlüsseltyp" << "Schlüsselstatus" << "Schlüssel\nZusatzinfo";

    //             unsigned int keyCode = keychainsCurrentAddress.at(j).toUInt();
    //             QString keyCodeReadable = Database::normaliseKeycode(keyCode);
    //             unsigned int keyChainStatusId = ioInterface()->getKeychainStatusId(keyCode);
    //             QString keyChainStatusString = ioInterface()->getKeychainStatusText(keyChainStatusId);
    //             QString internalLocation = QString::number(ioInterface()->getKeychainInternalLocation(keyCode));
    //             while (internalLocation.count() < 4)
    //             {
    //                 internalLocation.prepend("0");
    //             }

    //             // iterate through keys
    //             QList keys = ioInterface()->getKeyIdsByKeycode(keyCode);
    //             for (unsigned k = 0; k < keys.size(); k++)
    //             {
    //                 currentKeyId = keys.at(k).toUInt();
    //                 qDebug() << "current key id: " << currentKeyId;
    //                 keyCatId = ioInterface()->getKeyCategoryId(currentKeyId);
    //                 keyCatString = ioInterface()->getKeyCategoryString(keyCatId);
    //                 keyStatusId = ioInterface()->getKeyStatusId(currentKeyId);
    //                 keyStatusString = ioInterface()->getKeyStatusString(keyStatusId);
    //                 keyDescription = ioInterface()->getKeyDescription(currentKeyId);

    //                 QTableWidgetItem *address = new QTableWidgetItem (addressStreet + " " + addressStreetNr + ", " + addressAreaCode + " " + addressCity);
    //                 QTableWidgetItem *keyCode = new QTableWidgetItem (keyCodeReadable);
    //                 QTableWidgetItem *location = new QTableWidgetItem (internalLocation);
    //                 QTableWidgetItem *keychainStatus = new QTableWidgetItem (keyChainStatusString);
    //                 QTableWidgetItem *keyType = new QTableWidgetItem (keyCatString);
    //                 QTableWidgetItem *keyStatus = new QTableWidgetItem (keyStatusString);
    //                 QTableWidgetItem *keyAdditionalInfo = new QTableWidgetItem (keyDescription);

    //                 mDataTable->setItem(tableIndex, 0, address);
    //                 mDataTable->setItem(tableIndex, 1, keyCode);
    //                 mDataTable->setItem(tableIndex, 2, location);
    //                 mDataTable->setItem(tableIndex, 3, keychainStatus);
    //                 mDataTable->setItem(tableIndex, 4, keyType);
    //                 mDataTable->setItem(tableIndex, 5, keyStatus);
    //                 mDataTable->setItem(tableIndex, 6, keyAdditionalInfo);

    //                 tableIndex++;
    //             }
    //             //tableIndex++;
    //         }
    //     }
    // }
    // mDataTable->resizeColumnsToContents();
    // mDataTable->resizeRowsToContents();
    // mDataTable->horizontalHeader()->setStretchLastSection(true);
}

void SearchView::resetFilters()
{
    mPropertyLineEdit->setText("");
    mQrCodeLineEdit->setText("");
    mKeyPositionLineEdit->setText("");
    mKeyStatusLineEdit->setText("");
    mKeyInfoLineEdit->setText("");
}

void SearchView::setTreeData()
{
    //as suggested in QTreeView help, sorting should be enabled after inserting tree data
    mDataView->setSortingEnabled(false);

    //clear table
    mStandardModel->clear();

    //clear also removed the header labels
    QStringList headerLabels;
    headerLabels << "Liegenschaft" << "Schlüsselcode" << "Schlüsselhaken" << "Status" << "Schlüsseltyp" << "Schlüsselstatus" << "Schlüssel\nZusatzinfo";
    mStandardModel->setHorizontalHeaderLabels(headerLabels);

    // get all address Ids
    QList addressIds = ioInterface()->getTableColumn ("keyAddresses", "id");

    if (0 < addressIds.size())
    {
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

        //unsigned int tableIndex = 0;

        //add rows
        for (int i = 0; i < addressIds.size(); i++)
        {
            addressStreet = ioInterface()->getAddressStreet(addressIds.at(i).toUInt());
            addressStreetNr = ioInterface()->getAddressStreetNumber(addressIds.at(i).toUInt());
            addressAreaCode = QString::number(ioInterface()->getAddressAreaCode(addressIds.at(i).toUInt()));
            addressCity = ioInterface()->getAddressCity(addressIds.at(i).toUInt());

            //add keychains to address, as child items
            QString filter = "addressId = ";
            filter.append(addressIds.at(i).toString());
            QList keychainsCurrentAddress = ioInterface()->getTableColumn ("keychains", "id", filter);

            QStandardItem *item = mStandardModel->invisibleRootItem();

            for (int j=0; j < keychainsCurrentAddress.size(); j++)
            {
                unsigned int keyCode = keychainsCurrentAddress.at(j).toUInt();
                QString keyCodeReadable = Database::normaliseKeycode(keyCode);
                unsigned int keyChainStatusId = ioInterface()->getKeychainStatusId(keyCode);
                QString keyChainStatusString = ioInterface()->getKeychainStatusText(keyChainStatusId);
                QString internalLocation = QString::number(ioInterface()->getKeychainInternalLocation(keyCode));
                while (internalLocation.size() < 4)
                {
                    internalLocation.prepend("0");
                }

                // iterate through keys
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

                    QStandardItem *address = new QStandardItem (addressStreet + " " + addressStreetNr + ", " + addressAreaCode + " " + addressCity);
                    QStandardItem *keyCode = new QStandardItem (keyCodeReadable);
                    QStandardItem *location = new QStandardItem (internalLocation);
                    QStandardItem *keychainStatus = new QStandardItem (keyChainStatusString);
                    QStandardItem *keyType = new QStandardItem (keyCatString);
                    QStandardItem *keyStatus = new QStandardItem (keyStatusString);
                    QStandardItem *keyAdditionalInfo = new QStandardItem (keyDescription);

                    QList<QStandardItem*> childItems;
                    childItems << address;
                    childItems << keyCode;
                    childItems << location;
                    childItems << keychainStatus;
                    childItems << keyType;
                    childItems << keyStatus;
                    childItems << keyAdditionalInfo;

                    item->appendRow(childItems);

                    //tableIndex++;
                }
            }
        }
    }

    resizeTreeColumnsToContent();

    //as suggested in QTreeView help, sorting should be enabled after inserting tree data
    mDataView->setSortingEnabled(true);
}

void SearchView::resizeTreeColumnsToContent()
{
    if (mDataView)
    {
        for (int i = 0; i < mDataView->model()->columnCount(); i++)
        {
            mDataView->resizeColumnToContents(i);
        }
    }
}

void SearchView::showKeychainStatusView()
{
    if (0 != mSelectedKeycode)
    {
        dataInterface()->setScannedCode (mSelectedKeycode);

        if (!mKeychainStatusView)
        {
            mKeychainStatusView = new KeychainStatusView();
            connect (mKeychainStatusView, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT(onKeychainStatusMenuButtonClicked(Gui::MenuButton)));
        }
        mKeychainStatusView->setDataInterface(dataInterface());
        mKeychainStatusView->setIOInterface(ioInterface());
        mKeychainStatusView->setViewMode(ViewMode::ShowData);
        mKeychainStatusView->show();
    }
}

SearchView::~SearchView()
{
    if (0 != mKeychainStatusView)
    {
        delete mKeychainStatusView;
        mKeychainStatusView = 0;
    }


    /*if (0 != mStandardModel)
        delete mStandardModel;*/
}

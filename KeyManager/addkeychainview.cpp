#include "AddKeychainView.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTableView>
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QSqlRelationalTableModel>
#include <QItemSelection>
#include <QLineEdit>
#include <QMessageBox>
#include <QCalendarWidget>
#include <QCheckBox>
#include <QButtonGroup>
#include <QDate>
#include <QGroupBox>

#include "winsubmenu.h"
#include "globals.h"
//#include "dataobject.h"
//#include "dataobjecthandover.h"
#include "datainterface.h"
#include "iointerface.h"
#include "returndateview.h"
#include "addcustomerview.h"

AddKeychainView::AddKeychainView(QWidget *parent) : WinSubmenu {parent}
{
    mCustomersView = 0;
    mFilteredModel = 0;
    mCustomersModel = 0;
    mRowSelected = false;
    mAddCustomerView = 0;
    //mViewDataKeychain = 0;

    mFilteredModel = new QSortFilterProxyModel (this);

    setHeader("Schlüsselbund anlegen");

    //QVBoxLayout* layout = new QVBoxLayout (this);

    QHBoxLayout* searchLayout = new QHBoxLayout ();
    mSearchLabel = new QLabel ("Suche", this);
    mSearchField = new QLineEdit (this);
    searchLayout->addWidget(mSearchLabel);
    searchLayout->addWidget(mSearchField);
    layout()->addItem(searchLayout);

    mCustomersView = new QTableView (this);
    mCustomersView->setModel(mFilteredModel);
    layout()->addWidget(mCustomersView);

    //mCustomersView->clearSelection();
    mCustomersView->update();
    mCustomersView->setFocus();

    QLabel *internalLocLabel = new QLabel ("Schlüsselhaken zuweisen:", this);
    mInternalLocation = new QLineEdit (this);
    mInternalLocation->clear();
    mInternalLocation->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d\\d\\d\\d\\d"), mInternalLocation));
    QPushButton *btnLocProposal = new QPushButton("Freien Schlüsselhaken\n vorschlagen", this);

    QHBoxLayout *btnLayoutLoc = new QHBoxLayout ();
    btnLayoutLoc->addWidget(internalLocLabel);
    btnLayoutLoc->addWidget(mInternalLocation);
    btnLayoutLoc->addWidget(btnLocProposal);

    layout()->addItem(btnLayoutLoc);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout()->addItem(spacer);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::AddCustomer);
    menuButtons.append(Gui::Next);

    setMenuButtons(menuButtons);
    setButtonText(2, "Schlüssel hinzufügen");

    connect (mCustomersView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onTableSelectionChanged(const QItemSelection &, const QItemSelection &)));
    connect (mSearchField, SIGNAL(textChanged(const QString &)), this, SLOT(setTableFilter(const QString &)));
    connect (btnLocProposal, SIGNAL(clicked ()), this, SLOT(onButtonProposeLocationClicked ()));
}

bool AddKeychainView::setModel (QSqlRelationalTableModel* model)
{
    if (!model)
        return false;

    if (!mCustomersView)
        return false;

    if (!mFilteredModel)
        return false;

    model->setHeaderData(0, Qt::Horizontal, tr("ID"), Qt::DisplayRole);
    model->setHeaderData(1, Qt::Horizontal, tr("Straße"), Qt::DisplayRole);
    model->setHeaderData(2, Qt::Horizontal, tr("Hausnummer"), Qt::DisplayRole);
    model->setHeaderData(3, Qt::Horizontal, tr("PLZ"), Qt::DisplayRole);
    model->setHeaderData(4, Qt::Horizontal, tr("Ort"), Qt::DisplayRole);

    mFilteredModel->setSourceModel(model);
    mFilteredModel->setFilterCaseSensitivity(Qt::CaseInsensitive); // ignore capital letters

    mCustomersView->hideColumn(0); //don't show table index

    mCustomersView->setSortingEnabled(true);
    mCustomersView->sortByColumn(1, Qt::AscendingOrder);
    mCustomersView->setEditTriggers(QTableView::NoEditTriggers);
    mCustomersView->setSelectionMode(QTableView::SingleSelection);
    mCustomersView->setSelectionBehavior(QTableView::SelectRows);

    mCustomersView->show();

    mCustomersView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mCustomersView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    mCustomersView->resizeColumnsToContents();

    update ();

    return true;
}

void AddKeychainView::reset()
{
    dataInterface()->resetKeychainData();

    //init model/view at first show event
    if (!mCustomersModel)
        mCustomersModel = new QSqlRelationalTableModel (this);

    if (mCustomersModel)
    {
        ioInterface()->initCustomerModel(mCustomersModel);
        setModel(mCustomersModel);
    }

    mCustomersView->clearSelection();
    mCustomersView->setFocus();
    mCustomersView->update();
    mRowSelected = false;
    mInternalLocation->clear();
}

void AddKeychainView::showEvent(QShowEvent *)
{
    reset ();
}

void AddKeychainView::onMenuBtnClicked (Gui::MenuButton btnType)
{
    QMessageBox msgBox;
    bool foundKeyCode = ioInterface()->findKeyCode(dataInterface()->getScannedCode());

    switch (btnType)
    {
        case (Gui::Next):
            if (true == foundKeyCode)
            {
                msgBox.setStandardButtons(QMessageBox::Abort);
                msgBox.setDefaultButton(QMessageBox::Abort);
                msgBox.setText ("Fehler!");
                msgBox.setInformativeText("AddKeychainView::onMenuBtnClicked - keycode duplicate!");
                msgBox.exec();

                return;
            }

            if ("" == mInternalLocation->text())
            {
                msgBox.setStandardButtons(QMessageBox::Abort);
                msgBox.setDefaultButton(QMessageBox::Abort);
                msgBox.setText ("Fehler!");
                msgBox.setInformativeText("Schlüsselhaken nicht gesetzt!");
                msgBox.exec();
                return;
            }

            if (!mRowSelected)
            {
                msgBox.setStandardButtons(QMessageBox::Abort);
                msgBox.setDefaultButton(QMessageBox::Abort);
                msgBox.setText ("Fehler!");
                msgBox.setInformativeText("Es wurde keine Liegenschaft ausgewählt!");
                msgBox.exec();
                return;
            }

            if (0 != ioInterface()->getKeychainInternalLocation(dataInterface()->getScannedCode()))
            {
                QString text = "Schlüsselhaken ist bereits belegt!\n";
                msgBox.setStandardButtons(QMessageBox::Abort);
                msgBox.setDefaultButton(QMessageBox::Abort);
                msgBox.setText ("Fehler!");
                msgBox.setInformativeText(text);
                msgBox.exec();
                return;
            }
            else
            {
                if (ConfirmationDialog ())
                {
                    dataInterface()->setNewKeychainStatusId(Database::KeychainStatus::Available);
                    dataInterface()->setKeychainInternalLocation(mInternalLocation->text().toInt());

                    //mViewDataKeychain->setNewStatus(Database::KeychainStatus::Available);
                    //mViewDataKeychain->setInternalLocation(mInternalLocation->text().toInt());

                    bool retVal = ioInterface()->dbInsertKeychain (dataInterface()->getHandle());
                    if (!retVal)
                    {
                        QString text = "Fehler!";
                        msgBox.setStandardButtons(QMessageBox::Abort);
                        msgBox.setDefaultButton(QMessageBox::Abort);
                        msgBox.setText ("AddKeychainView::onMenuBtnClicked -> submitNewKeychain () - SQL Fehler!");
                        msgBox.setInformativeText(text);
                        msgBox.exec();
                        break;
                    }
                }
                else
                    return;
            }
            break;
        case Gui::AddCustomer:
            if (!mAddCustomerView)
            {
                mAddCustomerView = new AddCustomerView();
                mAddCustomerView->setIOInterface(ioInterface());
                connect (mAddCustomerView, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT(onAddCustomerButtonClicked(Gui::MenuButton)));
            }
            mAddCustomerView->show();
            break;
        default:
            break;
    }

    emit menuButtonClicked(btnType);
}

void AddKeychainView::setTableFilter(const QString &text)
{
    if (!mCustomersView)
        return;

    mFilteredModel->setFilterKeyColumn(-1); //set filter to search in all columns
    mFilteredModel->setFilterWildcard(text);
    mCustomersView->update();
    update ();
}

void AddKeychainView::setTableFilter(const int column, const QString &searchString)
{
    if (!mCustomersView)
        return;

    mFilteredModel->setFilterKeyColumn(column);
    mFilteredModel->setFilterWildcard(searchString);

    qDebug () << "AddKeychainView::setTableFilter";
    qDebug () << "column: " << column;
    qDebug () << "searchString: " << searchString;

    mCustomersView->update();
    update();
}

bool AddKeychainView::ConfirmationDialog ()
{
    QMessageBox msgBox;
    msgBox.setStandardButtons(QMessageBox::Abort | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setText ("Datenbankeintrag hinzufügen");
    msgBox.setInformativeText("Neuen Schlüsselbund in Datenbank anlegen?");

    bool ok = false;
    int selection = msgBox.exec();

    switch (selection)
    {
    case QMessageBox::Abort:
        ok = false;
        break;
    case QMessageBox::Ok:
        ok = true;
        break;
    default:
        break;
    }
    return ok;
}

void AddKeychainView::onTableSelectionChanged (const QItemSelection &itemNew, const QItemSelection &itemOld)
{
    Q_UNUSED(itemOld);
    Q_UNUSED(itemNew);

    // if (!mViewDataKeychain)
    //     return;

    if (!mCustomersView)
        return;

    int row = mCustomersView->selectionModel()->currentIndex().row();
    mRowSelected = true;

    dataInterface()->setKeychainAddressId(mCustomersView->model()->index(row, 0).data().toInt ());
    //mViewDataKeychain->setAddressId(mCustomersView->model()->index(row, 0).data().toInt ());

    qDebug () << "dataInterface()->getScannedCode(): " <<  dataInterface()->getScannedCode();

    //mViewDataKeychain->setAddressId()
    // mViewDataKeychain->setRecipientName(mCustomersView->model()->index(row, 1).data().toString ());
    // QString recipientType = mCustomersView->model()->index(row, 2).data().toString ();
    // mViewDataKeychain->setRecipientType(recipientType);
    // mViewDataKeychain->setRecipientStreet(mCustomersView->model()->index(row, 3).data().toString ());
    // mViewDataKeychain->setRecipientStreetNumber(mCustomersView->model()->index(row, 4).data().toInt ());
    // mViewDataKeychain->setRecipientAreaCode(mCustomersView->model()->index(row, 5).data().toInt ());
    // mViewDataKeychain->setRecipientCity(mCustomersView->model()->index(row, 6).data().toString());

    update();
}

void AddKeychainView::onButtonProposeLocationClicked ()
{
    mInternalLocation->setText (QString::number(ioInterface()->getFreeInternalLocation (1)));
    mInternalLocation->update();
}

void AddKeychainView::onAddCustomerButtonClicked(Gui::MenuButton btn)
{
    switch (btn)
    {
    case Gui::Back:
    case Gui::Ok:
        if (mAddCustomerView)
        {
            mAddCustomerView->hide();
            delete mAddCustomerView;
            mAddCustomerView = 0;
            reset();
            setFocus();
            mCustomersView->update();
            update();
        }
        break;
    default:
        break;
    }
}

AddKeychainView::~AddKeychainView()
{
    if (mAddCustomerView)
    {
        delete mAddCustomerView;
        mAddCustomerView = 0;
    }
}

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
#include "dataobject.h"
#include "dataobjecthandover.h"
#include "datainterface.h"
#include "iointerface.h"
#include "returndateview.h"
#include "viewdatakeychain.h"

AddKeychainView::AddKeychainView(QWidget *parent) : WinSubmenu {parent}
{
    mBuildings = 0;
    mFilteredModel = 0;
    mBuildingsModel = 0;
    mRowSelected = false;
    mViewDataKeychain = 0;

    mFilteredModel = new QSortFilterProxyModel (this);

    setHeader("Schlüsselbund anlegen");

    //QVBoxLayout* layout = new QVBoxLayout (this);

    QHBoxLayout* searchLayout = new QHBoxLayout ();
    mSearchLabel = new QLabel ("Suche", this);
    mSearchField = new QLineEdit (this);
    searchLayout->addWidget(mSearchLabel);
    searchLayout->addWidget(mSearchField);
    layout()->addItem(searchLayout);

    mBuildings = new QTableView (this);
    mBuildings->setModel(mFilteredModel);
    layout()->addWidget(mBuildings);

    //mBuildings->clearSelection();
    mBuildings->update();
    mBuildings->setFocus();

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

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::AddKey);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);
    setButtonText(2, "Schlüssel hinzufügen");

    //setMenuButtons(Gui::Back, Gui::AddRecipient, Gui::Next);
    //disableButton(2, true);

    connect (mBuildings->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onTableSelectionChanged(const QItemSelection &, const QItemSelection &)));
    connect (mSearchField, SIGNAL(textChanged(const QString &)), this, SLOT(setTableFilter(const QString &)));
    connect (btnLocProposal, SIGNAL(clicked ()), this, SLOT(onButtonProposeLocationClicked ()));
}

bool AddKeychainView::setModel (QSqlRelationalTableModel* model)
{
    if (!model)
        return false;

    if (!mBuildings)
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

    mBuildings->hideColumn(0); //don't show table index

    mBuildings->setSortingEnabled(true);
    mBuildings->sortByColumn(1, Qt::AscendingOrder);
    mBuildings->setEditTriggers(QTableView::NoEditTriggers);
    mBuildings->setSelectionMode(QTableView::SingleSelection);
    mBuildings->setSelectionBehavior(QTableView::SelectRows);

    mBuildings->show();

    mBuildings->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mBuildings->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    mBuildings->resizeColumnsToContents();

    update ();

    return true;
}

void AddKeychainView::reset()
{
    if (mViewDataKeychain)
    {
        delete mViewDataKeychain;
        mViewDataKeychain = nullptr;
    }

    mViewDataKeychain = new ViewDataKeychain ();
    dataInterface()->setData(mViewDataKeychain);

    //init model/view at first show event
    if (!mBuildingsModel)
    {
        mBuildingsModel = new QSqlRelationalTableModel ();
        ioInterface()->initBuildingModel(mBuildingsModel);
        setModel(mBuildingsModel);
    }

    mBuildings->clearSelection();
    mBuildings->setFocus();
    mBuildings->update();
    mRowSelected = false;
    mInternalLocation->clear();
}

void AddKeychainView::showEvent(QShowEvent *)
{
    reset ();
}

void AddKeychainView::onMenuBtnClicked (Gui::MenuButton btnType)
{
    switch (btnType)
    {
        case (Gui::Next):

            if ("" == mInternalLocation->text())
            {
                QMessageBox msgBox;
                msgBox.setStandardButtons(QMessageBox::Abort);
                msgBox.setDefaultButton(QMessageBox::Abort);
                msgBox.setText ("Fehler!");
                msgBox.setInformativeText("Schlüsselhaken nicht gesetzt!");
                msgBox.exec();
                return;
            }

            if (!mRowSelected)
            {
                QMessageBox msgBox;
                msgBox.setStandardButtons(QMessageBox::Abort);
                msgBox.setDefaultButton(QMessageBox::Abort);
                msgBox.setText ("Fehler!");
                msgBox.setInformativeText("Es wurde keine Liegenschaft ausgewählt!");
                msgBox.exec();
                return;
            }

            int retVal = ioInterface()->findInternalLocation (mInternalLocation->text().toInt());

            if (_UNDEFINED == retVal)
            {
                QMessageBox msgBox;
                msgBox.setStandardButtons(QMessageBox::Abort);
                msgBox.setDefaultButton(QMessageBox::Abort);
                msgBox.setText ("Fehler!");
                msgBox.setInformativeText("AddKeychainView::onMenuBtnClicked - findInternalLocation () - SQLQuery failed!");
                msgBox.exec();
                break;
            }
            else if (0 != retVal)
            {
                QString text = "Schlüsselhaken ist bereits belegt mit folgendem Schlüsselbund:\n";
                text.append ("Barcode: ");
                text.append (QString::number(retVal));
                QMessageBox msgBox;
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
                    mViewDataKeychain->setNewStatus(Database::KeychainStatus::Available);
                    mViewDataKeychain->setInternalLocation(mInternalLocation->text().toInt());

                    bool retVal = ioInterface()->dbInsertKeychain (dataInterface()->getHandle());
                    if (!retVal)
                    {
                        QString text = "Fehler!";
                        QMessageBox msgBox;
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
        // default:
        //     break;
    }

    emit menuButtonClicked(btnType);
}

void AddKeychainView::setTableFilter(const QString &text)
{
    if (!mBuildings)
        return;

    mFilteredModel->setFilterKeyColumn(-1); //set filter to search in all columns
    mFilteredModel->setFilterWildcard(text);
    mBuildings->update();
    update ();
}

void AddKeychainView::setTableFilter(const int column, const QString &searchString)
{
    if (!mBuildings)
        return;

    mFilteredModel->setFilterKeyColumn(column);
    mFilteredModel->setFilterWildcard(searchString);

    qDebug () << "AddKeychainView::setTableFilter";
    qDebug () << "column: " << column;
    qDebug () << "searchString: " << searchString;

    mBuildings->update();
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

    if (!mViewDataKeychain)
        return;

    if (!mBuildings)
        return;

    int row = mBuildings->selectionModel()->currentIndex().row();
    mRowSelected = true;

    mViewDataKeychain->setAddressId(mBuildings->model()->index(row, 0).data().toInt ());

    qDebug () << "dataInterface()->getScannedCode(): " <<  dataInterface()->getScannedCode();

    //mViewDataKeychain->setAddressId()
    // mViewDataKeychain->setRecipientName(mBuildings->model()->index(row, 1).data().toString ());
    // QString recipientType = mBuildings->model()->index(row, 2).data().toString ();
    // mViewDataKeychain->setRecipientType(recipientType);
    // mViewDataKeychain->setRecipientStreet(mBuildings->model()->index(row, 3).data().toString ());
    // mViewDataKeychain->setRecipientStreetNumber(mBuildings->model()->index(row, 4).data().toInt ());
    // mViewDataKeychain->setRecipientAreaCode(mBuildings->model()->index(row, 5).data().toInt ());
    // mViewDataKeychain->setRecipientCity(mBuildings->model()->index(row, 6).data().toString());

    update();
}

void AddKeychainView::onButtonProposeLocationClicked ()
{
    mInternalLocation->setText (QString::number(ioInterface()->getFreeInternalLocation ()));
    mInternalLocation->update();
}

AddKeychainView::~AddKeychainView()
{
}

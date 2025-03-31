#include <QSqlRelationalTableModel>
#include <QTableView>
#include "keychainstatusview.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlRelationalDelegate>
#include <QHeaderView>
#include "globals.h"
#include <QSizePolicy>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>
#include "winsubmenu.h"
#include "dataobjecthandover.h"
#include <QWidget>
#include "viewdata.h"
#include "viewdatascanner.h"
#include <QSqlRelationalTableModel>
#include "databaseimpl.h"
#include "viewdatakeychainstatus.h"

KeychainStatusView::KeychainStatusView(QWidget *parent)
    : WinSubmenu {parent}
{
    mKeys = 0;
    mKeychain = 0;
    mKeysImgPreview = 0;

    setHeader("Informationen zum Schlüsselbund");

    QLabel *keyChainHeader = new QLabel ("Schlüsselbund");
    layout()->addWidget(keyChainHeader);

    mKeychain = new QTableView ();
    layout()->addWidget(mKeychain);

    mKeysImgPreview = new QPushButton ("Bild hinzufügen...");
    mKeysImgPreview->setMinimumHeight(100);
    mKeysImgPreview->setMinimumWidth(100);
    mKeysImgPreview->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
    layout()->addWidget(mKeysImgPreview);

    QLabel *keysHeader = new QLabel ("Enthaltene Schlüssel");
    layout()->addWidget(keysHeader);

    mKeys = new QTableView;
    layout()->addWidget(mKeys);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    mKeysOverviewModel = new QSqlRelationalTableModel ();
    mKeychainModel = new QSqlRelationalTableModel ();
}

void KeychainStatusView::showEvent(QShowEvent *)
{
    int barcode = getViewData()->getDataScanner()->getBarcode();

    getDatabaseHandle()->initKeyOverviewModel(mKeysOverviewModel, barcode);
    setKeysModel(mKeysOverviewModel);

    getDatabaseHandle()->initKeychainModel(mKeychainModel, barcode);
    setKeychainModel(mKeychainModel);

    Database::eKeychainStatusId keyChainStatus = getDatabaseHandle()->getKeychainStatusId (barcode);
    setKeychainStatus(keyChainStatus);
}

void KeychainStatusView::setKeychainStatus (const int &statusId)
{
    mKeychainStatusId = statusId;

    // switch (mKeychainStatusId)
    // {
    //     case Database::AdministrationEnded:
    //     case Database::TemporaryOut:
    //     case Database::PermanentOut:
    //     case Database::Lost:
    //         setMenuButtons(Gui::Back, Gui::TakeBack);
    //         break;
    //     default:
    //         setMenuButtons(Gui::Back, Gui::Handout);
    // }
}

bool KeychainStatusView::setKeychainModel (QSqlRelationalTableModel* model)
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

        if (mKeychain)
        {
            mKeychain->setModel(model);
            mKeychain->hideColumn(7); // hide image column
            mKeychain->setEditTriggers(QTableView::NoEditTriggers);
            mKeychain->setSelectionMode(QTableView::NoSelection);
            mKeychain->verticalHeader()->hide();

            mKeychain->show();

            mKeychain->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
            mKeychain->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

            mKeychain->resizeColumnsToContents();

            //barcode must be unique
            if (1 != mKeychain->model()->rowCount())
            {
                QMessageBox::critical(0, "Datenbankfehler",
                                      "Barcode nicht eindeutig, Tabelle enthält Duplikate. \n"
                                      "Überprüfung der Datenbank notwendig. Fortfahren nicht möglich.", QMessageBox::Ok);
                //prevent user to proceed to next step
                return false;
            }

            ViewDataKeychainStatus *keychainData = new ViewDataKeychainStatus ();
            int internalLoc = mKeychain->model()->index(0, 2).data().toInt();
            keychainData->setInternalLocation(internalLoc);
            keychainData->setStatus(mKeychain->model()->index(0, 1).data().toInt());
            getViewData()->setData (keychainData);

            return true;
        }
        return false;
    }
    return false;
}

bool KeychainStatusView::setKeysModel (QSqlRelationalTableModel* model)
{
    if (model)
    {
        model->setEditStrategy(QSqlRelationalTableModel::EditStrategy::OnFieldChange);

        model->setHeaderData(2, Qt::Horizontal, tr("Kategorie"), Qt::DisplayRole);
        model->setHeaderData(3, Qt::Horizontal, tr("Zustand"), Qt::DisplayRole);
        model->setHeaderData(4, Qt::Horizontal, tr("Zusatzinformation"), Qt::DisplayRole);

        if (mKeys)
        {
            qDebug () << "KeychainStatusView::setModel QSqlRelationalTableModel";
            mKeys->setModel(model);
            mKeys->hideColumn(0); // hide table id
            mKeys->hideColumn(1); // hide barcode id
            //mKeys->setItemDelegate(new QSqlRelationalDelegate(mKeys));
            mKeys->setEditTriggers(QTableView::NoEditTriggers);
            mKeys->setSelectionMode(QTableView::NoSelection);
            mKeys->show();

            mKeys->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
            mKeys->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

            mKeys->resizeColumnsToContents();

            return true;
        }
        return false;
    }
    return false;
}

void KeychainStatusView::setKeychainImagePath (const QString& imgPath)
{
    if ("" == imgPath)
        return;

    if (mKeysImgPreview)
    {
        QIcon icon (imgPath);
        mKeysImgPreview->setIcon(icon);
        mKeysImgPreview->resize(100,100);
    }
}

KeychainStatusView::~KeychainStatusView ()
{
    if (mKeysOverviewModel)
        delete mKeysOverviewModel;

    if (mKeychainModel)
        delete mKeychainModel;
}

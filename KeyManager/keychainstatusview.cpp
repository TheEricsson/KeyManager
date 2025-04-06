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
#include <QSortFilterProxyModel>
#include "iointerface.h"
#include "winsubmenu.h"
#include <QWidget>
#include "datainterface.h"
#include "iointerface.h"
#include <QSqlRelationalTableModel>

KeychainStatusView::KeychainStatusView(QWidget *parent)
    : WinSubmenu {parent}
{
    mKeys = 0;
    mKeychain = 0;
    mKeysImgPreview = 0;
    mFilteredKeyModel = 0;
    mFilteredKeychainModel = 0;
    mViewData = 0;

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
    menuButtons.append(Gui::MainMenu);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    mKeychainModel = new QSqlRelationalTableModel ();
    mFilteredKeychainModel = new QSortFilterProxyModel ();
    mFilteredKeychainModel->setSourceModel(mKeychainModel);
    mKeychain->setModel(mFilteredKeychainModel);

    mKeyModel = new QSqlRelationalTableModel ();
    mFilteredKeyModel= new QSortFilterProxyModel ();
    mFilteredKeyModel->setSourceModel(mKeyModel);
    mKeys->setModel(mFilteredKeyModel);
}

void KeychainStatusView::showEvent(QShowEvent *)
{
    int barcode = dataInterface()->getScannedCode();

    QString filterKeyTable = "keychainId = ";
    filterKeyTable.append(QString::number(barcode));
    ioInterface()->initKeyOverviewModel(mKeyModel, filterKeyTable);
    setKeysModel(mKeyModel);
    mKeychain->update();

    QString filterKeychainTable = "barcode = ";
    filterKeychainTable.append(QString::number(barcode));
    ioInterface()->initKeychainModel(mKeychainModel, filterKeychainTable);
    setKeychainModel(mKeychainModel);
    mKeys->update ();

    // mFilteredKeychainModel->setFilterKeyColumn(0);
    // mFilteredKeychainModel->setFilterWildcard(QString::number(barcode));
    // mKeychain->update();

    // mFilteredKeyModel->setFilterKeyColumn(1);
    // //mFilteredKeyModel->setFilterWildcard(QString::number(barcode));
    // mFilteredKeyModel->setFilterWildcard("JKKSJFKSJF");
    // mKeys->update();

    update ();

    dataInterface()->resetKeychainData();

    int keyCode = dataInterface()->getScannedCode();

    QString imgPath = ioInterface()->getKeychainImgPath(keyCode);
    setKeychainImagePath (imgPath);

    //set values for keychainstatus data from db
    ioInterface()->setKeychainData(dataInterface()->getDataKeychain(), keyCode);

    setNextBtnText ();
}

void KeychainStatusView::setNextBtnText ()
{
    switch (dataInterface()->getKeychainStatusId())
    {
        case Database::AdministrationEnded:
        case Database::TemporaryOut:
        case Database::PermanentOut:
        case Database::Lost:
            setButtonText(2, "Rücknahme");
            break;
        default:
            setButtonText(2, "Ausgabe");
    }
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

        if (!mFilteredKeychainModel)
            return false;

        if (mKeychain)
        {
            mFilteredKeychainModel->setSourceModel(model);
            mKeychain->hideColumn(7); // hide image column
            mKeychain->setEditTriggers(QTableView::NoEditTriggers);
            mKeychain->setSelectionMode(QTableView::NoSelection);
            mKeychain->verticalHeader()->hide();

            mKeychain->show();

            mKeychain->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
            mKeychain->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

            mKeychain->resizeColumnsToContents();

            //barcode must be unique
            if (mKeychain->model()->rowCount() > 1)
            {
                QMessageBox::critical(0, "Datenbankfehler",
                                      "Barcode nicht eindeutig, Tabelle enthält Duplikate. \n"
                                      "Überprüfung der Datenbank notwendig. Fortfahren nicht möglich.", QMessageBox::Ok);
                //prevent user to proceed to next step
                return false;
            }

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
            mFilteredKeyModel->setSourceModel(model);
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
    qDebug () << "KeychainStatusView::setKeychainImagePath: " << imgPath;

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
    // if (mKeyOverview)
    // {
    //     delete mKeyOverview;
    //     mKeyOverview = 0;
    // }

    if (mFilteredKeyModel)
    {
        delete mFilteredKeyModel;
        mFilteredKeyModel = 0;
    }

    if (mFilteredKeychainModel)
    {
        delete mFilteredKeychainModel;
        mFilteredKeychainModel = 0;
    }

    if (mKeychainModel)
    {
        delete mKeychainModel;
        mKeychainModel = 0;
    }
}

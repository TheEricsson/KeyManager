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
#include "codegeneratorview.h"
#include "cameraview.h"

KeychainStatusView::KeychainStatusView(QWidget *parent)
    : WinSubmenu {parent}
{
    mKeys = 0;
    //mKeychain = 0;
    mKeysImgPreview = 0;
    mFilteredKeyModel = 0;
    //mFilteredKeychainModel = 0;
    mViewData = 0;
    mCameraView = 0;

    setHeader("Informationen zum Schlüsselbund");

    QLabel* keychainKeycode = new QLabel ("Gescannter Code: ", this);
    QLabel* keychainStatus = new QLabel ("Aktueller Status: ", this);
    QLabel* keychainInternalLocation = new QLabel ("Schlüsselhaken: ", this);

    QLabel* customer = new QLabel ("Kunde", this);

    mKeychainKeycode = new QLabel (this);
    mKeychainStatus = new QLabel (this);
    mKeychainInternalLocation = new QLabel (this);
    mKeychainStreet = new QLabel (this);
    mKeychainCity = new QLabel (this);

    mKeysImgPreview = new QPushButton ("Bild hinzufügen", this);
    mKeysImgPreview->setMinimumHeight(100);
    mKeysImgPreview->setMinimumWidth(100);
    mKeysImgPreview->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QSpacerItem *spacerItem = new QSpacerItem (1,1, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    QGridLayout *keychainData = new QGridLayout();

    keychainData->addWidget(keychainKeycode, 0, 0);
    keychainData->addWidget(mKeychainKeycode, 0, 1);
    keychainData->addWidget(keychainStatus, 1, 0);
    keychainData->addWidget(mKeychainStatus, 1, 1);
    keychainData->addWidget(keychainInternalLocation, 2, 0);
    keychainData->addWidget(mKeychainInternalLocation, 2, 1);
    keychainData->addWidget(customer, 3, 0);
    keychainData->addWidget(mKeychainStreet, 3, 1);
    keychainData->addWidget(mKeychainCity, 4, 1);
    keychainData->addItem(spacerItem, 0, 3, 1, 1);
    keychainData->addWidget(mKeysImgPreview, 0, 4, 5, 1, Qt::AlignLeft);

    layout()->addItem(keychainData);

    //mKeychain = new QTableView (this);

    //layout()->addWidget(mKeychain);

    // mKeysImgPreview = new QPushButton ("Bild hinzufügen...", this);
    // mKeysImgPreview->setMinimumHeight(100);
    // mKeysImgPreview->setMinimumWidth(100);
    // mKeysImgPreview->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    // layout()->addWidget(mKeysImgPreview);

    QLabel *keysHeader = new QLabel ("Hinterlegte Schlüssel", this);
    layout()->addWidget(keysHeader);

    mKeys = new QTableView (this);
    layout()->addWidget(mKeys);

    QLabel *historyHeader = new QLabel ("Historie", this);
    layout()->addWidget(historyHeader);

    mHistory = new QTableView (this);
    layout()->addWidget(mHistory);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout()->addItem(spacer);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::MainMenu);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    // mKeychainModel = new QSqlRelationalTableModel ();
    // mFilteredKeychainModel = new QSortFilterProxyModel ();
    // mFilteredKeychainModel->setSourceModel(mKeychainModel);
    // mKeychain->setModel(mFilteredKeychainModel);

    mKeyModel = new QSqlRelationalTableModel ();
    mFilteredKeyModel= new QSortFilterProxyModel ();
    mFilteredKeyModel->setSourceModel(mKeyModel);
    mKeys->setModel(mFilteredKeyModel);

    mHistoryModel = new QSqlRelationalTableModel ();
    mFilteredHistoryModel = new QSortFilterProxyModel ();
    mFilteredHistoryModel->setSourceModel(mHistoryModel);
    mHistory->setModel(mFilteredHistoryModel);

    //connect (keyCodeBtn, SIGNAL(clicked()), this, SLOT (keyCodeBtnClicked()));
    connect (mKeysImgPreview, SIGNAL(clicked()), this, SLOT (keyImgBtnClicked()));
}

void KeychainStatusView::showEvent(QShowEvent *)
{
    int barcode = dataInterface()->getScannedCode();
    QString barcodeAsString = QString::number(barcode);

    QString filter = "keychainId = ";
    filter.append(barcodeAsString);
    ioInterface()->initKeyOverviewModel(mKeyModel, filter);
    setKeysModel(mKeyModel);
    //mKeychain->update();

    filter = "id = ";
    filter.append(barcodeAsString);
    // ioInterface()->initKeychainModel(mKeychainModel, filter);
    // setKeychainModel(mKeychainModel);
    // mKeys->update ();

    filter = "keychainId = ";
    filter.append(barcodeAsString);
    ioInterface()->initKeychainHistoryModel(mHistoryModel, filter);
    setKeychainHistoryModel(mHistoryModel);
    mHistory->update ();

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

    QImage keychainImage;
    if  (ioInterface()->getKeychainImg(keyCode, keychainImage))
        setKeychainImage (keychainImage);
    else
        qDebug () << "keychainStatusView::showEvent - no keychain img from db available.";

    //set values for keychainstatus data from db
    ioInterface()->setKeychainData(dataInterface()->getDataKeychain(), keyCode);

    int code = dataInterface()->getScannedCode();
    mKeychainKeycode->setText(Database::normaliseKeycode(code));
    mKeychainStatus->setText(ioInterface()->getKeychainStatusText(dataInterface()->getKeychainStatusId()));
    mKeychainInternalLocation->setText(QString::number(dataInterface()->getInternalLocation()));

    int addressId = dataInterface()->getKeychainAddressId();
    QString txt = ioInterface()->getAddressStreet(addressId);
    txt.append(" ");
    txt.append(ioInterface()->getAddressStreetNumber(addressId));
    mKeychainStreet->setText(txt);

    txt = QString::number(ioInterface()->getAddressAreaCode(addressId));
    txt.append (" ");
    txt.append (ioInterface()->getAddressCity(addressId));
    mKeychainCity->setText(txt);

    setNextBtnText ();
}

void KeychainStatusView::setNextBtnText ()
{
    switch (dataInterface()->getKeychainStatusId())
    {
        case KeychainStatus::AdministrationEnded:
        case KeychainStatus::TemporaryOut:
        case KeychainStatus::PermanentOut:
        case KeychainStatus::Lost:
            setButtonText(2, "Rücknahme");
            break;
        default:
            setButtonText(2, "Ausgabe");
    }
}

// bool KeychainStatusView::setKeychainModel (QSqlRelationalTableModel* model)
// {
//     if (model)
//     {
//         model->setHeaderData(0, Qt::Horizontal, tr("Barcode"), Qt::DisplayRole);
//         model->setHeaderData(1, Qt::Horizontal, tr("Ausgabestatus"), Qt::DisplayRole);
//         model->setHeaderData(2, Qt::Horizontal, tr("Schlüsselhaken"), Qt::DisplayRole);
//         model->setHeaderData(3, Qt::Horizontal, tr("Straße"), Qt::DisplayRole);
//         model->setHeaderData(4, Qt::Horizontal, tr("Hausnummer"), Qt::DisplayRole);
//         model->setHeaderData(5, Qt::Horizontal, tr("PLZ"), Qt::DisplayRole);
//         model->setHeaderData(6, Qt::Horizontal, tr("Ort"), Qt::DisplayRole);

//         if (!mFilteredKeychainModel)
//             return false;

//         if (mKeychain)
//         {
//             mFilteredKeychainModel->setSourceModel(model);
//             mKeychain->hideColumn(7); // hide image column
//             mKeychain->setEditTriggers(QTableView::NoEditTriggers);
//             mKeychain->setSelectionMode(QTableView::NoSelection);
//             mKeychain->verticalHeader()->hide();

//             mKeychain->show();

//             mKeychain->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
//             mKeychain->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

//             mKeychain->resizeColumnsToContents();

//             //barcode must be unique
//             if (mKeychain->model()->rowCount() > 1)
//             {
//                 QMessageBox::critical(0, "Datenbankfehler",
//                                       "Barcode nicht eindeutig, Tabelle enthält Duplikate. \n"
//                                       "Überprüfung der Datenbank notwendig. Fortfahren nicht möglich.", QMessageBox::Ok);
//                 //prevent user to proceed to next step
//                 return false;
//             }

//             return true;
//         }
//         return false;
//     }
//     return false;
// }

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

            // for (int i = 0; i < mKeys->model()->rowCount(); i++)
            // {
            //     mKeys->setIndexWidget(mKeys->model()->index(i, 5), new QPushButton ("Editieren", this));
            // }

            return true;
        }
        return false;
    }
    return false;
}

bool KeychainStatusView::setKeychainHistoryModel (QSqlRelationalTableModel* model)
{
    if (model)
    {
        // model->setHeaderData(2, Qt::Horizontal, tr("Kategorie"), Qt::DisplayRole);
        // model->setHeaderData(3, Qt::Horizontal, tr("Zustand"), Qt::DisplayRole);
        // model->setHeaderData(4, Qt::Horizontal, tr("Zusatzinformation"), Qt::DisplayRole);

        if (mHistory)
        {
            mHistory->hideColumn(0); // hide table id
            mHistory->hideColumn(1); // hide barcode id
            mHistory->hideColumn(6); // hide address data
            mHistory->hideColumn(7); // hide address data
            mHistory->hideColumn(8); // hide address data
            mHistory->hideColumn(9); // hide address data
            mHistory->hideColumn(10); // signature name
            mHistory->hideColumn(11); // hide signature
            mHistory->hideColumn(12); // hide annotation

            mFilteredHistoryModel->setSourceModel(model);
            mHistory->setModel(model);
            //mKeys->setItemDelegate(new QSqlRelationalDelegate(mKeys));
            mHistory->setEditTriggers(QTableView::NoEditTriggers);
            mHistory->setSelectionMode(QTableView::NoSelection);
            mHistory->show();

            mHistory->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
            mHistory->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

            mHistory->resizeColumnsToContents();

            return true;
        }
        return false;
    }
    return false;
}

void KeychainStatusView::setKeychainImage (const QImage& img)
{
    if (mKeysImgPreview)
    {
        mKeysImgPreview->setText("");
        mKeysImgPreview->setIconSize(mKeysImgPreview->size());
        QIcon _img;
        _img.addPixmap(QPixmap::fromImage(img));
        mKeysImgPreview->setIcon(_img);
        //mKeysImgPreview->resize(100,100);
    }
}

void KeychainStatusView::keyCodeBtnClicked()
{
    qDebug () << "KeychainStatusView::keyCodeBtnClicked()";
    //CodeGeneratorView *view = new CodeGeneratorView ();
    //view->show ();
}

void KeychainStatusView::keyImgBtnClicked()
{
    qDebug () << "KeychainStatusView::keyImgBtnClicked()";

    if (!mCameraView)
    {
        mCameraView = new CameraView();
        connect (mCameraView, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT(onCameraViewButtonClicked(Gui::MenuButton)));
    }

    mCameraView->show();
}

void KeychainStatusView::onCameraViewButtonClicked(Gui::MenuButton btn)
{
    switch (btn)
    {
        case Gui::Save:
            if (mCameraView)
            {
                ioInterface()->dbInsertKeychainImg(dataInterface()->getScannedCode(), mCameraView->getImage());
                setKeychainImage(mCameraView->getImage());
                mCameraView->hide();
                delete mCameraView;
                mCameraView = 0;
                setFocus();
                update();
            }
            break;
        case Gui::Back:
            if (mCameraView)
            {
                mCameraView->hide();
                delete mCameraView;
                mCameraView = 0;
                setFocus();
                update();
            }
            break;
        default:
            break;
    }
}

KeychainStatusView::~KeychainStatusView ()
{
    if (mCameraView)
    {
        delete mCameraView;
        mCameraView = 0;
    }

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

    // if (mFilteredKeychainModel)
    // {
    //     delete mFilteredKeychainModel;
    //     mFilteredKeychainModel = 0;
    // }

    // if (mKeychainModel)
    // {
    //     delete mKeychainModel;
    //     mKeychainModel = 0;
    // }
}

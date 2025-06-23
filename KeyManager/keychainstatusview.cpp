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
#include <QFormLayout>
#include <QGroupBox>
#include "codegeneratorview.h"
#include "cameraview.h"
#include "imageview.h"

KeychainStatusView::KeychainStatusView(QWidget *parent)
    : WinSubmenu {parent}
{
    setHeader("Schlüsselbund");

    mViewMode = ViewMode::Undefined;
    mKeys = 0;
    //mKeychain = 0;
    mKeysImgPreview = 0;
    mFilteredKeyModel = 0;
    //mFilteredKeychainModel = 0;
    mViewData = 0;
    mCameraView = 0;
    mImageView = 0;

    //group keychain info
    mKeychainKeycode = new QLabel ();
    mKeychainStatus = new QLabel ();
    mKeychainInternalLocation = new QLabel ();
    mKeychainStreet = new QLabel ();
    mKeychainCity = new QLabel ();

    mKeysImgPreview = new QPushButton ("Bild hinzufügen");
    mKeysImgPreview->setMinimumHeight(100);
    mKeysImgPreview->setMinimumWidth(100);
    mKeysImgPreview->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QString groupBoxHeader = "Informationen zum Schlüsselbund";
    QGroupBox *keychainInfo = new QGroupBox(groupBoxHeader);
    QFormLayout *keychainInfoLayout = new QFormLayout();
    keychainInfo->setLayout(keychainInfoLayout);
    keychainInfoLayout->addRow("Straße: ", mKeychainStreet);
    keychainInfoLayout->addRow("PLZ, Ort: ", mKeychainCity);
    keychainInfoLayout->addRow("Aktueller Status: ", mKeychainStatus);
    keychainInfoLayout->addRow("Schlüsselhaken: ", mKeychainInternalLocation);
    keychainInfoLayout->addRow("QR-Code: ", mKeychainKeycode);

    //group preview image
    QGroupBox *keychainImageBox = new QGroupBox("Vorschau");
    QVBoxLayout *imageBoxLayout = new QVBoxLayout();
    keychainImageBox->setLayout(imageBoxLayout);
    imageBoxLayout->addWidget(mKeysImgPreview);

    //group key table
    mKeys = new QTableView (this);
    mKeys->setWordWrap(true);

    QGroupBox *keyOverviewBox = new QGroupBox("Hinterlegte Schlüssel");
    QVBoxLayout *keyOverviewLayout = new QVBoxLayout();
    keyOverviewBox->setLayout(keyOverviewLayout);
    keyOverviewLayout->addWidget(mKeys);

    //group keychain history
    mHistory = new QTableView (this);
    mHistory->setWordWrap(true);

    QGroupBox *keychainHistoryBox = new QGroupBox("Einträge zum Schlüsselbund");
    QVBoxLayout *keychainHistoryLayout = new QVBoxLayout();
    keychainHistoryBox->setLayout(keychainHistoryLayout);
    keychainHistoryLayout->addWidget(mHistory);

    QSpacerItem *spacerItem = new QSpacerItem (1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);

    //layout
    QGridLayout *keychainData = new QGridLayout();

    keychainData->addWidget(keychainInfo, 0, 0);
    keychainData->addWidget(keychainImageBox, 0, 1, Qt::AlignLeft);
    keychainData->addWidget(keyOverviewBox, 1, 0, 1, 2);
    keychainData->addWidget(keychainHistoryBox, 2, 0, 1, 2);
    keychainData->addItem(spacerItem, 3, 0, 1, 2);

    setCentralLayout(keychainData);

    mKeyModel = new QSqlRelationalTableModel (this);
    mFilteredKeyModel= new QSortFilterProxyModel (this);
    mFilteredKeyModel->setSourceModel(mKeyModel);
    mKeys->setModel(mFilteredKeyModel);

    mHistoryModel = new QSqlRelationalTableModel (this);
    mFilteredHistoryModel = new QSortFilterProxyModel (this);
    mFilteredHistoryModel->setSourceModel(mHistoryModel);
    mHistory->setModel(mFilteredHistoryModel);

    connect (mKeysImgPreview, SIGNAL(clicked()), this, SLOT (keyImgBtnClicked()));
}

void KeychainStatusView::setViewMode(ViewMode::Value mode)
{
    QList<Gui::MenuButton> menuButtons;

    mViewMode = mode;

    switch (mViewMode)
    {
        case ViewMode::ShowData:
            menuButtons.append(Gui::Back);
            setMenuButtons(menuButtons);
            break;
        default:
            menuButtons.append(Gui::Back);
            menuButtons.append(Gui::MainMenu);
            menuButtons.append(Gui::Next);
            setMenuButtons(menuButtons);
            break;
    }
}

void KeychainStatusView::showEvent(QShowEvent *)
{
    int barcode = dataInterface()->getScannedCode();
    QString barcodeAsString = QString::number(barcode);

    QString filter = "keychainId = ";
    filter.append(barcodeAsString);
    ioInterface()->initKeyOverviewModel(mKeyModel, filter);
    setKeysModel(mKeyModel);

    mKeys->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mKeys->horizontalHeader()->setStretchLastSection(true);
    mKeys->resizeRowsToContents();
    mKeys->resizeColumnsToContents();

    filter = "id = ";
    filter.append(barcodeAsString);

    filter = "keychainId = ";
    filter.append(barcodeAsString);
    ioInterface()->initKeychainHistoryModel(mHistoryModel, filter);
    setKeychainHistoryModel(mHistoryModel);

    mHistory->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mHistory->horizontalHeader()->setStretchLastSection(true);
    mHistory->resizeRowsToContents();
    mHistory->resizeColumnsToContents();

    dataInterface()->resetKeychainData();

    int keyCode = dataInterface()->getScannedCode();
    qDebug()<<"keycode:" << keyCode;

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

    if (mViewMode != ViewMode::ShowData)
    {
        setNextBtnText ();
    }
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
            qDebug () << "KeychainStatusView::setKeysModel QSqlRelationalTableModel";
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
        model->setHeaderData(2, Qt::Horizontal, tr("Eintrag vom"), Qt::DisplayRole);
        model->setHeaderData(3, Qt::Horizontal, tr("Rückgabefrist"), Qt::DisplayRole);
        model->setHeaderData(4, Qt::Horizontal, tr("Status"), Qt::DisplayRole);
        model->setHeaderData(5, Qt::Horizontal, tr("Empfänger"), Qt::DisplayRole);

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
    QImage keychainImage;
    ioInterface()->getKeychainImg(dataInterface()->getScannedCode(), keychainImage);

    if (!mImageView)
    {
        mImageView = new ImageView();
        connect (mImageView, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT(onImageViewButtonClicked(Gui::MenuButton)));
    }

    mImageView->show();

    if (!keychainImage.isNull())
        mImageView->setImage(keychainImage);
}

void KeychainStatusView::onCameraViewButtonClicked(Gui::MenuButton btn)
{
    switch (btn)
    {
        case Gui::Shutter:
            if (mCameraView)
            {
                QImage img = mCameraView->getImage();
                if (ioInterface()->dbInsertKeychainImg(dataInterface()->getScannedCode(), img))
                {
                    showMessageWindow("Bild gespeichert.");
                    setKeychainImage(mCameraView->getImage());
                }
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

void KeychainStatusView::onImageViewButtonClicked(Gui::MenuButton btn)
{
    // create empty image (delete image)
    QImage img;
    unsigned int keyCode = dataInterface()->getScannedCode();

    switch (btn)
    {
        case Gui::TakePicture:
            if (mImageView)
            {
                mImageView->hide();
                delete mImageView;
                mImageView = 0;
            }
            if (!mCameraView)
            {
                mCameraView = new CameraView();
                connect (mCameraView, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT(onCameraViewButtonClicked(Gui::MenuButton)));
            }

            mCameraView->show();
            break;
        case Gui::Delete:
            if (mImageView)
            {
                mImageView->hide();
                delete mImageView;
                mImageView = 0;
            }

            //insert empty image
            ioInterface()->dbInsertKeychainImg(keyCode, img);

            //read back image from db and set in gui
            ioInterface()->getKeychainImg(keyCode, img);
            setKeychainImage(img);

            //inform user about deleted image
            showMessageWindow("Bild wurde gelöscht.");

            break;
        case Gui::Back:
            if (mImageView)
            {
                mImageView->hide();
                delete mImageView;
                mImageView = 0;
            }
            break;
        default:
            break;
    }
}

void KeychainStatusView::showMessageWindow(const QString& infoText)
{
    QMessageBox msgBox;
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setInformativeText(infoText);
    msgBox.exec();
}

KeychainStatusView::~KeychainStatusView ()
{
    if (mCameraView)
    {
        mCameraView->deleteLater();
    }

    if (mImageView)
    {
        mImageView->deleteLater();
    }
}

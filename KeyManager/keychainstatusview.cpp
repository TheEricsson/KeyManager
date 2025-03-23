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
#include "winsubmenu.h"

KeychainStatusView::KeychainStatusView(QWidget *parent)
    : WinSubmenu {parent}
{
    mKeys = 0;
    mKeychain = 0;
    mKeysImgPreview = 0;

    setHeader("Informationen zum Schlüsselbund");

    QPushButton* btnPrevious = new QPushButton ();
    btnPrevious->setIcon(QIcon(":/images/menu_back.png"));
    btnPrevious->setIconSize (QSize(UiSpecs::buttonWidth, UiSpecs::buttonHeight));

    QLabel *keyChainHeader = new QLabel ("Schlüsselbund");
    layout()->addWidget(keyChainHeader);

    mKeychain = new QTableView ();
    layout()->addWidget(mKeychain);

    mKeysImgPreview = new QPushButton ("Kein Bild vorhanden.");
    mKeysImgPreview->setMinimumHeight(100);
    mKeysImgPreview->setMinimumWidth(100);
    layout()->addWidget(mKeysImgPreview);

    QLabel *keysHeader = new QLabel ("Enthaltene Schlüssel");
    layout()->addWidget(keysHeader);

    mKeys = new QTableView;
    layout()->addWidget(mKeys);

    setMenuButtons(UiSpecs::BackButton, UiSpecs::NextButton);
}

void KeychainStatusView::setKeychainStatus (const int &statusId)
{
    // if (0 != mButtonNext)
    // {
    //     switch (statusId)
    //     {
    //     case Database::Available:
    //         //mButtonNext->setIcon(QIcon(":/images/menu_keyOut.png"));
    //         break;
    //     case Database::TemporaryOut:
    //     case Database::PermanentOut:
    //     case Database::Lost:
    //     case Database::AdministrationEnded:
    //         //mButtonNext->setIcon(QIcon(":/images/menu_keyBack.png"));
    //         break;
    //     default:
    //         break;
    //     }
    // }
    // else
    //     qDebug () << "KeychainStatusView::setKeychainStatus: mButtonNext is NULL";
    // return;
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
            qDebug () << "KeychainStatusView::setModel QSqlRelationalTableModel";
            mKeychain->setModel(model);
            mKeychain->hideColumn(7); // hide image column
            mKeychain->setEditTriggers(QTableView::NoEditTriggers);
            mKeychain->setSelectionMode(QTableView::NoSelection);
            mKeychain->verticalHeader()->hide();

            mKeychain->show();

            mKeychain->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
            mKeychain->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

            mKeychain->resizeColumnsToContents();

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

void KeychainStatusView::onPreviousBtnClicked ()
{
    emit previousButtonClicked ();
}

void KeychainStatusView::onNextBtnClicked ()
{
    emit nextButtonClicked ();
}

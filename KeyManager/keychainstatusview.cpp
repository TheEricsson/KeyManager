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

KeychainStatusView::KeychainStatusView(QWidget *parent)
    : QWidget{parent}
{
    mKeys = 0;
    mKeychain = 0;
    mButtonNext = 0;
    mKeysImgPreview = 0;

    QPushButton* btnPrevious = new QPushButton ();
    btnPrevious->setIcon(QIcon(":/images/menu_back.png"));
    btnPrevious->setIconSize (QSize(UiSpecs::buttonWidth, UiSpecs::buttonHeight));

    mButtonNext = new QPushButton ();
    mButtonNext->setIcon(QIcon(":/images/menu_keyOut.png"));
    mButtonNext->setIconSize (QSize(UiSpecs::buttonWidth, UiSpecs::buttonHeight));

    QVBoxLayout *mainLayout = new QVBoxLayout (this);

    QLabel *keyChainHeader = new QLabel ("Schlüsselbund");
    mainLayout->addWidget(keyChainHeader);

    mKeychain = new QTableView;
    mainLayout->addWidget(mKeychain);

    mKeysImgPreview = new QPushButton ("Kein Bild vorhanden.");
    mKeysImgPreview->setMinimumHeight(100);
    mKeysImgPreview->setMinimumWidth(100);
    mainLayout->addWidget(mKeysImgPreview);

    QLabel *keysHeader = new QLabel ("Enthaltene Schlüssel");
    mainLayout->addWidget(keysHeader);

    mKeys = new QTableView;
    mainLayout->addWidget(mKeys);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainLayout->addSpacerItem(spacer);

    QHBoxLayout *btnLayout = new QHBoxLayout ();
    btnLayout->addWidget(btnPrevious);
    btnLayout->addWidget(mButtonNext);

    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("Übersicht"));
    show();

    connect (btnPrevious, SIGNAL (clicked()), this, SLOT (onPreviousBtnClicked()));
    connect (mButtonNext, SIGNAL (clicked()), this, SLOT (onNextBtnClicked()));
}

void KeychainStatusView::setKeychainStatus (const int &statusId)
{
    if (0 != mButtonNext)
    {
        switch (statusId)
        {
        case Database::Available:
            mButtonNext->setIcon(QIcon(":/images/menu_keyOut.png"));
            break;
        case Database::TemporaryOut:
        case Database::PermanentOut:
        case Database::Lost:
        case Database::AdministrationEnded:
            mButtonNext->setIcon(QIcon(":/images/menu_keyBack.png"));
            break;
        default:
            break;
        }
    }
    else
        qDebug () << "KeychainStatusView::setKeychainStatus: mButtonNext is NULL";
    return;
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

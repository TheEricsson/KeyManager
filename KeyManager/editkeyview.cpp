#include "editkeyview.h"
#include "datainterface.h"
#include "iointerface.h"
#include <QLabel>
#include <QLayout>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QSqlRelationalDelegate>
#include <QTextEdit>

EditKeyView::EditKeyView(QWidget *parent) : WinSubmenu {parent}
{
    mKeys = 0;
    mKeyModel = 0;
    mAddKeyView = 0;

    setHeader("Schlüsselverwaltung");
    mKeychainCode = new QLabel (this);

    QVBoxLayout* centralLayout = new QVBoxLayout();

    QLabel *headerTable = new QLabel ("Hinterlegte Schlüssel:");
    mKeys = new QTableView (this);

    mKeyModel = new QSqlRelationalTableModel (this);
    mKeys->setModel(mKeyModel);

    centralLayout->addWidget(mKeychainCode);
    centralLayout->addWidget(headerTable);
    centralLayout->addWidget(mKeys);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    centralLayout->addItem(spacer);

    setCentralLayout(centralLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::AddKey);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    connect (mKeys->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onTableSelectionChanged(const QItemSelection &, const QItemSelection &)));
}

void EditKeyView::showEvent(QShowEvent *)
{
    reset ();
    update ();
}

bool EditKeyView::setKeysModel (QSqlRelationalTableModel* model)
{
    if (model)
    {
        model->setEditStrategy(QSqlTableModel::OnFieldChange);

        model->setHeaderData(2, Qt::Horizontal, tr("Kategorie"), Qt::DisplayRole);
        model->setHeaderData(3, Qt::Horizontal, tr("Zustand"), Qt::DisplayRole);
        model->setHeaderData(4, Qt::Horizontal, tr("Zusatzinformation"), Qt::DisplayRole);

        if (mKeys)
        {
            qDebug () << "KeychainStatusView::setModel QSqlRelationalTableModel";
            mKeys->setModel(model);
            mKeys->hideColumn(0); // hide table id
            mKeys->hideColumn(1); // hide barcode id
            mKeys->setItemDelegate(new QSqlRelationalDelegate(mKeys));
            // mKeys->setEditTriggers(QTableView::NoEditTriggers);
            // mKeys->setSelectionMode(QTableView::SingleSelection);
            mKeys->show();

            mKeys->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
            mKeys->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

            mKeys->resizeColumnsToContents();

            return true;
        }
        return false;
    }
    return false;
}

void EditKeyView::onMenuBtnClicked (Gui::MenuButton btnType)
{
    switch (btnType)
    {
        case (Gui::AddKey):
            if (!mAddKeyView)
            {
                mAddKeyView = new AddKeyView();
                mAddKeyView->setDataInterface (dataInterface());
                mAddKeyView->setIOInterface (ioInterface());
                mAddKeyView->show();
                connect (mAddKeyView, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT(onAddKeyMenuButtonClicked(Gui::MenuButton)));
            }
            break;

        // no catch, emit signal
        default:
            emit menuButtonClicked(btnType);
            break;

    }
}

void EditKeyView::onTableSelectionChanged (const QItemSelection &itemNew, const QItemSelection &itemOld)
{
    Q_UNUSED(itemOld);
    Q_UNUSED(itemNew);

    if (!mKeyModel)
        return;

    if (!mKeys)
        return;

    mKeys->update();
    update();
}

void EditKeyView::onAddKeyMenuButtonClicked (Gui::MenuButton btn)
{
    switch (btn)
    {
        case Gui::Ok:
        case Gui::Back:
            // delete addkeyview instance
            if (mAddKeyView)
            {
                mAddKeyView->hide();
                delete mAddKeyView;
                mAddKeyView = nullptr;
            }
            reset ();
            setFocus();
            mKeys->update();
            mKeys->resizeRowsToContents();
            update();
            break;
        default:
            break;
    }
}

void EditKeyView::reset()
{
    int barcode = dataInterface()->getScannedCode();

    QString labelText = "Schlüsselbund Nr: ";

    labelText.append (Database::normaliseKeycode(barcode));
    mKeychainCode->setText(labelText);

    QString filterKeyTable = "keychainId = ";
    filterKeyTable.append(QString::number(barcode));

    //init model/view at first show event
    if (!mKeyModel)
        mKeyModel = new QSqlRelationalTableModel(this);

    if (mKeyModel)
    {
        ioInterface()->initKeyOverviewModel(mKeyModel, filterKeyTable);
        setKeysModel(mKeyModel);
        mKeys->resizeRowsToContents();
    }
    update ();
}

EditKeyView::~EditKeyView()
{
    if (mAddKeyView)
    {
        delete mAddKeyView;
        mAddKeyView = 0;
    }
}

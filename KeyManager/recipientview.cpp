#include "recipientview.h"
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
#include <QCalendarWidget>
#include <QCheckBox>
#include <QButtonGroup>
#include <QDate>
#include <QGroupBox>
#include <QMessageBox>

#include "winsubmenu.h"
#include "globals.h"
#include "datainterface.h"
#include "iointerface.h"
#include "returndateview.h"
#include "addrecipientview.h"

RecipientView::RecipientView(QWidget *parent) : WinSubmenu {parent}
{
    setHeader("Schlüsselempfänger");

    mRecipients = 0;
    mFilteredModel = 0;
    mActiveEntries = 0;
    mRecipientsModel = 0;
    mRowSelected = false;
    mAddRecipientView = 0;
    mCurrentRecipientId = _UNDEFINED;
    mIsHandoverSession = false;

    mActiveEntries = new QSortFilterProxyModel (this);
    mFilteredModel = new QSortFilterProxyModel (this);
    mActiveEntries->setSourceModel(mFilteredModel);

    QGridLayout* centralLayout = new QGridLayout();

    mSearchLabel = new QLabel ("Suche", this);
    mSearchField = new QLineEdit (this);

    mRecipients = new QTableView (this);
    mRecipients->setModel(mActiveEntries);

    mRecipientNameLabel = new QLabel ("Unterzeichner:", this);
    mRecipientNameEdit = new QLineEdit ("", this);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    centralLayout->addWidget(mSearchLabel, 0, 0);
    centralLayout->addWidget(mSearchField, 0, 1);
    centralLayout->addWidget(mRecipients, 1, 0, 1, 2);
    centralLayout->addWidget(mRecipientNameLabel, 2, 0);
    centralLayout->addWidget(mRecipientNameEdit, 2, 1);
    centralLayout->addItem(spacer, 3, 0);

    setCentralLayout(centralLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Edit);
    menuButtons.append(Gui::Delete);
    menuButtons.append(Gui::AddRecipient);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    //disable buttons, which need a valid selection in the table
    setMenuButtonState(false);

    mRecipients->update();
    mRecipients->setFocus();

    connect (mRecipients->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onTableSelectionChanged(const QItemSelection &, const QItemSelection &)));
    connect (mSearchField, SIGNAL(textChanged(QString)), this, SLOT(setTableFilter(QString)));
    connect (mRecipientNameEdit, SIGNAL(textChanged(QString)), this, SLOT(onRecipientNameTextChanged(QString)));
}

void RecipientView::setViewMode(ViewMode::Value mode)
{
    switch (mode)
    {
        case ViewMode::Handover:
            mIsHandoverSession = true;
            break;
        default:
            mIsHandoverSession = false;
            break;
    }
}

void RecipientView::setHandoverSession(bool value)
{
    mIsHandoverSession = value;
}

bool RecipientView::setModel (QSqlRelationalTableModel* model)
{
    if (!model)
        return false;

    if (!mRecipients)
        return false;

    if (!mFilteredModel)
        return false;

    model->setHeaderData(0, Qt::Horizontal, tr("ID"), Qt::DisplayRole);
    model->setHeaderData(1, Qt::Horizontal, tr("Name"), Qt::DisplayRole);
    model->setHeaderData(2, Qt::Horizontal, tr("Typ"), Qt::DisplayRole);
    model->setHeaderData(3, Qt::Horizontal, tr("Straße"), Qt::DisplayRole);
    model->setHeaderData(4, Qt::Horizontal, tr("Hausnummer"), Qt::DisplayRole);
    model->setHeaderData(5, Qt::Horizontal, tr("PLZ"), Qt::DisplayRole);
    model->setHeaderData(6, Qt::Horizontal, tr("Stadt"), Qt::DisplayRole);
    model->setHeaderData(7, Qt::Horizontal, tr("DB Status"), Qt::DisplayRole);

    mFilteredModel->setSourceModel(model);
    mFilteredModel->setFilterCaseSensitivity(Qt::CaseInsensitive); // ignore capital letters

    mRecipients->hideColumn(0); //don't show table index
    mRecipients->hideColumn(7); //don't show db status

    mRecipients->setSortingEnabled(true);
    mRecipients->sortByColumn(1, Qt::AscendingOrder);
    mRecipients->setEditTriggers(QTableView::NoEditTriggers);
    mRecipients->setSelectionMode(QTableView::SingleSelection);
    mRecipients->setSelectionBehavior(QTableView::SelectRows);

    //show active entries only
    filterActiveEntries();

    mRecipients->show();

    mRecipients->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mRecipients->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    mRecipients->resizeColumnsToContents();

    update ();

    return true;
}

void RecipientView::reset()
{
    //reset data/views
    dataInterface()->resetRecipientData();

    hideSearchField(false);
    hideNameField(true);
    setMenuButtonState(false);

    //init model/view at first show event
    if (!mRecipientsModel)
        mRecipientsModel = new QSqlRelationalTableModel(this);

    if (mRecipientsModel)
    {
        ioInterface()->initRecipientModel(mRecipientsModel);
        setModel(mRecipientsModel);
    }

    //if view is called as handover session -> check current keychain status and set view state
    if (mIsHandoverSession)
    {
        switch (dataInterface()->getKeychainStatusId())
        {
            case KeychainStatus::Value::AdministrationEnded:
            case KeychainStatus::Value::Lost:
            case KeychainStatus::Value::PermanentOut:
            case KeychainStatus::Value::TemporaryOut:
            case KeychainStatus::Value::Undefined:
                setTableFilter(2, "Mitarbeiter");
                break;
            default:
                setTableFilter("");
        }
    }

    mRecipients->clearSelection();
    mRecipients->setFocus();
    mRecipients->update();
    mSearchField->setText("");
    mRowSelected = false;
}

void RecipientView::showEvent(QShowEvent *)
{
    showFullScreen();
    reset ();
}

void RecipientView::onMenuBtnClicked (Gui::MenuButton btnType)
{
    switch (btnType)
    {
    case (Gui::Next):
        switch (dataInterface()->getNewKeychainStatusId())
        {
            case KeychainStatus::Value::AdministrationEnded:
            case KeychainStatus::Value::Available:
            case KeychainStatus::Value::Lost:
            case KeychainStatus::Value::PermanentOut:
                dataInterface()->setRecipientDeadlineDate("");
                break;
            default:
                break;
        }
        break;
    case (Gui::AddRecipient):
        addRecipient();
        break;
    case (Gui::Edit):
        editRecipient();
        break;
    case (Gui::Delete):
        disableRecipient();
    default:
        break;
    }

    emit menuButtonClicked(btnType);
}

void RecipientView::setTableFilter(const QString &text)
{
    if (!mRecipients)
        return;

    mFilteredModel->setFilterKeyColumn(-1); //set filter to search in all columns
    mFilteredModel->setFilterWildcard(text);
    mRecipients->update();
    update ();
}

void RecipientView::filterActiveEntries()
{
    DbEntryStatus::Value dbStatus = DbEntryStatus::Enabled;
    mActiveEntries->setFilterKeyColumn(7);
    mActiveEntries->setFilterRegularExpression(QRegularExpression(QString::number(dbStatus)));
    mRecipients->update();
}

void RecipientView::setTableFilter(const int column, const QString &searchString)
{
    if (!mRecipients)
        return;

    mFilteredModel->setFilterKeyColumn(column);
    //mFilteredModel->setFilterWildcard(searchString);
    mFilteredModel->setFilterRegularExpression(QRegularExpression(searchString, QRegularExpression::CaseInsensitiveOption));

    qDebug () << "RecipientView::setTableFilter";
    qDebug () << "column: " << column;
    qDebug () << "searchString: " << searchString;
    mRecipients->update();
}

void RecipientView::hideSearchField (bool hide)
{
    mSearchLabel->setHidden(hide);
    mSearchField->setHidden(hide);

    update ();
}

void RecipientView::hideNameField (bool hide)
{
    mRecipientNameEdit->setHidden(hide);
    mRecipientNameLabel->setHidden(hide);

    update ();
}

void RecipientView::setMenuButtonState (bool tableitemSelected)
{
    enableButton(1, tableitemSelected);
    enableButton(2, tableitemSelected);
    enableButton(4, tableitemSelected);
}

void RecipientView::addRecipient()
{
    if (!mAddRecipientView)
    {
        mAddRecipientView = new AddRecipientView();
        connect (mAddRecipientView, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT(onAddRecipientButtonClicked(Gui::MenuButton)));
    }
    mAddRecipientView->setDataInterface (dataInterface());
    mAddRecipientView->setViewMode(ViewMode::NewData);
    mAddRecipientView->setIOInterface (ioInterface());
    mAddRecipientView->show();
}

void RecipientView::editRecipient()
{
    if (!mAddRecipientView)
    {
        mAddRecipientView = new AddRecipientView();
        connect (mAddRecipientView, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT(onAddRecipientButtonClicked(Gui::MenuButton)));
    }
    mAddRecipientView->setDataInterface (dataInterface());
    mAddRecipientView->setData(mCurrentRecipientId, dataInterface()->getDataRecipient());
    mAddRecipientView->setViewMode(ViewMode::EditData);
    mAddRecipientView->setIOInterface (ioInterface());
    mAddRecipientView->show();
}

void RecipientView::disableRecipient()
{
    QMessageBox msgBox;
    msgBox.setText("Fortfahren?");
    msgBox.setInformativeText("Wenn Sie fortfahren, wird dieser Datensatz deaktiviert.\n \
                    Dieser Vorgang kann nicht rückgängig gemacht werden!\n \
                    Möchten Sie fortfahren?");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    switch (ret)
    {
        case QMessageBox::Yes:
            if (ioInterface())
            {
                ioInterface()->disableRecipient(mCurrentRecipientId);
                reset();
            }
            break;
        case QMessageBox::No:
            break;
        default:
            break;
    }
}

void RecipientView::onTableSelectionChanged (const QItemSelection &itemNew, const QItemSelection &itemOld)
{
    Q_UNUSED(itemOld);
    Q_UNUSED(itemNew);

    //mRecipientNameEdit->setStyleSheet ("");
    mRecipientNameEdit->setText("");

    int row = mRecipients->selectionModel()->currentIndex().row();
    mCurrentRecipientId = mRecipients->model()->index(row, 0).data().toInt();
    qDebug () << "mCurrentRecipientId: " << mCurrentRecipientId;
    dataInterface()->setRecipientName(mRecipients->model()->index(row, 1).data().toString ());
    QString recipientType = mRecipients->model()->index(row, 2).data().toString ();
    RecipientType::Value recipientTypeId = ioInterface()->getRecipientTypeId(recipientType);
    dataInterface()->setRecipientType(recipientTypeId);
    dataInterface()->setRecipientStreet(mRecipients->model()->index(row, 3).data().toString ());
    dataInterface()->setRecipientStreetNumber(mRecipients->model()->index(row, 4).data().toString ());
    dataInterface()->setRecipientAreaCode(mRecipients->model()->index(row, 5).data().toInt ());
    dataInterface()->setRecipientCity(mRecipients->model()->index(row, 6).data().toString());

    if (!itemNew.isEmpty())
    {
        setMenuButtonState(true);
        mRowSelected = true;
    }
    else
    {
        setMenuButtonState (false);
        mRowSelected = false;
    }

    if (mIsHandoverSession)
    {
        // show or hide signee field, depending on selected recipient type
        RecipientType::Value recTypeId = ioInterface()->getRecipientTypeId(recipientType);
        mRecipientNameEdit->setText("");
        switch (recTypeId)
        {
            case RecipientType::Company:
                hideNameField (false);
                break;
            default:
                hideNameField(true);
        }
    }

    update();
}

void RecipientView::onRecipientNameTextChanged (const QString &text)
{
    dataInterface()->setRecipientSigName(text);
    update ();
}

void RecipientView::onAddRecipientButtonClicked(Gui::MenuButton btn)
{
    switch (btn)
    {
        case Gui::Back:
        case Gui::Ok:
            if (mAddRecipientView)
            {
                mAddRecipientView->hide();
                delete mAddRecipientView;
                mAddRecipientView = 0;
                reset();
                setFocus();
            }
            break;
        default:
            break;
    }
}

RecipientView::~RecipientView()
{
    if (mAddRecipientView)
    {
        mAddRecipientView->deleteLater();
    }
}

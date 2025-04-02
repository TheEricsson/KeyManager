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
#include "returndateview.h"
#include "viewdatarecipient.h"

RecipientView::RecipientView(QWidget *parent) : WinSubmenu {parent}
{
    mRecipients = 0;
    mFilteredModel = 0;
    mRecipientsModel = 0;
    mRowSelected = false;
    mViewDataRecipient = 0;

    mFilteredModel = new QSortFilterProxyModel (this);

    setHeader("Empfänger auswählen");

    //QVBoxLayout* layout = new QVBoxLayout (this);

    QHBoxLayout* searchLayout = new QHBoxLayout ();
    mSearchLabel = new QLabel ("Suche", this);
    mSearchField = new QLineEdit (this);
    searchLayout->addWidget(mSearchLabel);
    searchLayout->addWidget(mSearchField);
    layout()->addItem(searchLayout);

    mRecipients = new QTableView (this);
    mRecipients->setModel(mFilteredModel);
    layout()->addWidget(mRecipients);

    //mRecipients->clearSelection();
    mRecipients->update();
    mRecipients->setFocus();

    mRecipientNameLabel = new QLabel ("Empfängername:", this);
    mRecipientNameEdit = new QLineEdit ("", this);
    QVBoxLayout *recipientNameLayout = new QVBoxLayout ();
    recipientNameLayout->addWidget(mRecipientNameLabel);
    recipientNameLayout->addWidget(mRecipientNameEdit);
    layout()->addItem(recipientNameLayout);

    mReturnDateWidget = new ReturnDateView ();
    layout()->addWidget(mReturnDateWidget);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::AddRecipient);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    //setMenuButtons(Gui::Back, Gui::AddRecipient, Gui::Next);
    disableButton(2, true);

    connect (mRecipients->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onTableSelectionChanged(const QItemSelection &, const QItemSelection &)));
    connect (mSearchField, SIGNAL (textChanged(const QString &)), this, SLOT(setTableFilter(const QString &)));
    connect (mReturnDateWidget, SIGNAL (dateSelectionChanged(QDate)), this, SLOT(onSelectedDateChanged (QDate)));
    connect (mReturnDateWidget, SIGNAL (keychainStatusChanged(Database::KeychainStatus)), this, SLOT(onKeychainStatusChanged (Database::KeychainStatus)));
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

    mFilteredModel->setSourceModel(model);
    mFilteredModel->setFilterCaseSensitivity(Qt::CaseInsensitive); // ignore capital letters

    mRecipients->hideColumn(0); //don't show table index

    mRecipients->setSortingEnabled(true);
    mRecipients->sortByColumn(1, Qt::AscendingOrder);
    mRecipients->setEditTriggers(QTableView::NoEditTriggers);
    mRecipients->setSelectionMode(QTableView::SingleSelection);
    mRecipients->setSelectionBehavior(QTableView::SelectRows);

    mRecipients->show();

    mRecipients->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mRecipients->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    mRecipients->resizeColumnsToContents();

    update ();

    return true;
}

void RecipientView::reset()
{
    if (mViewDataRecipient)
    {
        delete mViewDataRecipient;
        mViewDataRecipient = 0;
    }

    mViewDataRecipient = new ViewDataRecipient ();
    dataInterface()->setData(mViewDataRecipient);

    QDate curDate = QDate::currentDate();
    int day = 0;
    int month = 0;
    int year = 0;
    curDate.getDate(&year, &month, &day);

    QString curDateString = QString::number(day);
    curDateString.append(".");
    curDateString.append(QString::number(month));
    curDateString.append(".");
    curDateString.append(QString::number(year));

    mViewDataRecipient->setCurrentDate(curDateString);

    //init model/view at first show event
    if (!mRecipientsModel)
    {
        mRecipientsModel = new QSqlRelationalTableModel ();
        dataInterface()->initRecipientModel(mRecipientsModel);
        setModel(mRecipientsModel);
    }

    hideSearchField(false);
    hideNameField(false);
    disableButton(2, true);

    switch (dataInterface()->getKeychainStatusId())
    {
        case Database::KeychainStatus::AdministrationEnded:
        case Database::KeychainStatus::Lost:
        case Database::KeychainStatus::PermanentOut:
        case Database::KeychainStatus::TemporaryOut:
        case Database::KeychainStatus::Undefined:
            hideSearchField(true);
            hideNameField(true);
            setTableFilter(2, "Mitarbeiter");
            mReturnDateWidget->hide();
            update ();
            mViewDataRecipient->setDeadlineDate("");
            dataInterface()->setNewKeychainStatusId(Database::KeychainStatus::Available);
            break;
        default:
            hideSearchField(false);
            hideNameField(false);
            setTableFilter("");
            update ();
            mReturnDateWidget->show();
    }

    mRecipients->clearSelection();
    mRecipients->setFocus();
    mRecipients->update();
    mRowSelected = false;
}

void RecipientView::showEvent(QShowEvent *)
{
    reset ();
}

void RecipientView::onMenuBtnClicked (Gui::MenuButton btnType)
{
    switch (btnType)
    {
    case (Gui::Next):
        switch (dataInterface()->getNewKeychainStatusId())
        {
            case Database::KeychainStatus::AdministrationEnded:
            case Database::KeychainStatus::Available:
            case Database::KeychainStatus::Lost:
            case Database::KeychainStatus::PermanentOut:
                dataInterface()->setDeadlineDate("");
        }

        break;
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

// void RecipientView::onThirdBtnClicked ()
// {
//     // if (!mRowSelected)
    //     return;

    // if (mRecipientNameEdit->isEnabled() && "" == mRecipientNameEdit->text())
    // {
    //     mRecipientNameEdit->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");

    //     QMessageBox msgBox;
    //     msgBox.setText("Unvollständige Eingaben");
    //     msgBox.setInformativeText("Es wurde ein Unternehmen als Empfänger \n"
    //                              "ausgewählt. Der Name der Person, die den \n"
    //                               "Schlüssel entgegennimmt, wurde nicht angegeben.");

    //     msgBox.setStandardButtons(QMessageBox::Ignore | QMessageBox::Ok);
    //     msgBox.setDefaultButton(QMessageBox::Ok);

    //     int selection = msgBox.exec();

    //     bool ignore = false;

    //     switch (selection)
    //     {
    //         case QMessageBox::Ignore:
    //             ignore = true;
    //             break;
    //         case QMessageBox::Ok:
    //             break;
    //         default:
    //             break;
    //     }

    //     if (!ignore)
    //         return;
    // }

    // //set current data
    // if (mDataObject)
    // {
    //     DataObjectHandover *dataObj = (DataObjectHandover*)mDataObject;
    //     if (dataObj)
    //     {
    //         int row = mRecipients->selectionModel()->currentIndex().row();
    //         dataObj->setRecipient(mRecipients->model()->index(row, 1).data().toString ());
    //         dataObj->setSignatureName(mRecipientNameEdit->text());
    //         dataObj->setRecipientStreet(mRecipients->model()->index(row, 3).data().toString ());
    //         dataObj->setRecipientStreetNumber(mRecipients->model()->index(row, 4).data().toInt());
    //         dataObj->setRecipientAreaCode(mRecipients->model()->index(row, 5).data().toInt ());
    //         dataObj->setRecipientCity(mRecipients->model()->index(row, 6).data().toString ());
    //     }
    // }
    // emit thirdButtonClicked();
//}

void RecipientView::setTableFilter(const int column, const QString &searchString)
{
    if (!mRecipients)
        return;

    mFilteredModel->setFilterKeyColumn(column);
    mFilteredModel->setFilterWildcard(searchString);

    qDebug () << "RecipientView::setTableFilter";
    qDebug () << "column: " << column;
    qDebug () << "searchString: " << searchString;

    mRecipients->update();
    update();
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

void RecipientView::onTableSelectionChanged (const QItemSelection &itemNew, const QItemSelection &itemOld)
{
    if (!mViewDataRecipient)
        return;

    Q_UNUSED(itemOld);
    Q_UNUSED(itemNew);

    mRecipientNameEdit->setStyleSheet ("");
    mRecipientNameEdit->setText("");

    int row = mRecipients->selectionModel()->currentIndex().row();
    mViewDataRecipient->setRecipientName(mRecipients->model()->index(row, 1).data().toString ());
    QString recipientType = mRecipients->model()->index(row, 2).data().toString ();
    mViewDataRecipient->setRecipientType(recipientType);
    mViewDataRecipient->setRecipientStreet(mRecipients->model()->index(row, 3).data().toString ());
    mViewDataRecipient->setRecipientStreetNumber(mRecipients->model()->index(row, 4).data().toInt ());
    mViewDataRecipient->setRecipientAreaCode(mRecipients->model()->index(row, 5).data().toInt ());
    mViewDataRecipient->setRecipientCity(mRecipients->model()->index(row, 6).data().toString());

    if (!itemNew.isEmpty())
    {
        enableButton(2, true);
        mRowSelected = true;
    }
    else
    {
        disableButton(2, true);
        mRowSelected = false;
    }

    if ("Firma" == recipientType)
    {
        mRecipientNameEdit->setEnabled(true);
        mViewDataRecipient->setSignatureName(mRecipientNameEdit->text());
    }
    else
    {
        mRecipientNameEdit->setEnabled(false);
        mViewDataRecipient->setSignatureName(mViewDataRecipient->getRecipientName());

    }

    update();
}

void RecipientView::onSelectedDateChanged (QDate date)
{
    if (!mViewDataRecipient)
        return;

    QString dateDeadline;

    int day = 0;
    int month = 0;
    int year = 0;

    date.getDate (&year, &month, &day);

    dateDeadline.append(QString::number(day));
    dateDeadline.append(".");
    dateDeadline.append(QString::number(month));
    dateDeadline.append(".");
    dateDeadline.append(QString::number(year));

    mViewDataRecipient->setDeadlineDate(dateDeadline);
}

void RecipientView::onKeychainStatusChanged (Database::KeychainStatus newStatus)
{
    dataInterface()->setNewKeychainStatusId(newStatus);
    //mViewDataRecipient->setNewKeychainStatus(newStatus);
}

RecipientView::~RecipientView()
{
}

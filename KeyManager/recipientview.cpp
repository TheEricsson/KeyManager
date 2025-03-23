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

#include "winsubmenu.h"
#include "globals.h"

RecipientView::RecipientView(QWidget *parent) : WinSubmenu {parent}
{
    mRecipients = 0;
    mFilteredModel = 0;
    mRowSelected = false;

    mFilteredModel = new QSortFilterProxyModel (this);

    setHeader("Empfänger auswählen");

    //QVBoxLayout* layout = new QVBoxLayout (this);

    QHBoxLayout* searchLayout = new QHBoxLayout ();
    QLabel *searchLabel = new QLabel ("Suche", this);
    QLineEdit *searchField = new QLineEdit (this);
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchField);
    layout()->addItem(searchLayout);

    mRecipients = new QTableView (this);
    mRecipients->setModel(mFilteredModel);
    layout()->addWidget(mRecipients);

    setMenuButtons(UiSpecs::BackButton, UiSpecs::AddRecipientButton, UiSpecs::NextButton);
    disableButton(2, true);

    connect (mRecipients->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onTableSelectionChanged(const QItemSelection &, const QItemSelection &)));
    connect (searchField, SIGNAL (textChanged(const QString &)), this, SLOT (setTableFilter(const QString &)));
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
    mFilteredModel->setFilterKeyColumn(-1); //set filter to search in all columns
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

bool RecipientView::getRecipientData (RecipientData &data)
{
    if (!mRowSelected)
        return false;

    int row = mRecipients->selectionModel()->currentIndex().row();
    data.name = mRecipients->model()->index(row, 1).data().toString ();

    if ("Firma" == mRecipients->model()->index(row, 2).data().toString ())
        data.type = RecipientType::Company;
    else if ("Privatperson" == mRecipients->model()->index(row, 2).data().toString ())
        data.type = RecipientType::PrivatePerson;
    else if ("Mitarbeiter" == mRecipients->model()->index(row, 2).data().toString ())
        data.type = RecipientType::Employee;
    else
    {
        qDebug () << "RecipientView::getRecipientData (RecipientData &data): " << "RecipientType not recognized, check string comparison";
        return false;
    }

    data.street = mRecipients->model()->index(row, 3).data().toString ();
    data.number = mRecipients->model()->index(row, 4).data().toString ();
    data.areaCode = mRecipients->model()->index(row, 5).data().toString ();
    data.city = mRecipients->model()->index(row, 6).data().toString ();

    return true;
}

void RecipientView::reset()
{
    disableButton(2, true);
    mRecipients->clearSelection();
    mRowSelected = false;
}

void RecipientView::setTableFilter(const QString &text)
{
    qDebug  () << "RecipientView::setTableFilter(const QString &text): " << text;

    if (!mRecipients)
        return;

    mFilteredModel->setFilterWildcard(text);
    mRecipients->update();
    update ();
}

void RecipientView::onNextBtnClicked ()
{
    // if (mRecipientName->isEnabled() && "" == mRecipientName->text())
    // {
    //     mRecipientName->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");

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
    emit nextButtonClicked ();
}

void RecipientView::onPreviousBtnClicked ()
{
    emit previousButtonClicked ();
}

void RecipientView::onNewRecipientBtnClicked ()
{
    emit newRecipientButtonClicked();
}

void RecipientView::onTableSelectionChanged (const QItemSelection &itemNew, const QItemSelection &itemOld)
{
    Q_UNUSED(itemOld);
    Q_UNUSED(itemNew);

    int row = mRecipients->selectionModel()->currentIndex().row();
    QString recipientType = mRecipients->model()->index(row, 2).data().toString ();

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

    update();
}

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
#include "globals.h"

RecipientView::RecipientView(QWidget *parent)
    : QWidget{parent}
{
    mRecipients = 0;
    mFilteredModel = 0;

    mFilteredModel = new QSortFilterProxyModel (this);

    QVBoxLayout* layout = new QVBoxLayout (this);

    QHBoxLayout* searchLayout = new QHBoxLayout (this);
    QLabel *searchLabel = new QLabel ("Suche", this);
    QLineEdit *searchField = new QLineEdit (this);
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchField);
    layout->addLayout(searchLayout);

    mRecipients = new QTableView (this);
    mRecipients->setModel(mFilteredModel);
    layout->addWidget(mRecipients);

    QPushButton* btnNewRecipient = new QPushButton (this);
    btnNewRecipient->setIcon(QIcon(":/images/addRecipient.jpeg"));
    btnNewRecipient->setIconSize(QSize(UiSpecs::buttonWidth,UiSpecs::buttonHeight));
    layout->addWidget(btnNewRecipient);

    QPushButton* btnPrevious = new QPushButton (this);
    btnPrevious->setIcon(QIcon(":/images/menu_back.png"));
    btnPrevious->setIconSize(QSize(UiSpecs::buttonWidth,UiSpecs::buttonHeight));

    mBtnNext = new QPushButton (this);
    mBtnNext->setIcon(QIcon(":/images/menu_next.png"));
    mBtnNext->setIconSize(QSize(UiSpecs::buttonWidth,UiSpecs::buttonHeight));
    mBtnNext->setDisabled(true);

    QHBoxLayout* hLayout = new QHBoxLayout (this);
    hLayout->addWidget(btnPrevious);
    hLayout->addWidget(mBtnNext);

    layout->addLayout(hLayout);

    setLayout(layout);

    connect (btnPrevious, SIGNAL (clicked()), this, SLOT (onPreviousBtnClicked()));
    connect (mBtnNext, SIGNAL (clicked()), this, SLOT (onNextBtnClicked()));
    connect (btnNewRecipient, SIGNAL (clicked()), this, SLOT (onNewRecipientBtnClicked()));
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

    return true;
}

void RecipientView::reset()
{
    mBtnNext->setDisabled(true);
    mRecipients->clearSelection();
}

void RecipientView::setTableFilter(const QString &text)
{
    qDebug  () << "RecipientView::setTableFilter(const QString &text): " << text;

    if (!mRecipients)
        return;

    mFilteredModel->setFilterWildcard(text);
    mRecipients->update();
}

void RecipientView::onNextBtnClicked ()
{
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

    if (!itemNew.isEmpty())
        mBtnNext->setEnabled(true);
    else
        mBtnNext->setEnabled(false);
}

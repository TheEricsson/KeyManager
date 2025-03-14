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

    mRecipients = new QTableView;
    mRecipients->setModel(mFilteredModel);
    layout->addWidget(mRecipients);

    QPushButton* btnNewRecipient = new QPushButton ();
    btnNewRecipient->setIcon(QIcon(":/images/addRecipient.jpeg"));
    btnNewRecipient->setIconSize(QSize(UiDimensions::buttonWidth,UiDimensions::buttonHeight));
    layout->addWidget(btnNewRecipient);

    QPushButton* btnPrevious = new QPushButton ();
    btnPrevious->setIcon(QIcon(":/images/menu_back.png"));
    btnPrevious->setIconSize(QSize(UiDimensions::buttonWidth,UiDimensions::buttonHeight));

    QPushButton* btnNext = new QPushButton ();
    btnNext->setIcon(QIcon(":/images/menu_next.png"));
    btnNext->setIconSize(QSize(UiDimensions::buttonWidth,UiDimensions::buttonHeight));
    btnNext->setDisabled(true);

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(btnPrevious);
    hLayout->addWidget(btnNext);

    layout->addLayout(hLayout);

    setLayout(layout);

    connect (btnPrevious, SIGNAL (clicked()), this, SLOT (onPreviousBtnClicked()));
    connect (btnNext, SIGNAL (clicked()), this, SLOT (onNextBtnClicked()));
    connect (btnNewRecipient, SIGNAL (clicked()), this, SLOT (onNewRecipientBtnClicked()));

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
    model->setHeaderData(2, Qt::Horizontal, tr("Straße"), Qt::DisplayRole);
    model->setHeaderData(3, Qt::Horizontal, tr("Hausnummer"), Qt::DisplayRole);
    model->setHeaderData(4, Qt::Horizontal, tr("PLZ"), Qt::DisplayRole);
    model->setHeaderData(5, Qt::Horizontal, tr("Stadt"), Qt::DisplayRole);

    mFilteredModel->setSourceModel(model);
    mFilteredModel->setFilterKeyColumn(-1); //set filter to search in all columns
    mFilteredModel->setFilterCaseSensitivity(Qt::CaseInsensitive); // ignore capital letters

    mRecipients->hideColumn(0); //don't show table index

    mRecipients->setSortingEnabled(true);
    mRecipients->setEditTriggers(QTableView::NoEditTriggers);
    mRecipients->setSelectionMode(QTableView::SingleSelection);

    mRecipients->show();

    mRecipients->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mRecipients->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    mRecipients->resizeColumnsToContents();

    return true;
}

void RecipientView::setTableFilter(const QString &text)
{
    qDebug  () << "RecipientView::setTableFilter(const QString &text): " << text;

    if (!mRecipients)
        return;

    mFilteredModel->setFilterWildcard(text);
    mRecipients->update();

    // QSqlRelationalTableModel *model = (QSqlRelationalTableModel*)mRecipients->model();

    // if(model)
    // {
    //     QString searchString ("name='");
    //     searchString.append(text);
    //     searchString.append ("'");

    //     qDebug () << "RecipientView::setTableFilter: " << searchString;

    //     mFilteredModel->setFilterWildcard(text);
    //     //model->setFilter(searchString);
    //     model->select();
    //     mRecipients->update();
    // }
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

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

    QHBoxLayout *recipientNameLayout = new QHBoxLayout (this);
    QLabel* recipientNameLabel = new QLabel ("Empfängername", this);
    mRecipientName = new QLineEdit (this);
    recipientNameLayout->addWidget(recipientNameLabel);
    recipientNameLayout->addWidget(mRecipientName);
    mRecipientName->setEnabled(false);
    layout->addLayout(recipientNameLayout);

    QPushButton* btnNewRecipient = new QPushButton (this);
    btnNewRecipient->setIcon(QIcon(":/images/addRecipient.jpeg"));
    btnNewRecipient->setIconSize(QSize(UiSpecs::buttonWidth,UiSpecs::buttonHeight));

    QPushButton* btnPrevious = new QPushButton (this);
    btnPrevious->setIcon(QIcon(":/images/menu_back.png"));
    btnPrevious->setIconSize(QSize(UiSpecs::buttonWidth,UiSpecs::buttonHeight));

    mBtnNext = new QPushButton (this);
    mBtnNext->setIcon(QIcon(":/images/menu_next.png"));
    mBtnNext->setIconSize(QSize(UiSpecs::buttonWidth,UiSpecs::buttonHeight));
    mBtnNext->setDisabled(true);

    QHBoxLayout* hLayout = new QHBoxLayout (this);
    hLayout->addWidget(btnPrevious);
    hLayout->addWidget(btnNewRecipient);
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
    mRecipientName->setDisabled(true);
    mRecipientName->setText("");
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
    if (mRecipientName->isEnabled() && "" == mRecipientName->text())
    {
        mRecipientName->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");

        QMessageBox msgBox;
        msgBox.setText("Unvollständige Eingaben");
        msgBox.setInformativeText("Es wurde ein Unternehmen als Empfänger \n"
                                 "ausgewählt. Der Name der Person, die den \n"
                                  "Schlüssel entgegennimmt, wurde nicht angegeben.");

        msgBox.setStandardButtons(QMessageBox::Ignore | QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        int selection = msgBox.exec();

        bool ignore = false;

        switch (selection)
        {
            case QMessageBox::Ignore:
                ignore = true;
                break;
            case QMessageBox::Ok:
                break;
            default:
                break;
        }

        if (!ignore)
            return;
    }
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

    qDebug () << "onTableSelectionChanged, type: " << recipientType;

    if ("Firma" == recipientType)
    {
        mRecipientName->setEnabled(true);
    }
    else
    {
        mRecipientName->setEnabled(false);
        mRecipientName->setText("");
    }

    if (!itemNew.isEmpty())
        mBtnNext->setEnabled(true);
    else
    {
        mBtnNext->setEnabled(false);
        mRecipientName->setEnabled(false);
        mRecipientName->setText("");
    }
}

#include "tableview.h"

#include <QSqlTableModel>
#include <QTableView>
#include <QHBoxLayout>
#include <QPushButton>

TableView::TableView(const QString &tableName, QWidget *parent)
    : QWidget{parent}
{
    mAdresses = new QSqlTableModel(this);
    mAdresses->setTable("keys");
    mAdresses->setEditStrategy(QSqlTableModel::OnFieldChange);
    mAdresses->select();

    mAdresses->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mAdresses->setHeaderData(1, Qt::Horizontal, tr("Straße"));
    mAdresses->setHeaderData(2, Qt::Horizontal, tr("Hausnummer"));
    mAdresses->setHeaderData(3, Qt::Horizontal, tr("PLZ"));
    mAdresses->setHeaderData(4, Qt::Horizontal, tr("Ort"));

    QTableView *adresses = new QTableView;
    adresses->setModel(mAdresses);

    mKeys = new QSqlTableModel(this);
    mKeys->setTable("keyStates");
    mKeys->setEditStrategy(QSqlTableModel::OnFieldChange);
    mKeys->select();

    mKeys->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mKeys->setHeaderData(1, Qt::Horizontal, tr("BarcodeMandant"));
    mKeys->setHeaderData(2, Qt::Horizontal, tr("BarcodeID"));
    mKeys->setHeaderData(3, Qt::Horizontal, tr("Art"));
    mKeys->setHeaderData(4, Qt::Horizontal, tr("Anzahl"));
    mKeys->setHeaderData(4, Qt::Horizontal, tr("Beschreibung"));

    QTableView *keys = new QTableView;
    keys->setModel(mKeys);

    QPushButton* btnPrevious = new QPushButton ();
    btnPrevious->setIcon(QIcon(":/images/menu_back.png"));
    btnPrevious->setIconSize(QSize(75,75));

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QSpacerItem *spacer = new QSpacerItem (1, 10);
    mainLayout->addWidget(adresses);
    mainLayout->addSpacerItem(spacer);
    mainLayout->addWidget(keys);
    mainLayout->addWidget(btnPrevious);

    setLayout(mainLayout);
    setWindowTitle(tr("Übersicht"));
    show();

    adresses->resizeColumnsToContents();
    keys->resizeColumnsToContents();

    connect (btnPrevious, SIGNAL (clicked()), this, SLOT (onPreviousBtnClicked()));
}

void TableView::onPreviousBtnClicked ()
{
    emit previousButtonClicked ();
}

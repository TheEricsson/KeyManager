#include <QSqlRelationalTableModel>
#include <QTableView>
#include "keychainstatusview.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

KeychainStatusView::KeychainStatusView(QWidget *parent)
    : QWidget{parent}
{
    init ();
}

KeychainStatusView::KeychainStatusView(int aLocaleId, int aKeychainId, QWidget *parent)
        : QWidget{parent}
{
    init ();
    //todo: implement use of aLocaleId, aKeychainId
}

void KeychainStatusView::init()
{
    mKeyStatus = new QSqlRelationalTableModel(this);
    mKeyStatus->setTable("keys");
    mKeyStatus->select();

    QTableView *keychain = new QTableView;
    keychain->setModel(mKeyStatus);

    // mKeyStatus->setHeaderData(0, Qt::Horizontal, tr("ID"));
    // mKeyStatus->setHeaderData(1, Qt::Horizontal, tr("Straße"));
    // mKeyStatus->setHeaderData(2, Qt::Horizontal, tr("Hausnummer"));
    // mKeyStatus->setHeaderData(3, Qt::Horizontal, tr("PLZ"));
    // mKeyStatus->setHeaderData(4, Qt::Horizontal, tr("Ort"));

    QPushButton* btnPrevious = new QPushButton ();
    btnPrevious->setIcon(QIcon(":/images/menu_back.png"));
    btnPrevious->setIconSize(QSize(75,75));

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(keychain);
    mainLayout->addWidget(btnPrevious);

    setLayout(mainLayout);
    setWindowTitle(tr("Übersicht"));
    show();

    keychain->resizeColumnsToContents();

    connect (btnPrevious, SIGNAL (clicked()), this, SLOT (onPreviousBtnClicked()));
}

void KeychainStatusView::onPreviousBtnClicked ()
{
    emit previousButtonClicked ();
}

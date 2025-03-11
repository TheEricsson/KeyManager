#include <QSqlRelationalTableModel>
#include <QTableView>
#include "keychainstatusview.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QSqlQuery>

KeychainStatusView::KeychainStatusView(QWidget *parent)
    : QWidget{parent}
{
    mKeys = 0;
    init ();
}

KeychainStatusView::KeychainStatusView(int aLocaleId, int aKeychainId, QWidget *parent)
        : QWidget{parent}
{
    init ();
    //todo: implement use of aLocaleId, aKeychainId
}

void KeychainStatusView::setModel (QSqlQueryModel* model)
{
    if (model)
    {
        if (mKeys)
        {
            qDebug () << "KeychainStatusView::setModel";
            mKeys->setModel(model);
            mKeys->show();
            mKeys->resizeColumnsToContents();
        }
    }
}

void KeychainStatusView::init()
{
    QPushButton* btnPrevious = new QPushButton ();
    btnPrevious->setIcon(QIcon(":/images/menu_back.png"));
    btnPrevious->setIconSize(QSize(75,75));

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mKeys = new QTableView ();

    mainLayout->addWidget(mKeys);
    mainLayout->addWidget(btnPrevious);

    setLayout(mainLayout);
    setWindowTitle(tr("Übersicht"));
    show();

    connect (btnPrevious, SIGNAL (clicked()), this, SLOT (onPreviousBtnClicked()));
}

void KeychainStatusView::onPreviousBtnClicked ()
{
    emit previousButtonClicked ();
}

#include "handoverview.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

HandoverView::HandoverView(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* layout = new QVBoxLayout;
    QTableView* view = new QTableView;

    QPushButton* btnPrevious = new QPushButton ();
    btnPrevious->setIcon(QIcon(":/images/menu_back.png"));
    btnPrevious->setIconSize(QSize(75,75));

    QPushButton* btnNext = new QPushButton ();
    btnNext->setIcon(QIcon(":/images/menu_next.png"));
    btnNext->setIconSize(QSize(75,75));
    btnNext->setDisabled(true);

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(btnPrevious);
    hLayout->addWidget(btnNext);

    layout->addWidget(view);
    layout->addLayout(hLayout);

    setLayout(layout);
}

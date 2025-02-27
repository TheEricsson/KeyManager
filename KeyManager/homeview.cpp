#include "homeview.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QLabel>

HomeView::HomeView(QWidget *parent)
    : QWidget{parent}
{
    mBtnScan = 0;
    mBtnSearch = 0;
    mBtnManage = 0;
    mBtnExit = 0;
    mLayout = 0;

    QPixmap logo (":/images/logo.png");
    QLabel *label = new QLabel();
    label->setScaledContents(true);
    label->setMaximumHeight(100);
    label->setMaximumWidth(300);
    label->setPixmap(logo);

    mBtnScan = new QPushButton (/*tr("Code Scannen")*/);
    mBtnSearch = new QPushButton (/*tr("Manuell Suchen")*/);
    mBtnManage = new QPushButton (/*tr("Einstellungen")*/);
    mBtnExit = new QPushButton (/*tr("Beenden")*/);

    int desiredDim = 75;

    mBtnScan->setIcon(QIcon(":/images/shutter.svg"));
    mBtnScan->setIconSize(QSize(desiredDim,desiredDim));

    mBtnSearch->setIcon(QIcon(":/images/search.svg"));
    mBtnSearch->setIconSize(QSize(desiredDim,desiredDim));

    mBtnManage->setIcon(QIcon(":/images/settings.svg"));
    mBtnManage->setIconSize(QSize(desiredDim,desiredDim));

    mBtnExit->setIcon(QIcon(":/images/exit.svg"));
    mBtnExit->setIconSize(QSize(desiredDim,desiredDim));

    /*QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(mBtnScan,0);
    buttonLayout->addWidget(mBtnSearch,1);
    buttonLayout->addWidget(mBtnManage,2);
    buttonLayout->addWidget(mBtnExit,3);*/

    mLayout = new QGridLayout;
    //mLayout->addLayout(buttonLayout,1,0);
    mLayout->addWidget(label,0,0,1,2);
    mLayout->addWidget(mBtnScan,1,0);
    mLayout->addWidget(mBtnSearch,1,1);
    mLayout->addWidget(mBtnManage,2,0);
    mLayout->addWidget(mBtnExit,2,1);

    setLayout(mLayout);
    setWindowTitle(tr("Key-Manager"));

    connect (mBtnScan,SIGNAL(released()), this, SLOT (onScanButtonReleased()));

    /*connect (btnScan,SIGNAL(released()), this, SLOT (onScanButtonReleased()));
    connect (btnSearch,SIGNAL(released()), this, SLOT (onSearchButtonReleased()));
    connect (btnManage,SIGNAL(released()), this, SLOT (onManageButtonReleased()));
    connect (btnExit, &QPushButton::clicked, this, &QCoreApplication::quit, Qt::QueuedConnection);*/
}

void HomeView::onScanButtonReleased()
{
    emit showScannerView ();
}

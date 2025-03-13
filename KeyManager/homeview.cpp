#include "homeview.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QLabel>

#include "globals.h"

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
    label->setMaximumHeight(UiDimensions::logoHeight);
    label->setMaximumWidth(UiDimensions::logoWidth);
    label->setPixmap(logo);    

    mBtnScan = new QPushButton (/*tr("Code Scannen")*/);
    mBtnSearch = new QPushButton (/*tr("Manuell Suchen")*/);
    mBtnManage = new QPushButton (/*tr("Einstellungen")*/);
    mBtnExit = new QPushButton (/*tr("Beenden")*/);

    mBtnScan->setIcon(QIcon(":/images/scan_barcode.png"));
    mBtnScan->setIconSize(QSize(UiDimensions::buttonWidth,UiDimensions::buttonHeight));

    mBtnSearch->setIcon(QIcon(":/images/search.svg"));
    mBtnSearch->setIconSize(QSize(UiDimensions::buttonWidth,UiDimensions::buttonHeight));

    mBtnManage->setIcon(QIcon(":/images/settings.svg"));
    mBtnManage->setIconSize(QSize(UiDimensions::buttonWidth,UiDimensions::buttonHeight));

    mBtnExit->setIcon(QIcon(":/images/exit.svg"));
    mBtnExit->setIconSize(QSize(UiDimensions::buttonWidth,UiDimensions::buttonHeight));

    mLayout = new QGridLayout;
    mLayout->addWidget(label,0,0,1,2,Qt::AlignHCenter);
    mLayout->addWidget(mBtnScan,1,0);
    mLayout->addWidget(mBtnSearch,1,1);
    mLayout->addWidget(mBtnManage,2,0);
    mLayout->addWidget(mBtnExit,2,1);

    setLayout(mLayout);
    setWindowTitle(tr("Key-Manager"));

    connect (mBtnScan, SIGNAL(released()), this, SLOT (onScanButtonReleased()));
    connect (mBtnSearch, SIGNAL(released()), this, SLOT (onSearchButtonReleased()));

    /*connect (btnScan,SIGNAL(released()), this, SLOT (onScanButtonReleased()));
    connect (btnSearch,SIGNAL(released()), this, SLOT (onSearchButtonReleased()));
    connect (btnManage,SIGNAL(released()), this, SLOT (onManageButtonReleased()));
    connect (btnExit, &QPushButton::clicked, this, &QCoreApplication::quit, Qt::QueuedConnection);*/
}

void HomeView::onScanButtonReleased()
{
    emit showScannerView ();
}

void HomeView::onSearchButtonReleased()
{
    emit showTableView ();
}

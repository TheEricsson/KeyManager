#include "scannerview.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QCamera>
#include <QMediaDevices>
#include <QDebug>
#include <QStackedWidget>
#include <QLabel>
#include <QVideoWidget>
#include <QPermission>
#include <QApplication>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QScreenCapture>
#include <QLineEdit>

#if QT_CONFIG(permissions)
#include <QPermission>
#endif

ScannerView::ScannerView(QWidget *parent)
    : QWidget{parent}
{
    m_viewfinder = 0;

    // main Layout
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QLabel* header = new QLabel("Scannen Sie einen Barcode");
    layout->addWidget(header);

    // layout for the cam video/ pic screen
    m_viewfinder = new QVideoWidget ();

    layout->addWidget(m_viewfinder);
    layout->setStretch(1,2);

    // mandant text field
    QHBoxLayout *mandantIdLayout = new QHBoxLayout;
    QLabel *mandantDescr = new QLabel ("Mandant", this);
    mCustomerLabel = new QLabel (this);
    mandantIdLayout->addWidget (mandantDescr);
    mandantIdLayout->addWidget(mCustomerLabel);

    layout->addLayout(mandantIdLayout);

    // key text field
    QHBoxLayout *keyIdLayout = new QHBoxLayout;
    QLabel *keyDescr = new QLabel ("Schlüssel", this);
    mKeyLabel = new QLabel (this);
    keyIdLayout->addWidget (keyDescr);
    keyIdLayout->addWidget(mKeyLabel);

    layout->addLayout(keyIdLayout);

    //prev+retry+next button
    QHBoxLayout *menuLayout = new QHBoxLayout;

    QPushButton* btnPrevious = new QPushButton ();
    btnPrevious->setIcon(QIcon(":/images/menu_back.png"));
    btnPrevious->setIconSize(QSize(75,75));

    mBtnNext = new QPushButton ();
    mBtnNext->setIcon(QIcon(":/images/menu_next.png"));
    mBtnNext->setIconSize(QSize(75,75));
    mBtnNext->setDisabled(true);

    mBtnScan = new QPushButton ();
    mBtnScan->setIcon(QIcon(":/images/menu_retry.png"));
    mBtnScan->setIconSize(QSize(75,75));
    mBtnScan->setDisabled(true);

    menuLayout->addWidget(btnPrevious);
    menuLayout->addWidget(mBtnScan);
    menuLayout->addWidget(mBtnNext);

    layout->addLayout(menuLayout);

    connect (btnPrevious, SIGNAL (clicked()), this, SLOT (onPreviousBtnClicked()));
    connect (mBtnScan, SIGNAL (clicked()), this, SLOT (onScanBtnClicked()));
    connect (mBtnNext, SIGNAL (clicked()), this, SLOT (onNextBtnClicked()));

    setScannerState (ScannerState::READY);
}

void ScannerView::setScannerState (ScannerState aStatus)
{
    mScannerState = aStatus;

    switch (mScannerState)
    {
        case READY:
            qDebug () << "ScannerState is READY";
            mCustomerLabel->setText("---");
            mKeyLabel->setText("---");
            mBtnNext->setDisabled(true);
            mBtnScan->setDisabled(false);
            break;
        case SCANNING:
            mCustomerLabel->setText("---");
            mKeyLabel->setText("---");
            qDebug() <<  "ScannerState is SCANNING";
            mBtnNext->setDisabled(true);
            mBtnScan->setDisabled(true);
            break;
        case SCANSUCCEEDED:
            qDebug() <<  "ScannerState is SCANSUCCEEDED";
            mBtnNext->setDisabled(false);
            mBtnScan->setDisabled(false);
            break;
    }
}

void ScannerView::onPreviousBtnClicked ()
{
    emit previousButtonClicked ();
}

void ScannerView::onScanBtnClicked ()
{
    emit scanButtonClicked ();
}

void ScannerView::onNextBtnClicked()
{
    emit nextButtonClicked ();
}

QSize ScannerView::getViewfinderSize ()
{
    return m_viewfinder->size();
}

QVideoWidget* ScannerView::getViewfinder ()
{
    if (!m_viewfinder)
        return 0;
    return m_viewfinder;
}

void ScannerView::setCustomerLabel (QString aCustomerId)
{
    mCustomerLabel->setText(aCustomerId);
}

void ScannerView::setKeyLabel (QString aKeyId)
{
    mKeyLabel->setText(aKeyId);
}

const QString ScannerView::getCustomerLabel ()
{
    return mCustomerLabel->text();
}

const QString ScannerView::getKeyLabel()
{
    return mKeyLabel->text();
}

ScannerView::~ScannerView()
{
    if (m_viewfinder != 0)
    {
        delete m_viewfinder;
    }
}

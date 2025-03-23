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
#include "winsubmenu.h"

#if QT_CONFIG(permissions)
#include <QPermission>
#endif

ScannerView::ScannerView(QWidget *parent)
    : WinSubmenu {parent}
{
    setHeader("Scannen Sie einen Barcode");

    m_viewfinder = 0;

    // layout for the cam video/ pic screen
    m_viewfinder = new QVideoWidget ();
    m_viewfinder->setMinimumSize(300, 150);
    m_viewfinder->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);

    layout()->addWidget(m_viewfinder);
    //layout()->setStretch(1,2);

    // mandant text field
    QHBoxLayout *mandantIdLayout = new QHBoxLayout;
    QLabel *mandantDescr = new QLabel ("Mandant", this);
    mCustomerLabel = new QLabel (this);
    mandantIdLayout->addWidget (mandantDescr);
    mandantIdLayout->addWidget(mCustomerLabel);

    layout()->addItem(mandantIdLayout);

    // key text field
    QHBoxLayout *keyIdLayout = new QHBoxLayout;
    QLabel *keyDescr = new QLabel ("Schlüssel", this);
    mKeyLabel = new QLabel (this);
    keyIdLayout->addWidget (keyDescr);
    keyIdLayout->addWidget(mKeyLabel);

    layout()->addItem(keyIdLayout);

    setMenuButtons(UiSpecs::BackButton, UiSpecs::RepeatButton);

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
            break;
        case SCANNING:
            mCustomerLabel->setText("---");
            mKeyLabel->setText("---");
            qDebug() <<  "ScannerState is SCANNING";
            break;
        case SCANSUCCEEDED:
            qDebug() <<  "ScannerState is SCANSUCCEEDED";
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

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
#include "QZXing.h"
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QScreenCapture>

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

    layout->setStretch(1, 5);

    QPushButton* btnAbort = new QPushButton ("Abbrechen");
    layout->addWidget(btnAbort);

    connect (btnAbort, SIGNAL (clicked()), this, SLOT (onAbortBtnClicked()));
}

void ScannerView::onAbortBtnClicked ()
{
    emit abortScanner ();
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

ScannerView::~ScannerView()
{
    if (m_viewfinder != 0)
    {
        delete m_viewfinder;
    }
}

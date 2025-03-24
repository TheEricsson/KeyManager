#ifndef SCANNERVIEW_H
#define SCANNERVIEW_H

#include <QWidget>
#include <QSize>
#include "winsubmenu.h"

class QVideoWidget;
class QLabel;
class QPushButton;

enum ScannerState {
    READY,
    SCANNING,
    SCANSUCCEEDED,
};

class ScannerView : public WinSubmenu
{
    Q_OBJECT
public:
    explicit ScannerView(QWidget *parent = nullptr);
    void setScannerState (ScannerState aStatus);
    QVideoWidget* getViewfinder ();
    QSize getViewfinderSize ();
    void setCustomerLabel (QString aCustomerId);
    void setKeyLabel (QString aKeyId);
    const QString getCustomerLabel ();
    const QString getKeyLabel();
    ~ScannerView();

private:

    ScannerState mScannerState;
    QVideoWidget *m_viewfinder;
    QLabel *mCustomerLabel;
    QLabel *mKeyLabel;
};

#endif // SCANNERVIEW_H

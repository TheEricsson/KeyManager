#ifndef SCANNERVIEW_H
#define SCANNERVIEW_H

#include <QWidget>
#include <QSize>
#include "winsubmenu.h"
#include "QZXing.h"

class QVideoWidget;
class QLabel;
class QPushButton;
class Camera;

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
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    void setScannerState (ScannerState aStatus);
    QVideoWidget* getViewfinder ();
    QSize getViewfinderSize ();
    void setCustomerLabel (QString aCustomerId);
    void setKeyLabel (QString aKeyId);
    const QString getCustomerLabel ();
    const QString getKeyLabel();
    ~ScannerView();

signals:
    void keycodeRecognised (int keycode);

private slots:
    void onMenuBtnClicked (Gui::MenuButton btnType);
    void decodeFromVideoFrame ();
private:
    void startScanner ();
    void stopScanner ();
    Camera *mCameraInstance;
    QTimer *mGrabTimer;
    ScannerState mScannerState;
    QVideoWidget *m_viewfinder;
    QLabel *mCustomerLabel;
    QLabel *mKeyLabel;

    QZXing decoder;
};

#endif // SCANNERVIEW_H

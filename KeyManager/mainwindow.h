#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QPushButton;
class QGridLayout;
class ScannerView;
class QStackedLayout;
class Camera;
class HomeView;
class ScannerView;
class KeyScannedView;
class QTimer;

#ifdef ENCODERTEST
class QREncoderTest;
#endif

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

/*public slots:
    void onCloseScanner ();*/

private slots:
    void showScannerView ();
    void closeScannerView ();
    void onSearchButtonReleased ();
    void onManageButtonReleased ();
    void onKeyScanned ();
    void decodeImage (int requestId, const QImage &img);

private:
    QPushButton* btnScan;
    QPushButton* btnSearch;
    QPushButton* btnManage;
    QPushButton* btnExit;
    //QGridLayout* mainLayout;
    QStackedLayout* mLayout;

    HomeView *mHomeView;
    ScannerView *mScanView;
    KeyScannedView *mKeyScannedView;

    Camera *mCameraInstance;
    QTimer *mGrabTimer;

#ifdef ENCODERTEST
    QREncoderTest* qrencoderTest;
#endif
};
#endif // MAINWINDOW_H

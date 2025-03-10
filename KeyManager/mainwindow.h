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
class DatabaseImpl;
class TableView;
class KeychainStatusView;

#ifdef ENCODERTEST
class QREncoderTest;
#endif

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void initDatabase ();
    ~MainWindow();

private slots:
    void showScannerView ();
    void showTableView ();
    void showKeychainStatusView (int aLocale, int aKeychainId);
    void closeKeychainStatusView ();
    void closeScannerView ();
    void closeTableView ();
    void onSearchButtonReleased ();
    void onManageButtonReleased ();
    void decodeImage (int requestId, const QImage &img);
    void decodeFromVideoFrame ();

private:
    void playSound ();
    void handleScannedKey();

    QPushButton* btnScan;
    QPushButton* btnSearch;
    QPushButton* btnManage;
    QPushButton* btnExit;
    QStackedLayout* mLayout;

    HomeView *mHomeView;
    ScannerView *mScanView;
    KeyScannedView *mKeyScannedView;
    TableView *mTableView;
    KeychainStatusView *mKeychainStatusView;

    Camera *mCameraInstance;
    QTimer *mGrabTimer;
    DatabaseImpl *mDatabase;

#ifdef ENCODERTEST
    QREncoderTest* qrencoderTest;
#endif
};
#endif // MAINWINDOW_H

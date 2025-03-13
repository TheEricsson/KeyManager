#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>

class QPushButton;
class QGridLayout;
class ScannerView;
class QStackedLayout;
class Camera;
class HomeView;
class ScannerView;
class QTimer;
class DatabaseImpl;
class TableView;
class KeychainStatusView;
class HandoverView;

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
    void showHandoverView ();
    bool showKeychainStatusView (int aBarcode);
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
    TableView *mTableView;
    KeychainStatusView *mKeychainStatusView;
    HandoverView *mHandoverView;

    Camera *mCameraInstance;
    QTimer *mGrabTimer;
    DatabaseImpl *mDatabase;

    //QSqlQueryModel mKcStatusModel;
    QSqlRelationalTableModel *mKeysOverviewModel;
    QSqlRelationalTableModel *mKeychainModel;

#ifdef ENCODERTEST
    QREncoderTest* qrencoderTest;
#endif
};
#endif // MAINWINDOW_H

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
class RecipientView;
class AddRecipientView;

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
    void showRecipientView ();
    void closeRecipientView ();
    void showAddRecipientView ();
    void closeAddRecipientView();
    void addRecipientViewSubmitted ();
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
    RecipientView *mRecipientView;
    AddRecipientView *mAddRecipientView;

    Camera *mCameraInstance;
    QTimer *mGrabTimer;
    DatabaseImpl *mDatabase;

    QSqlRelationalTableModel *mKeysOverviewModel;
    QSqlRelationalTableModel *mKeychainModel;
    QSqlRelationalTableModel *mRecipientsModel;

#ifdef ENCODERTEST
    QREncoderTest* qrencoderTest;
#endif
};
#endif // MAINWINDOW_H

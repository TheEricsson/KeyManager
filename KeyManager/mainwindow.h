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
class HandoverView;

#ifdef ENCODERTEST
class QREncoderTest;
#endif

class MainWindow : public QWidget
{
    enum eViewState
    {
        None = -1,
        Home = 0,
        Scanner = 1,
        Search = 2,
        Recipient = 3,
        Keychain = 4,
        AddRecipient = 5
    };

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
    void showHandoverView ();
    void closeScannerView ();
    void closeTableView ();
    void onSearchButtonReleased ();
    void onManageButtonReleased ();
    void decodeImage (int requestId, const QImage &img);
    void decodeFromVideoFrame ();

private:
    void setView (QWidget* view);
    void playSound ();
    void handleScannedKey();

    int mLastView;

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
    HandoverView *mHandoverView;

    Camera *mCameraInstance;
    QTimer *mGrabTimer;
    DatabaseImpl *mDatabase;

    QSqlRelationalTableModel *mKeysOverviewModel;
    QSqlRelationalTableModel *mKeychainModel;
    QSqlRelationalTableModel *mRecipientsModel;

    eViewState mViewState;

#ifdef ENCODERTEST
    QREncoderTest* qrencoderTest;
#endif
};
#endif // MAINWINDOW_H

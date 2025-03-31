#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include "QZXing.h"
#include "globals.h"

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
class ReturnDateView;
class HandoutSummaryView;
class DataObjectHandover;
class AnnotationView;
class WinSubmenu;
class ViewStackManager;
class mDataInterface;

#include "datainterface.h"

#ifdef ENCODERTEST
class QREncoderTest;
#endif

class ViewStack;

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
    void onMenuBtnClicked (Gui::MenuButton btnType);
    void onKeycodeRecognised (int keyCode);

    // void showTableView ();
    // void showRecipientView ();
    // void closeRecipientView ();
    // void showAddRecipientView ();
    // void closeAddRecipientView();
    // void showReturnDateView();
    // void closeReturnDateView ();
    // void showAnnotationView ();
    // void closeAnnotationView ();
    // void addRecipientViewSubmitted ();
    // bool showKeychainStatusView (int aBarcode);
    // void closeKeychainStatusView ();
    // void showHandoverView ();
    // void closeHandoverView ();
    // void showHandoutSummaryView ();
    // void closeHandoutSummaryView ();
    // void onSearchButtonReleased ();
    // void onManageButtonReleased ();
    // void decodeImage (int requestId, const QImage &img);
    // void decodeFromVideoFrame ();

private:
    void registerView (WinSubmenu *view);
    void setView (QWidget* view);
    void playSound ();
    void handleScannedKey();

    //int mLastView;

    QPushButton* btnScan;
    QPushButton* btnSearch;
    QPushButton* btnManage;
    QPushButton* btnExit;
    //QStackedLayout* mLayout;
    QStackedLayout *mViewStack;
    ViewStackManager *mViewStackManager;

    HomeView *mHomeView;
    ScannerView *mScanView;
    TableView *mTableView;
    KeychainStatusView *mKeychainStatusView;
    RecipientView *mRecipientView;
    AddRecipientView *mAddRecipientView;
    HandoverView *mHandoverView;
    ReturnDateView *mReturnDateView;
    HandoutSummaryView *mHandoutSummaryView;
    AnnotationView *mAnnotationView;

    //Camera *mCameraInstance;
    //QTimer *mGrabTimer;
    //DatabaseImpl *mDatabase;

    // QSqlRelationalTableModel *mKeysOverviewModel;
    // QSqlRelationalTableModel *mKeychainModel;
    // QSqlRelationalTableModel *mRecipientsModel;

    QZXing decoder;

    //DataObjectHandover *mDataHandover;

    DataInterface *mDataInterface;

#ifdef ENCODERTEST
    QREncoderTest* qrencoderTest;
#endif
};
#endif // MAINWINDOW_H

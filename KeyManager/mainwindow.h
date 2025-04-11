#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "globals.h"

class QPushButton;
class QGridLayout;
class ScannerView;
class QStackedLayout;
class HomeView;
class ScannerView;
class TableView;
class KeychainStatusView;
class RecipientView;
class AddRecipientView;
class HandoverView;
class AnnotationView;
class WinSubmenu;
class ViewStackManager;
class DataInterface;
class AddKeychainView;
class EditKeyView;
class IOInterfaceSQLITE;
class ToolsView;

class ViewStack;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void initDatabase ();
    ~MainWindow();

private slots:
    void onMenuBtnClicked (Gui::MenuButton btnType);

private:
    void registerView (WinSubmenu *view);
    void setView (QWidget* view);

    QPushButton* btnScan;
    QPushButton* btnSearch;
    QPushButton* btnManage;
    QPushButton* btnExit;

    QStackedLayout *mViewStack;
    ViewStackManager *mViewStackManager;

    // views for the key management
    HomeView *mHomeView;
    ScannerView *mScanView;
    TableView *mTableView;
    KeychainStatusView *mKeychainStatusView;
    RecipientView *mRecipientView;
    AddRecipientView *mAddRecipientView;
    HandoverView *mHandoverView;
    AnnotationView *mAnnotationView;
    AddKeychainView *mAddKeychainView;
    EditKeyView *mEditKeyView;

    // views in the section settings+tools
    ToolsView *mToolsView;

    DataInterface *mDataInterface;
    IOInterfaceSQLITE *mDbInterface;
};
#endif // MAINWINDOW_H

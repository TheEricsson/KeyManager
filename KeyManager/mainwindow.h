#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include "globals.h"
#include <QMainWindow>

class QPushButton;
class QGridLayout;
class ScannerView;
class QStackedWidget;
class HomeView;
class ScannerView;
class SearchView;
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
class IOInterface;
class ToolsViewCodeGenerator;
class ReturnDateView;
class ViewStack;
class SettingsViewDb;
class ToolsViewDataAdministration;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setDataInterface (DataInterface *dataInterface);
    void setIOInterface (IOInterface *ioInterface);
    void setViewMode (ViewMode::Value mode);
    void init();
    bool reallyQuit();
    ~MainWindow();

private slots:
    void onMenuBtnClicked (Gui::MenuButton btnType);

private:
    void registerView (WinSubmenu *view);
    void setView (QWidget* view);
/*#if defined(Q_OS_ANDROID)
    void accessAllFiles();
#endif*/
    void doBackup();

    QPushButton* btnScan;
    QPushButton* btnSearch;
    QPushButton* btnManage;
    QPushButton* btnExit;

    QStackedWidget *mViewStack;
    ViewStackManager *mViewStackManager;

    // views for the key management
    HomeView *mHomeView;
    ScannerView *mScanView;
    SearchView *mSearchView;
    KeychainStatusView *mKeychainStatusView;
    RecipientView *mRecipientView;
    AddRecipientView *mAddRecipientView;
    HandoverView *mHandoverView;
    AnnotationView *mAnnotationView;
    AddKeychainView *mAddKeychainView;
    EditKeyView *mEditKeyView;
    ReturnDateView *mReturnDateView;

    // views in the section tools
    ToolsViewCodeGenerator *mToolsViewCodegenerator;
    ToolsViewDataAdministration *mToolsViewDataAdministration;

    // views in the section settings
    SettingsViewDb *mSettingsViewDb;

    DataInterface *mDataInterface;
    IOInterface *mDbInterface;
};
#endif // MAINWINDOW_H

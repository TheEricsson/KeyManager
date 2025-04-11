#ifndef KEYCHAINSTATUSVIEW_H
#define KEYCHAINSTATUSVIEW_H

#include <QObject>
#include <QWidget>
#include "winsubmenu.h"
#include "viewdatakeychain.h"

class QLabel;
class QSqlQueryModel;
class QTableView;
class QSqlRelationalTableModel;
class QPushButton;
class KeychainDataHandover;
class QSortFilterProxyModel;

class KeychainStatusView : public WinSubmenu
{
    Q_OBJECT
public:
    explicit KeychainStatusView(QWidget *parent = nullptr);
    ~KeychainStatusView ();
    void setNextBtnText ();
    bool setKeychainModel (QSqlRelationalTableModel* model);
    bool setKeysModel (QSqlRelationalTableModel* model);
    bool setKeychainHistoryModel (QSqlRelationalTableModel* model);
    void setKeychainImagePath (const QString& imgPath);

private slots:
    void keyCodeBtnClicked();

private:
    virtual void showEvent(QShowEvent *);
    QSqlRelationalTableModel *mKeyModel;
    //QSqlRelationalTableModel *mKeychainModel;
    QSqlRelationalTableModel *mHistoryModel;
    //QSortFilterProxyModel *mFilteredKeychainModel;
    QSortFilterProxyModel *mFilteredKeyModel;
    QSortFilterProxyModel *mFilteredHistoryModel;

    //QSqlQueryModel *mKeyOverview;
    QTableView *mKeys;
    //QTableView *mKeychain;
    QTableView *mHistory;
    QLabel *mKeychainKeycode;
    QLabel *mKeychainStatus;
    QLabel *mKeychainInternalLocation;
    QLabel *mKeychainStreet;
    QLabel *mKeychainStreetNumber;
    QLabel *mKeychainCity;

    QImage *mKeychainImage;
    QPushButton *mKeysImgPreview;
    ViewDataKeychain *mViewData;
};

#endif // KEYCHAINSTATUSVIEW_H

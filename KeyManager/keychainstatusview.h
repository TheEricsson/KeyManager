#ifndef KEYCHAINSTATUSVIEW_H
#define KEYCHAINSTATUSVIEW_H

#include <QObject>
#include <QWidget>
#include "winsubmenu.h"

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
    void setKeychainImagePath (const QString& imgPath);

private:
    virtual void showEvent(QShowEvent *);
    QSqlRelationalTableModel *mKeyModel;
    QSqlRelationalTableModel *mKeychainModel;
    QSortFilterProxyModel *mFilteredKeychainModel;
    QSortFilterProxyModel *mFilteredKeyModel;

    QSqlQueryModel *mKeyOverview;
    QTableView *mKeys;
    QTableView *mKeychain;
    QLabel *mIdBarcode;
    QLabel *mStreet;
    QLabel *mStreetNr;
    QLabel *mAreaCode;
    QLabel *mCity;
    QLabel *mQuantityKeys;
    QImage *mKeychainImage;
    QPushButton *mKeysImgPreview;
};

#endif // KEYCHAINSTATUSVIEW_H

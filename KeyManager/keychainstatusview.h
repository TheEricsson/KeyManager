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

class KeychainStatusView : public WinSubmenu
{
    Q_OBJECT
public:
    explicit KeychainStatusView(QWidget *parent = nullptr);
    ~KeychainStatusView ();
    void setKeychainStatus (const int &statusId);
    bool setKeychainModel (QSqlRelationalTableModel* model);
    bool setKeysModel (QSqlRelationalTableModel* model);
    void setKeychainImagePath (const QString& imgPath);

private:
    virtual void showEvent(QShowEvent *);
    int mKeychainStatusId;
    QSqlRelationalTableModel *mKeysOverviewModel;
    QSqlRelationalTableModel *mKeychainModel;

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

#ifndef ADDKEYCHAINVIEW_H
#define ADDKEYCHAINVIEW_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QDate>
#include "globals.h"
#include "winsubmenu.h"

class QTableView;
class QSqlRelationalTableModel;
class QSortFilterProxyModel;
class QItemSelection;
class QPushButton;
class QLineEdit;
class QLabel;

class ViewDataKeychain;

class AddKeychainView : public WinSubmenu
{
    Q_OBJECT
public:
    explicit AddKeychainView(QWidget *parent = nullptr);
    ~AddKeychainView();
    void reset();

public slots:
    void setTableFilter(const QString &);

private slots:
    virtual void onMenuBtnClicked (Gui::MenuButton btnType);
    void onTableSelectionChanged (const QItemSelection &itemNew, const QItemSelection &itemOld);
    void onButtonProposeLocationClicked ();

private:
    bool setModel (QSqlRelationalTableModel* model);
    virtual void showEvent(QShowEvent *);
    void setTableFilter(const int column, const QString &searchString);
    bool ConfirmationDialog ();

    QSqlRelationalTableModel *mBuildingsModel;
    QTableView *mBuildings;
    QSortFilterProxyModel *mFilteredModel;
    bool mRowSelected;
    QLabel *mSearchLabel;
    QLineEdit *mSearchField;
    QLineEdit *mInternalLocation;

    //ViewDataKeychain *mViewDataKeychain;
};

#endif // ADDKEYCHAINVIEW_H

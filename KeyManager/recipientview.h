#ifndef RECIPIENTVIEW_H
#define RECIPIENTVIEW_H

#include <QObject>
#include <QWidget>
#include <QString>
#include "globals.h"
#include "winsubmenu.h"

class QTableView;
class QSqlRelationalTableModel;
class QSortFilterProxyModel;
class QItemSelection;
class QPushButton;
class QLineEdit;
class QLabel;

class RecipientView : public WinSubmenu
{
    Q_OBJECT
public:
    explicit RecipientView(QWidget *parent = nullptr);
    //void setDataObject (DataObject *data) override;
    bool setModel (QSqlRelationalTableModel* model);
    bool getRecipientData (RecipientData &data);
    void reset();

public slots:
    void setTableFilter(const QString &);

private slots:
    void onTableSelectionChanged (const QItemSelection &itemNew, const QItemSelection &itemOld);
    void onThirdBtnClicked ();

private:
    void setTableFilter(const int column, const QString &searchString);
    void hideSearchField(bool hide);
    void hideNameField(bool hide);
    QTableView *mRecipients;
    QSortFilterProxyModel *mFilteredModel;
    QLabel *mRecipientNameLabel;
    QLineEdit *mRecipientNameEdit;
    bool mRowSelected;
    QLabel *mSearchLabel;
    QLineEdit *mSearchField;
};

#endif // RECIPIENTVIEW_H

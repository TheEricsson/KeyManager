#ifndef RECIPIENTVIEW_H
#define RECIPIENTVIEW_H

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
class QCalendarWidget;
class ReturnDateView;
class AddRecipientView;

class RecipientView : public WinSubmenu
{
    Q_OBJECT
public:
    explicit RecipientView(QWidget *parent = nullptr);
    ~RecipientView();
    void reset();

public slots:
    void setTableFilter(const QString &);

private slots:
    virtual void onMenuBtnClicked (Gui::MenuButton btnType);
    void onTableSelectionChanged (const QItemSelection &itemNew, const QItemSelection &itemOld);
    void onSelectedDateChanged (QDate date);
    void onKeychainStatusChanged (KeychainStatus::Value newStatus);
    void onRecipientNameTextChanged (const QString &text);
    void onAddRecipientButtonClicked(Gui::MenuButton btn);

private:
    bool setModel (QSqlRelationalTableModel* model);
    virtual void showEvent(QShowEvent *);
    void setTableFilter(const int column, const QString &searchString);
    void hideSearchField(bool hide);
    void hideNameField(bool hide);
    void setMenuButtonState (bool tableitemSelected);

    QSqlRelationalTableModel *mRecipientsModel;
    QTableView *mRecipients;
    QSortFilterProxyModel *mFilteredModel;
    QLabel *mRecipientNameLabel;
    QLineEdit *mRecipientNameEdit;
    bool mRowSelected;
    QLabel *mSearchLabel;
    QLineEdit *mSearchField;

    QLabel *mReturnDateLabel;
    KeychainStatus::Value mDurationHandout;
    QVBoxLayout *mDateLayout;

    AddRecipientView *mAddRecipientView;
    ReturnDateView *mReturnDateWidget;

    int mCurrentRecipientId;
};

#endif // RECIPIENTVIEW_H

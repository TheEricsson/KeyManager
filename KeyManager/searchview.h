#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include "winsubmenu.h"

class QTreeView;
class QStandardItem;
class QStandardItemModel;
class QSortFilterProxyModel;
class QLineEdit;
class KeychainStatusView;
class QItemSelection;
class QTableWidget;
class QTableWidgetItem;

class SearchView : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit SearchView(QWidget *parent = nullptr);
        ~SearchView();
        virtual void showEvent(QShowEvent *);
    private slots:
        virtual void onMenuBtnClicked (Gui::MenuButton btnType);
        void onKeychainStatusMenuButtonClicked (Gui::MenuButton btnType);
        void onTableItemClicked(QTableWidgetItem *item);
        void onTreeViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
        void onPropertyFilterSet(QString txt);
        void onKeycodeFilterSet(QString txt);
        void onKeyPositionFilterSet(QString txt);
        void onKeyStatusFilterSet(QString txt);
        void onKeyInfoFilterSet(QString txt);
        void onResetFiltersCLicked();
    private:
        void setData();
        void resetFilters();
        //void setTreeAddressData();
        void setTreeData ();
        void resizeTreeColumnsToContent();
        void showKeychainStatusView();
        QTreeView *mDataView;
        //QTableWidget *mDataTable;
        QStandardItemModel *mStandardModel;

        unsigned int mSelectedKeycode;

        QLineEdit *mPropertyLineEdit;
        QLineEdit *mQrCodeLineEdit;
        QLineEdit *mKeyPositionLineEdit;
        QLineEdit *mKeyStatusLineEdit;
        QLineEdit *mKeyInfoLineEdit;

        QSortFilterProxyModel *mFilterProperty;
        QSortFilterProxyModel *mFilterQrCode;
        QSortFilterProxyModel *mFilterKeyPosition;
        QSortFilterProxyModel *mFilterStatus;
        QSortFilterProxyModel *mFilterInfo;

        KeychainStatusView *mKeychainStatusView;
};

#endif // SEARCHVIEW_H

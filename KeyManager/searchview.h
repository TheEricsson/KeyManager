#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include "winsubmenu.h"

class QTreeView;
class QStandardItem;
class QStandardItemModel;
class QSortFilterProxyModel;
class QLineEdit;

class SearchView : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit SearchView(QWidget *parent = nullptr);
        ~SearchView();
        virtual void showEvent(QShowEvent *);
    private slots:
        void onPropertyFilterSet(QString txt);
        void onKeycodeFilterSet(QString txt);
        void onKeyPositionFilterSet(QString txt);
        void onKeyStatusFilterSet(QString txt);
        void onKeyInfoFilterSet(QString txt);
        void onResetFiltersCLicked();
    private:
        void resetFilters();
        void setTreeAddressData();
        void setTreeData ();
        void resizeTreeColumnsToContent();
        QTreeView *mDataView;
        QStandardItemModel *mStandardModel;

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
};

#endif // SEARCHVIEW_H

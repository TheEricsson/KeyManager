#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include "winsubmenu.h"

class QTreeView;
class QStandardItem;
class QStandardItemModel;

class SearchView : public WinSubmenu
{
    Q_OBJECT
public:
    explicit SearchView(QWidget *parent = nullptr);
    ~SearchView();
    virtual void showEvent(QShowEvent *);
private:
    void setTreeData ();
    QList<QStandardItem *> prepareRow(const QString &first,
               const QString &second,
               const QString &third);
    QTreeView *mDataView;
    QStandardItemModel *mStandardModel;;
};

#endif // SEARCHVIEW_H

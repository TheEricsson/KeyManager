#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include "winsubmenu.h"

class TreeViewImpl;
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
    QList<QStandardItem *> prepareRow(const QString &first,
               const QString &second,
               const QString &third);
    TreeViewImpl *mDataView;
    QStandardItemModel *mStandardModel;;
};

#endif // SEARCHVIEW_H

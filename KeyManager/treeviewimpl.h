#ifndef TREEVIEWIMPL_H
#define TREEVIEWIMPL_H

#include <QObject>
#include <QTreeView>

class TreeViewImpl : public QTreeView
{
    public:
        explicit TreeViewImpl (QWidget *parent = nullptr);
    /*protected:
        void mousePressEvent(QMouseEvent *event) override;*/
};

#endif // TREEVIEWIMPL_H

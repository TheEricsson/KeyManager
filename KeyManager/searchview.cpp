#include "searchview.h"
#include <QSpacerItem>
#include <QLayout>
#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>

SearchView::SearchView (QWidget *parent)
    : WinSubmenu {parent}
{
    mDataView = new QTreeView (this);
    mDataView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mDataView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    mStandardModel = new QStandardItemModel (this);
    mDataView->setModel(mStandardModel);

    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->addWidget(mDataView);
    setCentralLayout(centralLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    setMenuButtons(menuButtons);
}

void SearchView::showEvent(QShowEvent *)
{
    QList<QStandardItem *> preparedRow =prepareRow("first", "second", "third");
    QStandardItem *item = mStandardModel->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item->appendRow(preparedRow);

    QList<QStandardItem *> secondRow =prepareRow("111", "222", "333");
    // adding a row to an item starts a subtree
    preparedRow.first()->appendRow(secondRow);
}

//test...
QList<QStandardItem *> SearchView::prepareRow(const QString &first,
                                              const QString &second,
                                              const QString &third)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(first);
    rowItems << new QStandardItem(second);
    rowItems << new QStandardItem(third);
    return rowItems;
}

SearchView::~SearchView()
{
    if (0 != mStandardModel)
        delete mStandardModel;
}

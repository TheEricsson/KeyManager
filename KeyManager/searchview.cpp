#include "searchview.h"
#include <QSpacerItem>
#include <QLayout>
#include "treeviewimpl.h"
#include <QStandardItem>
#include <QStandardItemModel>

SearchView::SearchView (QWidget *parent)
    : WinSubmenu {parent}
{
    mDataView = new TreeViewImpl (this);
    mDataView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mDataView->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    mStandardModel = new QStandardItemModel;
    mDataView->setModel(mStandardModel);
    mDataView->expandAll();

    layout()->addWidget(mDataView);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout()->addItem(spacer);

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
    delete mStandardModel;
}

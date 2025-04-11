#include "editcustomerview.h"
#include "qlabel.h"
#include "winsubmenu.h"
#include "iointerface.h"

#include <QTableView>
#include <QSqlRelationalTableModel>

EditCustomerView::EditCustomerView(QWidget *parent) : WinSubmenu {parent}
{
    mCustomerView = 0;
    mCustomerModel = 0;

    setHeader("Kundenverwaltung");

    mCustomerView = new QTableView (this);
    mCustomerModel = new QSqlRelationalTableModel (this);

    setCustomersModel(mCustomerModel);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::AddCustomer);
    menuButtons.append(Gui::Edit);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    connect (mCustomerView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onTableSelectionChanged(const QItemSelection &, const QItemSelection &)));
}

void EditCustomerView::showEvent(QShowEvent *)
{
    reset ();
}

void EditCustomerView::reset()
{
    if (mCustomerModel)
    {
        ioInterface()->initCustomerModel(mCustomerModel);
        setCustomersModel (mCustomerModel);
    }
    update ();
}

bool EditCustomerView::setCustomersModel(QSqlRelationalTableModel* model)
{
    if (model)
    {
        //model->setEditStrategy(QSqlTableModel::OnFieldChange);

        // model->setHeaderData(2, Qt::Horizontal, tr("Kategorie"), Qt::DisplayRole);
        // model->setHeaderData(3, Qt::Horizontal, tr("Zustand"), Qt::DisplayRole);
        // model->setHeaderData(4, Qt::Horizontal, tr("Zusatzinformation"), Qt::DisplayRole);

        if (mCustomerView)
        {
            qDebug () << "KeychainStatusView::setModel QSqlRelationalTableModel";
            mCustomerView->setModel(model);
            mCustomerView->show();

            mCustomerView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
            mCustomerView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

            mCustomerView->resizeColumnsToContents();

            return true;
        }
        return false;
    }
    return false;
}

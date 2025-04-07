#ifndef EDITKEYVIEW_H
#define EDITKEYVIEW_H

#include <QWidget>
#include <QItemSelectionModel>
#include "winsubmenu.h"
#include "addkeyview.h"

class QSqlRelationalTableModel;
class QTableView;

class EditKeyView : public WinSubmenu
{
    Q_OBJECT
    public:
        EditKeyView(QWidget *parent = nullptr);
        ~EditKeyView();
    private slots:
        virtual void onMenuBtnClicked (Gui::MenuButton btnType);
        void onTableSelectionChanged (const QItemSelection &itemNew, const QItemSelection &itemOld);
        void onAddKeyMenuButtonClicked (Gui::MenuButton btn);
    private:
        void showEvent(QShowEvent *);
        bool setKeysModel (QSqlRelationalTableModel* model);
        void reset();
        QLabel* mKeychainCode;
        QTableView *mKeys;
        QSqlRelationalTableModel *mKeyModel;
        AddKeyView *mAddKeyView;
};

#endif // EDITKEYVIEW_H

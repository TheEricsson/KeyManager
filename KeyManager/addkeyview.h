#ifndef ADDKEYVIEW_H
#define ADDKEYVIEW_H

#include <QWidget>
#include "winsubmenu.h"

class CheckBoxArray;
class QTextEdit;

class AddKeyView : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit AddKeyView(QWidget *parent = nullptr);
        ~AddKeyView();
    private slots:
        virtual void onMenuBtnClicked (Gui::MenuButton btnType);
    private:
        void reset ();
        bool checkSelections ();
        void showEvent(QShowEvent *);

        CheckBoxArray *mKeyCategories;
        CheckBoxArray *mKeyStates;
        QTextEdit *mKeyDescription;
};

#endif // ADDKEYVIEW_H

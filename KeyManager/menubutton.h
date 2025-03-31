#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QToolButton>
#include <QWidget>

#include "globals.h"

class MenuButton : public QToolButton
{
    Q_OBJECT

    public:
        MenuButton (QWidget *parent = nullptr);
        void setButtonType (Gui::MenuButton btnType) {mButtonType = btnType;};
        Gui::MenuButton getButtonType () {return mButtonType;};
    signals:
        void clicked (Gui::MenuButton btnType);
    private:
        Gui::MenuButton mButtonType;
    private slots:
        void menuButtonClicked();
};

#endif // MENUBUTTON_H

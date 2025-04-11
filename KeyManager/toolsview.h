#ifndef TOOLSVIEW_H
#define TOOLSVIEW_H

#include <QWidget>
#include "winsubmenu.h"

class CodeGeneratorView;

class ToolsView : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit ToolsView(QWidget *parent = nullptr);

    signals:
    private slots:
        void onEncoderSettingsClicked();
        void onBtnClicked_CodeGenerator (Gui::MenuButton btn);
    private:
        CodeGeneratorView *mCodeGeneratorView;
};

#endif // TOOLSVIEW_H

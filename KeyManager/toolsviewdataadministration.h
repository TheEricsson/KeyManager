#ifndef TOOLSVIEWDATAADMINISTRATION_H
#define TOOLSVIEWDATAADMINISTRATION_H

#include "winsubmenu.h"

class CodeGeneratorView;

class ToolsViewDataAdministration : public WinSubmenu
{
    Q_OBJECT

    public:
        explicit ToolsViewDataAdministration(QWidget *parent = nullptr);
        ~ToolsViewDataAdministration();
    signals:
    private slots:
        void onEncoderSettingsClicked();
        void onBtnClicked_CodeGenerator (Gui::MenuButton btn);
    private:
        CodeGeneratorView *mCodeGeneratorView;
};

#endif // TOOLSVIEWDATAADMINISTRATION_H

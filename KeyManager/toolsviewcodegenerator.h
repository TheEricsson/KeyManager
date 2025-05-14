#ifndef TOOLSVIEWCODEGENERATOR_H
#define TOOLSVIEWCODEGENERATOR_H

#include <QWidget>
#include "winsubmenu.h"

class CodeGeneratorView;

class ToolsViewCodeGenerator : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit ToolsViewCodeGenerator(QWidget *parent = nullptr);
        ~ToolsViewCodeGenerator();

    signals:
    private slots:
        void onEncoderSettingsClicked();
        void onBtnClicked_CodeGenerator (Gui::MenuButton btn);
    private:
        CodeGeneratorView *mCodeGeneratorView;
};

#endif // TOOLSVIEWCODEGENERATOR_H

#ifndef TOOLSVIEWDATAADMINISTRATION_H
#define TOOLSVIEWDATAADMINISTRATION_H

#include "winsubmenu.h"

class CodeGeneratorView;
class RecipientView;
class TableView;

class ToolsViewDataAdministration : public WinSubmenu
{
    Q_OBJECT

    public:
        explicit ToolsViewDataAdministration(QWidget *parent = nullptr);
        ~ToolsViewDataAdministration();
    signals:
    private slots:
        void onEncoderSettingsClicked();
        void onRecipientDataClicked();
        void onBtnClicked_CodeGenerator (Gui::MenuButton btn);
        void onBtnClicked_RecipientView (Gui::MenuButton btn);
    private:
        CodeGeneratorView *mCodeGeneratorView;
        RecipientView *mRecipientView;
};

#endif // TOOLSVIEWDATAADMINISTRATION_H

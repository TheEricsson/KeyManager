#include "toolsviewcodegenerator.h"
#include "winsubmenu.h"
#include "codegeneratorview.h"
#include "globals.h"
#include <QGridLayout>
#include <QPushButton>

ToolsViewCodeGenerator::ToolsViewCodeGenerator(QWidget *parent)
    : WinSubmenu{parent}
{
    mCodeGeneratorView = 0;

    setHeader("Code-Generator");

    QGridLayout *gridLayout = new QGridLayout();
    QPushButton *encoderBtn = new QPushButton("Neue Codes\ngenerieren", this);
    encoderBtn->setMinimumSize(QSize(Gui::buttonHeight,Gui::buttonWidth));

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    gridLayout->addWidget(encoderBtn, 0, 0);
    gridLayout->addItem(spacer, 1, 0);

    setCentralLayout(gridLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    connect (encoderBtn, SIGNAL (clicked()), this, SLOT(onEncoderSettingsClicked()));
}

void ToolsViewCodeGenerator::onEncoderSettingsClicked()
{
    if (!mCodeGeneratorView)
    {
        mCodeGeneratorView = new CodeGeneratorView ();
        mCodeGeneratorView->setIOInterface(ioInterface());
        connect (mCodeGeneratorView, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT(onBtnClicked_CodeGenerator(Gui::MenuButton)));
    }
    mCodeGeneratorView->show();
}

void ToolsViewCodeGenerator::onBtnClicked_CodeGenerator (Gui::MenuButton btn)
{
    switch (btn)
    {
        case Gui::Back:
        case Gui::Ok:
            if (mCodeGeneratorView)
            {
                mCodeGeneratorView->hide();
                delete mCodeGeneratorView;
                mCodeGeneratorView = 0;
                setFocus();
                update ();
            }
            break;
        default:
            break;
    }
}

ToolsViewCodeGenerator::~ToolsViewCodeGenerator()
{
    if (mCodeGeneratorView)
        delete mCodeGeneratorView;
}

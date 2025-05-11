#include "toolsview.h"
#include "winsubmenu.h"
#include "codegeneratorview.h"
#include "globals.h"
#include <QGridLayout>
#include <QPushButton>

ToolsView::ToolsView(QWidget *parent)
    : WinSubmenu{parent}
{
    mCodeGeneratorView = 0;

    //setHeader("Tools");

    QGridLayout *gridLayout = new QGridLayout();
    QPushButton *encoderBtn = new QPushButton("Codegenerator", this);
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

void ToolsView::onEncoderSettingsClicked()
{
    if (!mCodeGeneratorView)
    {
        mCodeGeneratorView = new CodeGeneratorView ();
        mCodeGeneratorView->setIOInterface(ioInterface());
        connect (mCodeGeneratorView, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT(onBtnClicked_CodeGenerator(Gui::MenuButton)));
    }
    mCodeGeneratorView->show();
}

void ToolsView::onBtnClicked_CodeGenerator (Gui::MenuButton btn)
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

#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include "toolsviewdataadministration.h"
#include "winsubmenu.h"
#include "codegeneratorview.h"
#include "recipientview.h"
#include "tableview.h"

ToolsViewDataAdministration::ToolsViewDataAdministration(QWidget *parent)
    : WinSubmenu{parent}
{
    mCodeGeneratorView = 0;
    mRecipientView = 0;
    mRecipientDataView = 0;

    setHeader("Tools");

    //code administration
    QGroupBox *codeAdministration  = new QGroupBox(tr("Code-Administration"));
    QPushButton *encoderBtn = new QPushButton("Neue Codes\ngenerieren", this);

    QGridLayout *codeAdminLayout = new QGridLayout();
    codeAdminLayout->addWidget(encoderBtn, 0, 0);

    codeAdministration->setLayout(codeAdminLayout);

    //database administration
    QGroupBox *databaseAdministration = new QGroupBox(tr("Datenbankpflege"));
    QPushButton *btnRecipientData = new QPushButton (tr("Empfängeradressen\nbearbeiten"));
    QPushButton *btnRecipientCategories = new QPushButton (tr("Empfängerkategorien\nbearbeiten"));
    QPushButton *btnCustomerData = new QPushButton (tr("Gebäudeadressen\nbearbeiten"));
    QPushButton *btnKeyCategories = new QPushButton (tr("Schlüsselkategorien\nbearbeiten"));
    QPushButton *btnKeychainStatus= new QPushButton (tr("Verfügbarkeitsarten\nbearbeiten"));

    QGridLayout *databaseAdminLayout = new QGridLayout();
    databaseAdminLayout->addWidget(btnRecipientData, 0, 0);
    databaseAdminLayout->addWidget(btnRecipientCategories, 0, 1);
    databaseAdminLayout->addWidget(btnCustomerData, 1, 0);
    databaseAdminLayout->addWidget(btnKeyCategories, 1, 1);
    databaseAdminLayout->addWidget(btnKeychainStatus, 2, 0);

    databaseAdministration->setLayout(databaseAdminLayout);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(codeAdministration);
    mainLayout->addWidget(databaseAdministration);
    mainLayout->addItem(spacer);

    setCentralLayout(mainLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    setMenuButtons(menuButtons);

    //signals
    connect (encoderBtn, SIGNAL (clicked()), this, SLOT(onEncoderSettingsClicked()));
    connect (btnRecipientData, SIGNAL (clicked()), this, SLOT(onRecipientDataClicked()));
}

void ToolsViewDataAdministration::onEncoderSettingsClicked()
{
    if (!mCodeGeneratorView)
    {
        mCodeGeneratorView = new CodeGeneratorView ();
        mCodeGeneratorView->setIOInterface(ioInterface());
        connect (mCodeGeneratorView, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT(onBtnClicked_CodeGenerator(Gui::MenuButton)));
    }
    mCodeGeneratorView->show();
}

void ToolsViewDataAdministration::onRecipientDataClicked()
{
    if (!mRecipientDataView)
    {
        mRecipientDataView = new TableView("recipientAddresses");
        mRecipientDataView->setIOInterface(ioInterface());
        mRecipientDataView->show();
    }
    /*if (!mRecipientView)
    {
        mRecipientView = new RecipientView ();
        mRecipientView->setIOInterface(ioInterface());
        connect (mCodeGeneratorView, SIGNAL(menuButtonClicked(Gui::MenuButton)), this, SLOT(onBtnClicked_CodeGenerator(Gui::MenuButton)));
    }
    mRecipientView->show();*/
}

void ToolsViewDataAdministration::onBtnClicked_CodeGenerator (Gui::MenuButton btn)
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

ToolsViewDataAdministration::~ToolsViewDataAdministration()
{
    if (mCodeGeneratorView)
        mCodeGeneratorView->deleteLater();

    if (mRecipientView)
        mRecipientView->deleteLater();
}

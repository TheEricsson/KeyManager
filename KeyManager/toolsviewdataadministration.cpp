#include "toolsviewdataadministration.h"
#include "winsubmenu.h"
#include <QVBoxLayout>
#include <QPushButton>

ToolsViewDataAdministration::ToolsViewDataAdministration(QWidget *parent)
    : WinSubmenu{parent}
{
    setHeader("Datenpflege");

    //widgets
    QPushButton *btnRecipientData = new QPushButton ("Empfängeradressen\nbearbeiten");
    QPushButton *btnRecipientCategories = new QPushButton ("Empfängerkategorien\nbearbeiten");
    QPushButton *btnCustomerData = new QPushButton ("Gebäudeadressen\nbearbeiten");
    QPushButton *btnKeyCategories = new QPushButton ("Schlüsselkategorien\nbearbeiten");
    QPushButton *btnKeychainStatus= new QPushButton ("Verfügbarkeitsarten\nbearbeiten");
    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(btnRecipientData);
    layout->addWidget(btnRecipientCategories);
    layout->addWidget(btnCustomerData);
    layout->addWidget(btnKeyCategories);
    layout->addWidget(btnKeychainStatus);
    layout->addItem(spacer);

    setCentralLayout(layout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);
}

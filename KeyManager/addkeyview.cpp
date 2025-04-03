#include "addkeyview.h"
#include <QComboBox>
#include <QTextEdit>
#include <QLayout>
#include <QSqlRelationalTableModel>
#include "datainterface.h"

AddKeyView::AddKeyView(QWidget *parent) : WinSubmenu {parent}
{
    setHeader("Schlüssel hinzufügen");

    QComboBox *boxKeyCategory = new QComboBox (this);
    QComboBox *boxKeyStatus = new QComboBox (this);
    QTextEdit *keyDescription = new QTextEdit (this);

    mViewModel = new QSqlRelationalTableModel(this);
    mViewModel->setTable("keys");
    mViewModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    boxKeyCategory->setModel(mViewModel);

    //dataInterface()->getKeychainStatusCount ();

    boxKeyStatus->addItem("test1");
    boxKeyStatus->addItem("test2");
    boxKeyStatus->addItem("test3");

    layout()->addWidget(boxKeyCategory);
    layout()->addWidget(boxKeyStatus);
    layout()->addWidget(keyDescription);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Ok);
    setMenuButtons(menuButtons);

    update ();

    connect (boxKeyStatus, SIGNAL(highlighted(int)), this, SLOT(updateView()));
    connect (boxKeyStatus, SIGNAL(currentIndexChanged(int)), this, SLOT(updateView()));

}

void AddKeyView::updateView ()
{
    qDebug () << "AddKeyView::updateView";
    update ();
}

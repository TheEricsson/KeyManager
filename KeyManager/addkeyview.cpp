#include "addkeyview.h"
#include <QComboBox>
#include <QTextEdit>
#include <QLayout>
#include <QLabel>
#include "datainterface.h"
#include "checkboxarray.h"
#include "iointerface.h"
#include <QMessageBox>

AddKeyView::AddKeyView(QWidget *parent) : WinSubmenu {parent}
{
    mKeyCategories = 0;
    mKeyStates = 0;

    setHeader("Schlüsseleigenschaften setzen");

    mKeyDescription = new QTextEdit (this);
    mKeyCategories = new CheckBoxArray (this);
    mKeyStates = new CheckBoxArray (this);

    QLabel *headerKeyCat = new QLabel ("Schlüsselkategorie");
    headerKeyCat->setAlignment(Qt::AlignHCenter);
    headerKeyCat->setContentsMargins(0,0,0,0);
    headerKeyCat->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    QLabel *headerKeyStatus = new QLabel ("Schlüsselstatus");
    headerKeyStatus->setAlignment(Qt::AlignHCenter);
    headerKeyStatus->setContentsMargins(0,0,0,0);
    headerKeyStatus->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    QLabel *headerDescription = new QLabel ("Zusätzliche Angaben");
    headerKeyStatus->setAlignment(Qt::AlignHCenter);
    headerKeyStatus->setContentsMargins(0,0,0,0);
    headerKeyStatus->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    layout()->addWidget(headerKeyCat);
    layout()->addWidget(mKeyCategories);
    layout()->addWidget(headerKeyStatus);
    layout()->addWidget(mKeyStates);
    layout()->addWidget(headerDescription);
    layout()->addWidget(mKeyDescription);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Ok);
    setMenuButtons(menuButtons);

    update ();
}

void AddKeyView::onMenuBtnClicked (Gui::MenuButton btnType)
{
    switch (btnType)
    {
        // check valid selection
        case (Gui::Ok):
            if (checkSelections ())
            {
                int keyCode = _UNDEFINED;
                if (dataInterface())
                    keyCode = dataInterface()->getScannedCode();

                //check if keycode is valid
                if (_UNDEFINED != keyCode)
                {
                    IOInterface::keyData *data = new IOInterface::keyData();
                    data->keychainId = keyCode;
                    data->categoryId = mKeyCategories->getCheckedButtonIndex();
                    data->statusId = mKeyStates->getCheckedButtonIndex();
                    data->description = mKeyDescription->toPlainText();
                    ioInterface()->addKey (data);
                    delete data;
                }

                emit menuButtonClicked(btnType);
            }
            break;
        // fall through for any other button
        default:
            emit menuButtonClicked(btnType);
            break;
    }
}

void AddKeyView::showEvent(QShowEvent *)
{
    qDebug () << "AddKeyView::showEvent(QShowEvent *)";
    reset ();
}

void AddKeyView::reset ()
{
    mKeyCategories->setData (ioInterface(), dataInterface());
    mKeyCategories->init ("keyCategories", "category");

    mKeyStates->setData (ioInterface(), dataInterface());
    mKeyStates->init ("keyStates", "status");
}

bool AddKeyView::checkSelections ()
{
    if (_UNDEFINED == mKeyCategories->getCheckedButtonIndex() ||
        _UNDEFINED == mKeyStates->getCheckedButtonIndex())
    {
        QMessageBox::information(0, "Unvollständige Eingaben",
                                 "Bitte vollständige Auswahl treffen.", QMessageBox::Ok);
        return false;
    }
    return true;
}

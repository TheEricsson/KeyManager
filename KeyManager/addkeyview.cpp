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

    mKeyDescription = new QTextEdit ();
    mKeyCategories = new CheckBoxArray ();
    connect (mKeyCategories, SIGNAL(radioBtnToggled()), this, SLOT(onRadioBtnToggled()));
    //mKeyStates = new CheckBoxArray (this);
    //connect (mKeyStates, SIGNAL(radioBtnToggled()), this, SLOT(onRadioBtnToggled()));

    QLabel *headerKeyCat = new QLabel ("Schlüsselkategorie", this);
    headerKeyCat->setAlignment(Qt::AlignHCenter);
    //headerKeyCat->setContentsMargins(0,0,0,0);
    //headerKeyCat->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    //QLabel *headerKeyStatus = new QLabel ("Schlüsselstatus", this);
    //headerKeyStatus->setAlignment(Qt::AlignHCenter);
    //headerKeyStatus->setContentsMargins(0,0,0,0);
    //headerKeyStatus->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    QLabel *headerDescription = new QLabel ("Zusätzliche Angaben", this);
    //headerKeyStatus->setAlignment(Qt::AlignHCenter);
    //headerKeyStatus->setContentsMargins(0,0,0,0);
    //headerKeyStatus->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    layout()->addWidget(headerKeyCat);
    layout()->addWidget(mKeyCategories);
    //layout()->addWidget(headerKeyStatus);
    //layout()->addWidget(mKeyStates);
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
                    // add new key in database
                    IOInterface::keyData *data = new IOInterface::keyData();
                    data->keychainId = keyCode;
                    data->categoryId = mKeyCategories->getCheckedButtonIndex();
                    data->statusId = (int)KeyStatus::Ok;//mKeyStates->getCheckedButtonIndex();
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

void AddKeyView::onRadioBtnToggled()
{
    qDebug() << "AddKeyView::onRadioBtnToggled()";
    update();
}

void AddKeyView::showEvent(QShowEvent *)
{
    reset ();
}

void AddKeyView::reset ()
{
    mKeyCategories->setData (ioInterface(), dataInterface());
    mKeyCategories->init ("keyCategories", "category");

    //mKeyStates->setData (ioInterface(), dataInterface());
    //mKeyStates->init ("keyStates", "status");

    //update();
}

bool AddKeyView::checkSelections ()
{
    if (_UNDEFINED == mKeyCategories->getCheckedButtonIndex())
    {
        QMessageBox msgBox;
        msgBox.setStandardButtons(QMessageBox::Abort);
        msgBox.setText ("Fehler!");
        msgBox.setInformativeText("Eingaben unvollständig. Bitte prüfen.");
        msgBox.exec();
        return false;
    }
    return true;
}

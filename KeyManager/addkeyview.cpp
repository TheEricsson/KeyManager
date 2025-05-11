#include "addkeyview.h"
#include <QComboBox>
#include <QTextEdit>
#include <QLayout>
#include <QLabel>
#include "datainterface.h"
#include "checkboxarray.h"
#include "iointerface.h"
#include <QMessageBox>
#include <QRadioButton>

AddKeyView::AddKeyView(QWidget *parent) : WinSubmenu {parent}
{
    mKeyDescription = 0;
    mKeyCategories = 0;
    mKeyStates = 0;

    setHeader("Schlüssel hinzufügen");

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Ok);
    setMenuButtons(menuButtons);
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
    //update();
}

void AddKeyView::showEvent(QShowEvent *)
{
    reset ();
}

void AddKeyView::reset ()
{
    if (!mKeyDescription)
        mKeyDescription = new QTextEdit ();
    else
        mKeyDescription->clear();

    if (!mKeyCategories)
        mKeyCategories = new CheckBoxArray (this);

    QLabel *keyTypeHeader = new QLabel("Schlüsselkategorie");
    QLabel *keyAdditionalInfoHeader = new QLabel ("Zusätzliche Angaben");
    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    mKeyCategories->setData (ioInterface(), dataInterface());
    mKeyCategories->init ("keyCategories", "category");

    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->addWidget(keyTypeHeader);

    for (unsigned int i = 0; i < mKeyCategories->count(); i++)
    {
        QRadioButton *btn= mKeyCategories->getButtonAt(i);
        if (0 != btn)
        {
            centralLayout->addWidget(btn);
        }
    }
    centralLayout->addWidget(keyAdditionalInfoHeader);
    centralLayout->addWidget(mKeyDescription);
    centralLayout->addItem(spacer);

    setCentralLayout(centralLayout);

    //connect (mKeyCategories, SIGNAL(radioBtnToggled()), this, SLOT(onRadioBtnToggled()));
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

AddKeyView::~AddKeyView()
{
    //if (mKeyCategories)
    //    delete mKeyCategories;
}

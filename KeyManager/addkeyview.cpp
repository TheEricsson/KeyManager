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
    mKeyDescriptionText = "";

    setHeader("Schlüssel hinzufügen");

    QTextEdit *keyDecription = new QTextEdit();
    QLabel *keyTypeHeader = new QLabel("Schlüsselkategorie");
    QLabel *keyAdditionalInfoHeader = new QLabel ("Zusätzliche Angaben");
    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    mKeyCategories = new CheckBoxArray ();
    mKeyDescription = new QTextEdit();

    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->addWidget(keyTypeHeader);
    centralLayout->addWidget(mKeyCategories);
    centralLayout->addWidget(keyAdditionalInfoHeader);
    centralLayout->addWidget(mKeyDescription);
    centralLayout->addItem(spacer);

    connect (mKeyDescription, SIGNAL(textChanged()), this, SLOT(onKeyDescriptionChanged()));

    setCentralLayout(centralLayout);

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

                    //db index is starting with '1'
                    int categoryIdDb = mKeyCategories->getCheckedButtonIndex() + 1;
                    data->categoryId = categoryIdDb;
                    data->statusId = (int)KeyStatus::Ok;//mKeyStates->getCheckedButtonIndex();
                    data->description = mKeyDescriptionText;
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

void AddKeyView::onKeyDescriptionChanged ()
{
    mKeyDescriptionText = mKeyDescription->toPlainText();
}

void AddKeyView::showEvent(QShowEvent *)
{
    reset ();
}

void AddKeyView::reset ()
{
    if (mKeyDescription)
        mKeyDescription->clear();

    if (mKeyCategories)
    {
        if (ioInterface())
        {
            mKeyCategories->init (ioInterface(), "keyCategories", "category");
        }
    }
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

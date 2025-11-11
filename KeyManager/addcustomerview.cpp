#include "AddCustomerView.h"
#include <QComboBox>
#include <QTextEdit>
#include <QLayout>
#include <QLabel>
#include "iointerface.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QFormLayout>

AddCustomerView::AddCustomerView(QWidget *parent) : WinSubmenu {parent}
{
    setHeader("Gebäude hinzufügen");

    mStreet = new QLineEdit (this);
    mStreetNr = new QLineEdit (this);
    //mStreetNr->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d+\\w+"), mStreetNr));
    mAreaCode = new QLineEdit (this);
    mAreaCode->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d\\d\\d\\d\\d"), mAreaCode));
    mCity = new QLineEdit (this);
    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QFormLayout *mainLayout = new QFormLayout();
    mainLayout->addRow("Straße", mStreet);
    mainLayout->addRow("Hausnummer", mStreetNr);
    mainLayout->addRow("PLZ", mAreaCode);
    mainLayout->addRow("Stadt", mCity);
    mainLayout->addItem(spacer);

    setCentralLayout(mainLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Ok);
    setMenuButtons(menuButtons);

    update ();
}

void AddCustomerView::onMenuBtnClicked (Gui::MenuButton btnType)
{
    switch (btnType)
    {
    // check valid selection
    case (Gui::Ok):
        //force update on text items (android bug)
        updateForm();
        if (checkSelections ())
        {
            // add new key in database
            IOInterface::customerData *data = new IOInterface::customerData();
            data->street = mStreet->text();
            data->number = mStreetNr->text();
            data->areaCode = mAreaCode->text();
            data->city = mCity->text();
            ioInterface()->addNewCustomer(data);
            delete data;
            emit menuButtonClicked(btnType);
        }
        break;
    // fall through for any other button
    default:
        emit menuButtonClicked(btnType);
        break;
    }
}

void AddCustomerView::showEvent(QShowEvent *)
{
    reset();
}

void AddCustomerView::updateForm()
{
    /*mStreet->update();
    mStreetNr->update();
    mAreaCode->update();
    mCity->update();
    update();*/
}

void AddCustomerView::reset ()
{
    mStreet->setText("");
    mStreetNr->setText("");
    mAreaCode->setText("");
    mCity->setText("");
}

bool AddCustomerView::checkSelections ()
{
    bool checkOk = true;
    QString styleSheet = "border-style: solid;border-width: 1px;border-color: red";

    //check all line edits
    if ("" == mStreet->text())
    {
        mStreet->setStyleSheet(styleSheet);
        checkOk = false;
    }
    else
        mStreet->setStyleSheet("");

    if ("" == mStreetNr->text())
    {
        mStreetNr->setStyleSheet(styleSheet);
        checkOk = false;
    }
    else
        mStreetNr->setStyleSheet("");

    if ("" == mAreaCode->text())
    {
        mAreaCode->setStyleSheet(styleSheet);
        checkOk = false;
    }
    else
        mAreaCode->setStyleSheet("");

    if ("" == mCity->text())
    {
        mCity->setStyleSheet(styleSheet);
        checkOk = false;
    }
    else
        mCity->setStyleSheet("");

    if (!checkOk)
    {
        QMessageBox msgBox;
        msgBox.setStandardButtons(QMessageBox::Abort);
        msgBox.setText ("Fehler!");
        msgBox.setInformativeText("Eingaben unvollständig. Bitte prüfen.");
        msgBox.exec();
    }

    return checkOk;
}

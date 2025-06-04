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
    mStreetNr->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d+\\w+"), mStreetNr));
    mAreaCode = new QLineEdit (this);
    mAreaCode->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d\\d\\d\\d\\d"), mAreaCode));
    mCity = new QLineEdit (this);
    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    connect (mStreet, SIGNAL(textChanged(QString)), this, SLOT(onStreetChanged(QString)));
    connect (mStreetNr, SIGNAL(textChanged(QString)), this, SLOT(onStreetNrChanged(QString)));
    connect (mAreaCode, SIGNAL(textChanged(QString)), this, SLOT(onAreaCodeChanged(QString)));
    connect (mCity, SIGNAL(textChanged(QString)), this, SLOT(onCityChanged(QString)));

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
        if (checkSelections ())
        {
            // add new key in database
            IOInterface::customerData *data = new IOInterface::customerData();
            data->street = mStreetText;
            data->number = mStreetNrText;
            data->areaCode = mAreaCodeText;
            data->city = mCityText;
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

void AddCustomerView::onStreetChanged(QString text)
{
    mStreetText = text;
}

void AddCustomerView::onStreetNrChanged(QString text)
{
    mStreetNrText = text;
}

void AddCustomerView::onAreaCodeChanged(QString text)
{
    mAreaCodeText = text;
}

void AddCustomerView::onCityChanged(QString text)
{
    mCityText = text;
}

void AddCustomerView::showEvent(QShowEvent *)
{
    reset();
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
    if ("" == mStreetText)
    {
        mStreet->setStyleSheet(styleSheet);
        checkOk = false;
    }
    else
        mStreet->setStyleSheet("");

    if ("" == mStreetNrText)
    {
        mStreetNr->setStyleSheet(styleSheet);
        checkOk = false;
    }
    else
        mStreetNr->setStyleSheet("");

    if ("" == mAreaCodeText)
    {
        mAreaCode->setStyleSheet(styleSheet);
        checkOk = false;
    }
    else
        mAreaCode->setStyleSheet("");

    if ("" == mCityText)
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

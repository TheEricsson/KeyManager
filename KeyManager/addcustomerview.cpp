#include "AddCustomerView.h"
#include <QComboBox>
#include <QTextEdit>
#include <QLayout>
#include <QLabel>
#include "iointerface.h"
#include <QMessageBox>
#include <QLineEdit>

AddCustomerView::AddCustomerView(QWidget *parent) : WinSubmenu {parent}
{
    //setHeader("Kunde hinzufügen");

    mStreet = new QLineEdit (this);
    mStreetNr = new QLineEdit (this);
    mStreetNr->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d+\\w+"), mStreetNr));
    mAreaCode = new QLineEdit (this);
    mAreaCode->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d\\d\\d\\d\\d"), mAreaCode));
    mCity = new QLineEdit (this);

    connect (mStreet, SIGNAL(textChanged(QString)), this, SLOT(onValueChanged(QString)));
    connect (mStreetNr, SIGNAL(textChanged(QString)), this, SLOT(onValueChanged(QString)));
    connect (mAreaCode, SIGNAL(textChanged(QString)), this, SLOT(onValueChanged(QString)));
    connect (mCity, SIGNAL(textChanged(QString)), this, SLOT(onValueChanged(QString)));

    QLabel *headerStreet = new QLabel ("Straße:", this);
    QLabel *headerStreetNr = new QLabel ("Hausnummer", this);
    QLabel *headerAreaCode = new QLabel ("Postleitzahl", this);
    QLabel *headerCity = new QLabel ("Stadt", this);

    QGridLayout *gridLayout = new QGridLayout ();
    gridLayout->addWidget(headerStreet, 0, 0, Qt::AlignLeft);
    gridLayout->addWidget(mStreet, 0, 1, Qt::AlignLeft);

    gridLayout->addWidget(headerStreetNr, 1, 0, Qt::AlignLeft);
    gridLayout->addWidget(mStreetNr, 1, 1, Qt::AlignLeft);

    gridLayout->addWidget(headerAreaCode, 2, 0, Qt::AlignLeft);
    gridLayout->addWidget(mAreaCode, 2, 1, Qt::AlignLeft);

    gridLayout->addWidget(headerCity, 3, 0, Qt::AlignLeft);
    gridLayout->addWidget(mCity, 3, 1, Qt::AlignLeft);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    gridLayout->addItem(spacer, 4, 1, 1, 1);

    setCentralLayout(gridLayout);

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

void AddCustomerView::onValueChanged (QString value)
{

    update();
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

    //check all line edits
    if ("" == mStreet->text())
    {
        mStreet->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
        checkOk = false;
    }
    else
        mStreet->setStyleSheet("");

    if ("" == mStreetNr->text())
    {
        mStreetNr->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
        checkOk = false;
    }
    else
        mStreetNr->setStyleSheet("");

    if ("" == mAreaCode->text())
    {
        mAreaCode->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
        checkOk = false;
    }
    else
        mAreaCode->setStyleSheet("");

    if ("" == mCity->text())
    {
        mCity->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
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

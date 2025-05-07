#include "addrecipientview.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include "iointerface.h"

AddRecipientView::AddRecipientView (QWidget *parent)
    : WinSubmenu {parent}
{
    mIsCompany = 0;
    mLabelRecipientName = 0;
    mRecipientNameEdit = 0;
    mStreetEdit = 0;
    mStreetNumberEdit = 0;
    mAreaCodeEdit = 0;
    mCityEdit = 0;
    mRecipientType = RecipientType::Company; // usual case

    setHeader("Empfänger anlegen");

    QGridLayout* gridLayout = new QGridLayout ();

    mIsCompany = new QRadioButton ("Firma", this);
    mIsCompany->setChecked(true); // this is the common case
    QRadioButton *isPrivatePerson = new QRadioButton ("Privatperson", this);
    QRadioButton *isEmployee = new QRadioButton ("Mitarbeiter", this);

    // isPrivatePerson->setStyleSheet("QRadioButton::indicator {width: 25px; height 25px;}");
    // isEmployee->setStyleSheet("QRadioButton::indicator {width: 25px; height 25px;}");
    // mIsCompany->setStyleSheet("QRadioButton::indicator {width: 25px; height 25px;}");

    QHBoxLayout *recipientTypeBtnBox = new QHBoxLayout ();
    recipientTypeBtnBox->addWidget(mIsCompany);
    recipientTypeBtnBox->addWidget(isPrivatePerson);
    recipientTypeBtnBox->addWidget(isEmployee);

    mLabelRecipientName = new QLabel ("Firmenbezeichnung", this);
    mRecipientNameEdit = new QLineEdit (this);

    QLabel *street = new QLabel ("Straße", this);
    mStreetEdit = new QLineEdit (this);
    //mStreetEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\w+"), mStreetEdit));

    QLabel *streetNumber = new QLabel ("Hausnummer", this);
    mStreetNumberEdit = new QLineEdit (this);
    // accept only a digit in first place, followed by digits or text
    mStreetNumberEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d+\\w+"), mStreetNumberEdit));

    QLabel *areaCode = new QLabel ("PLZ", this);
    // accept only digits (5)
    mAreaCodeEdit = new QLineEdit (this);
    mAreaCodeEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d\\d\\d\\d\\d"), mAreaCodeEdit));

    QLabel *city= new QLabel ("Stadt", this);
    mCityEdit = new QLineEdit (this);
    //mCityEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\w+\\^( |ß|-)\\w+\\^( |ß|-)"), mCityEdit));

    connect (mIsCompany, SIGNAL (clicked()), this, SLOT (onIsCompanyBtnClicked()));
    connect (isPrivatePerson, SIGNAL (clicked()), this, SLOT (onIsPrivatePersonBtnClicked()));
    connect (isEmployee, SIGNAL (clicked()), this, SLOT (onIsEmployeeBtnClicked()));

    gridLayout->addLayout(recipientTypeBtnBox, 1, 0, 1, 2);

    gridLayout->addWidget(mLabelRecipientName, 2, 0, 1, 1);
    gridLayout->addWidget(mRecipientNameEdit, 2, 1, 1, 1);

    gridLayout->addWidget(street, 3, 0, 1, 1);
    gridLayout->addWidget(mStreetEdit, 3, 1, 1, 1);

    gridLayout->addWidget(streetNumber, 4, 0, 1, 1);
    gridLayout->addWidget(mStreetNumberEdit, 4, 1, 1, 1);

    gridLayout->addWidget(areaCode, 5, 0, 1, 1);
    gridLayout->addWidget(mAreaCodeEdit, 5, 1, 1, 1);

    gridLayout->addWidget(city, 6, 0, 1, 1);
    gridLayout->addWidget(mCityEdit, 6, 1, 1, 1);

    layout()->addItem(gridLayout);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout()->addItem(spacer);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Ok);
    setMenuButtons(menuButtons);

    // line edit manipulators
    // convert all chars to uppercase
    connect(mStreetNumberEdit, SIGNAL(textChanged(QString)), SLOT(toUpper(QString)));

    // trigger screen update, when values change -> otherwise, screen update on android target is buggy
    connect (mRecipientNameEdit, SIGNAL(textChanged(QString)), this, SLOT(onValueChanged(QString)));
    connect (mStreetEdit, SIGNAL(textChanged(QString)), this, SLOT(onValueChanged(QString)));
    connect (mStreetNumberEdit, SIGNAL(textChanged(QString)), this, SLOT(onValueChanged(QString)));
    connect (mAreaCodeEdit, SIGNAL(textChanged(QString)), this, SLOT(onValueChanged(QString)));
    connect (mCityEdit, SIGNAL(textChanged(QString)), this, SLOT(onValueChanged(QString)));
}

void AddRecipientView::clearForm()
{
    mIsCompany->setChecked(true);
    mLabelRecipientName->setText("Firmenbezeichnung");
    mRecipientType = RecipientType::Company;
    mRecipientNameEdit->setText("");
    mStreetEdit->setText("");
    mStreetNumberEdit->setText("");
    mAreaCodeEdit->setText("");
    mCityEdit->setText("");
}

void AddRecipientView::toUpper(QString text)
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(sender());

    if (lineEdit)
        lineEdit->setText(text.toUpper());
}

void AddRecipientView::onMenuBtnClicked (Gui::MenuButton btnType)
{
    switch (btnType)
    {
        // user wants to add recipient
        case (Gui::Ok):
            // check editable fiels
            if (checkValues ())
            {
                qDebug () << "AddRecipientView::onMenuBtnClicked OK";
                IOInterface::recipientData *data = new IOInterface::recipientData ();
                data->areaCode = mAreaCodeEdit->text();
                data->city = mCityEdit->text();
                data->name = mRecipientNameEdit->text();
                data->street = mStreetEdit->text();
                data->number = mStreetNumberEdit->text();
                data->type = mRecipientType;

                ioInterface()->addNewRecipient(data);
                delete data;

                clearForm();

                emit menuButtonClicked(btnType);
            }
            break;
        case (Gui::Back):
                clearForm();
                emit menuButtonClicked(btnType);
                break;

        // fall through for any other button
        default:
            emit menuButtonClicked(btnType);
            break;
    }
}

bool AddRecipientView::checkValues ()
{
    bool checkOk = true;

    //check all line edits
    if ("" == mRecipientNameEdit->text())
    {
        mRecipientNameEdit->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
        checkOk = false;
    }
    else
        mRecipientNameEdit->setStyleSheet("");

    if ("" == mStreetEdit->text())
    {
        mStreetEdit->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
        checkOk = false;
    }
    else
        mStreetEdit->setStyleSheet("");

    if ("" == mStreetNumberEdit->text())
    {
        mStreetNumberEdit->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
        checkOk = false;
    }
    else
        mStreetNumberEdit->setStyleSheet("");

    if ("" == mAreaCodeEdit->text())
    {
        mAreaCodeEdit->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
        checkOk = false;
    }
    else
        mAreaCodeEdit->setStyleSheet("");

    if ("" == mCityEdit->text())
    {
        mCityEdit->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
        checkOk = false;
    }
    else
        mCityEdit->setStyleSheet("");

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

void AddRecipientView::onValueChanged (QString value)
{
    Q_UNUSED(value);

    // do manual screen update, when value is changed.
    // otherwise, visualisation in android target is buggy
    update();
}

void AddRecipientView::onIsCompanyBtnClicked ()
{
    mLabelRecipientName->setText("Firmenbezeichnung");
    mRecipientType = RecipientType::Company;
    update (); // do a screen update, since behaviour with android is like there is no auto screen refresh after clicking a radio button.
}

void AddRecipientView::onIsPrivatePersonBtnClicked ()
{
    mLabelRecipientName->setText("Empfängername");
    mRecipientType = RecipientType::PrivatePerson;
    update (); // do a screen update, since behaviour with android is like there is no auto screen refresh after clicking a radio button.
}

void AddRecipientView::onIsEmployeeBtnClicked ()
{
    mLabelRecipientName->setText("Mitarbeitername");
    mRecipientType = RecipientType::Employee;
    update (); // do a screen update, since behaviour with android is like there is no auto screen refresh after clicking a radio button.
}

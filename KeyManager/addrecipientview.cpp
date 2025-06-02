#include "addrecipientview.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QGroupBox>
#include <QFormLayout>
#include "iointerface.h"

AddRecipientView::AddRecipientView (QWidget *parent)
    : WinSubmenu {parent}
{
    setHeader("Neuer Empfänger");

    mIsCompany = 0;
    mLabelRecipientName = 0;
    mRecipientNameEdit = 0;
    mStreetEdit = 0;
    mStreetNumberEdit = 0;
    mAreaCodeEdit = 0;
    mCityEdit = 0;
    mRecipientType = RecipientType::Company; // usual case

    QVBoxLayout* mainLayout= new QVBoxLayout();

    mIsCompany = new QRadioButton ("Firma", this);
    mIsCompany->setChecked(true); // this is the common case
    QRadioButton *isPrivatePerson = new QRadioButton ("Privatperson", this);
    QRadioButton *isEmployee = new QRadioButton ("Mitarbeiter", this);

    // isPrivatePerson->setStyleSheet("QRadioButton::indicator {width: 25px; height 25px;}");
    // isEmployee->setStyleSheet("QRadioButton::indicator {width: 25px; height 25px;}");
    // mIsCompany->setStyleSheet("QRadioButton::indicator {width: 25px; height 25px;}");

    QVBoxLayout *recipientTypeBtnBox = new QVBoxLayout ();
    recipientTypeBtnBox->setSizeConstraint(QLayout::SetMinimumSize);
    recipientTypeBtnBox->addWidget(mIsCompany);
    recipientTypeBtnBox->addWidget(isPrivatePerson);
    recipientTypeBtnBox->addWidget(isEmployee);

    QGroupBox *recipientType = new QGroupBox("Empfängertyp");
    recipientType->setLayout(recipientTypeBtnBox);

    mRecipientNameEdit = new QLineEdit ();
    mStreetEdit = new QLineEdit ();
    //mStreetEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\w+"), mStreetEdit));
    mStreetNumberEdit = new QLineEdit ();
    // accept only a digit in first place, followed by digits or text
    mStreetNumberEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d+\\w+"), mStreetNumberEdit));
    // accept only digits (5)
    mAreaCodeEdit = new QLineEdit ();
    mAreaCodeEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d\\d\\d\\d\\d"), mAreaCodeEdit));
    mCityEdit = new QLineEdit ();
    //mCityEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\w+\\^( |ß|-)\\w+\\^( |ß|-)"), mCityEdit));
    mLabelRecipientName = new QLabel();

    QGroupBox *recipientAddressData = new QGroupBox ("Empfängeranschrift");
    QFormLayout *recipientDataLayout = new QFormLayout ();
    recipientDataLayout->setSizeConstraint(QLayout::SetMinimumSize);
    recipientDataLayout->addRow(mLabelRecipientName, mRecipientNameEdit);
    recipientDataLayout->addRow(tr("Straße"), mStreetEdit);
    recipientDataLayout->addRow(tr("Hausnummer"), mStreetNumberEdit);
    recipientDataLayout->addRow(tr("PLZ"), mAreaCodeEdit);
    recipientDataLayout->addRow(tr("Stadt"), mCityEdit);
    recipientAddressData->setLayout(recipientDataLayout);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    mainLayout->addWidget(recipientType);
    mainLayout->addWidget(recipientAddressData);
    mainLayout->addSpacerItem(spacer);

    setCentralLayout(mainLayout);

    //set inital values
    mIsCompany->setChecked(true);
    mLabelRecipientName->setText("Firmenbezeichnung");

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Ok);
    setMenuButtons(menuButtons);

    // line edit manipulators
    // convert all chars of a street number to uppercase
    connect(mStreetNumberEdit, SIGNAL(textChanged(QString)), SLOT(toUpper(QString)));

    connect (mIsCompany, SIGNAL (clicked()), this, SLOT (onIsCompanyBtnClicked()));
    connect (isPrivatePerson, SIGNAL (clicked()), this, SLOT (onIsPrivatePersonBtnClicked()));
    connect (isEmployee, SIGNAL (clicked()), this, SLOT (onIsEmployeeBtnClicked()));
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

void AddRecipientView::onIsCompanyBtnClicked ()
{
    mLabelRecipientName->setText("Firmenbezeichnung");
    mRecipientType = RecipientType::Company;
}

void AddRecipientView::onIsPrivatePersonBtnClicked ()
{
    mLabelRecipientName->setText("Empfängername");
    mRecipientType = RecipientType::PrivatePerson;
}

void AddRecipientView::onIsEmployeeBtnClicked ()
{
    mLabelRecipientName->setText("Mitarbeitername");
    mRecipientType = RecipientType::Employee;
}

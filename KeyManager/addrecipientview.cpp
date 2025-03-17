#include "addrecipientview.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QRegularExpressionValidator>
#include <QMessageBox>

AddRecipientView::AddRecipientView(QWidget *parent)
    : QWidget{parent}
{
    mLabelRecipientName = 0;

    mRecipientNameEdit = 0;
    mStreetEdit = 0;
    mStreetNumberEdit = 0;
    mAreaCodeEdit = 0;
    mCityEdit = 0;
    mRecipientType = RecipientType::Company; // usual case

    QGridLayout* layout = new QGridLayout (this);

    QLabel *header= new QLabel ("Empfänger anlegen", this);

    QRadioButton *isCompany = new QRadioButton ("Firma");
    isCompany->setChecked(true); // this is the common case
    QRadioButton *isPrivatePerson = new QRadioButton ("Privatperson");
    QRadioButton *isEmployee = new QRadioButton ("Mitarbeiter");
    QButtonGroup *btnGroup = new QButtonGroup (this);

    btnGroup->addButton(isCompany, 0);
    btnGroup->addButton(isPrivatePerson, 1);
    btnGroup->addButton(isEmployee, 2);

    QHBoxLayout *recipientTypeBtnBox = new QHBoxLayout (this);
    recipientTypeBtnBox->addWidget(isCompany);
    recipientTypeBtnBox->addWidget(isPrivatePerson);
    recipientTypeBtnBox->addWidget(isEmployee);
    QSpacerItem *btnSpacer = new QSpacerItem (0, 0, QSizePolicy::Expanding,QSizePolicy::Minimum);
    recipientTypeBtnBox->addSpacerItem(btnSpacer);

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

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding,QSizePolicy::Expanding);

    QPushButton* btnPrevious = new QPushButton (this);
    btnPrevious->setIcon(QIcon(":/images/menu_back.png"));
    btnPrevious->setIconSize(QSize(UiDimensions::buttonWidth,UiDimensions::buttonHeight));

    QPushButton* btnOk = new QPushButton (this);
    btnOk->setIcon(QIcon(":/images/btn_Ok.png"));
    btnOk->setIconSize(QSize(UiDimensions::buttonWidth,UiDimensions::buttonHeight));
    //btnOk->setDisabled(true);

    //prev+retry+next button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout (this);
    buttonLayout->addWidget(btnPrevious);
    buttonLayout->addWidget(btnOk);

    connect (btnPrevious, SIGNAL (clicked()), this, SLOT (onPreviousBtnClicked()));
    connect (btnOk, SIGNAL (clicked()), this, SLOT (onOkBtnClicked()));
    connect (isCompany, SIGNAL (clicked()), this, SLOT (onIsCompanyBtnClicked()));
    connect (isPrivatePerson, SIGNAL (clicked()), this, SLOT (onIsPrivatePersonBtnClicked()));
    connect (isEmployee, SIGNAL (clicked()), this, SLOT (onIsEmployeeBtnClicked()));

    layout->addWidget(header, 0, 0, 1, 2, Qt::AlignCenter);
    layout->addLayout(recipientTypeBtnBox, 1, 0, 1, 2);

    layout->addWidget(mLabelRecipientName, 2, 0, 1, 1);
    layout->addWidget(mRecipientNameEdit, 2, 1, 1, 1);

    layout->addWidget(street, 3, 0, 1, 1);
    layout->addWidget(mStreetEdit, 3, 1, 1, 1);

    layout->addWidget(streetNumber, 4, 0, 1, 1);
    layout->addWidget(mStreetNumberEdit, 4, 1, 1, 1);

    layout->addWidget(areaCode, 5, 0, 1, 1);
    layout->addWidget(mAreaCodeEdit, 5, 1, 1, 1);

    layout->addWidget(city, 6, 0, 1, 1);
    layout->addWidget(mCityEdit, 6, 1, 1, 1);

    layout->addItem(spacer, 7, 0, 1, 1);
    layout->addLayout(buttonLayout, 8, 0, 1, 2);

    setLayout(layout);

    // line edit manipulators
    // convert all chars to uppercase
    connect(mStreetNumberEdit, SIGNAL(textChanged(QString)), SLOT(toUpper(QString)));
}

const RecipientData AddRecipientView::getRecipientData ()
{
    mRecipientData.type = mRecipientType;
    mRecipientData.name = mRecipientNameEdit->displayText();
    mRecipientData.street = mStreetEdit->displayText();
    mRecipientData.number = mStreetNumberEdit->displayText();
    mRecipientData.areaCode = mAreaCodeEdit->displayText();
    mRecipientData.city = mCityEdit->displayText();

    return mRecipientData;
}

void AddRecipientView::toUpper(QString text)
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(sender());

    if (!lineEdit)
        return;

    lineEdit->setText(text.toUpper());
}

void AddRecipientView::onPreviousBtnClicked ()
{
    emit previousButtonClicked();
}

void AddRecipientView::onOkBtnClicked ()
{
    bool fail = false;

    //check all line edits
    if ("" == mRecipientNameEdit->displayText())
    {
        mRecipientNameEdit->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
        fail = true;
    }
    else
        mRecipientNameEdit->setStyleSheet("");

    if ("" == mStreetEdit->displayText())
    {
        mStreetEdit->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
        fail = true;
    }
    else
        mStreetEdit->setStyleSheet("");

    if ("" == mStreetNumberEdit->displayText())
    {
        mStreetNumberEdit->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
        fail = true;
    }
    else
        mStreetNumberEdit->setStyleSheet("");

    if ("" == mAreaCodeEdit->displayText())
    {
        mAreaCodeEdit->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
        fail = true;
    }
    else
        mAreaCodeEdit->setStyleSheet("");

    if ("" == mCityEdit->displayText())
    {
        mCityEdit->setStyleSheet("border-style: solid;border-width: 1px;border-color: red");
        fail = true;
    }
    else
        mCityEdit->setStyleSheet("");

    if (fail)
    {
        QMessageBox::information(0, "Unvollständige Eingaben",
                              "Einige erforderliche Felder wurden \n"
                              "nicht ausgefüllt. Bitte prüfen Sie \n "
                              "Ihre Eingaben.", QMessageBox::Ok);
    }

    if (!fail)
        emit okButtonClicked();
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

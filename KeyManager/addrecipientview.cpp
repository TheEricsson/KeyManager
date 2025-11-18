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
#include "datainterface.h"
#include "viewdatarecipient.h"

AddRecipientView::AddRecipientView (QWidget *parent)
    : WinSubmenu {parent}
{
    setHeader("Neuer Empfänger");

    mIsCompany = 0;
    mIsPrivatePerson = 0;
    mIsEmployee = 0;
    mLabelRecipientName = 0;
    mRecipientNameEdit = 0;
    mStreetEdit = 0;
    mStreetNumberEdit = 0;
    mAreaCodeEdit = 0;
    mCityEdit = 0;
    mRecipientType = RecipientType::Company; // usual case
    mRecipientId = _UNDEFINED;
    mViewMode = ViewMode::Undefined;

    QVBoxLayout* mainLayout= new QVBoxLayout();

    mIsCompany = new QRadioButton ("Unternehmen", this);
    mIsCompany->setChecked(true); // this is the common case
    mIsPrivatePerson = new QRadioButton ("Privatperson", this);
    mIsEmployee = new QRadioButton ("Mitarbeiter", this);

    QVBoxLayout *recipientTypeBtnBox = new QVBoxLayout ();
    recipientTypeBtnBox->setSizeConstraint(QLayout::SetMinimumSize);
    recipientTypeBtnBox->addWidget(mIsCompany);
    recipientTypeBtnBox->addWidget(mIsPrivatePerson);
    recipientTypeBtnBox->addWidget(mIsEmployee);

    QGroupBox *recipientType = new QGroupBox("Empfängertyp");
    recipientType->setLayout(recipientTypeBtnBox);

    mRecipientNameEdit = new QLineEdit ();
    mStreetEdit = new QLineEdit ();
    //mStreetEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\w+"), mStreetEdit));
    mStreetNumberEdit = new QLineEdit ();
    // accept only a digit in first place, followed by digits or text
    //mStreetNumberEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d+\\w+"), mStreetNumberEdit));
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
    connect (mStreetNumberEdit, SIGNAL(textChanged(QString)), SLOT(toUpper(QString)));
    connect (mIsCompany, SIGNAL (clicked()), this, SLOT (onIsCompanyBtnClicked()));
    connect (mIsPrivatePerson, SIGNAL (clicked()), this, SLOT (onIsPrivatePersonBtnClicked()));
    connect (mIsEmployee, SIGNAL (clicked()), this, SLOT (onIsEmployeeBtnClicked()));
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
    mRecipientId = _UNDEFINED;
    //mViewMode = ViewMode::Undefined;
}

void AddRecipientView::setViewMode (ViewMode::Value mode)
{
    qDebug()<< "AddRecipientView::setViewMode: " << mode;
    mViewMode = mode;

    if (mViewMode == ViewMode::NewData)
        clearForm();
}

void AddRecipientView::setData(unsigned int id, ViewDataRecipient *data)
{
    mRecipientId = id;
    setHeader("Empfängerdaten ändern");

    if (data)
    {
        switch (data->getRecipientType())
        {
            case RecipientType::Company:
                mIsCompany->setChecked(true);
                mLabelRecipientName->setText("Firmenbezeichnung");
                break;
            case RecipientType::Employee:
                mIsEmployee->setChecked(true);
                mLabelRecipientName->setText("Mitarbeitername");
                break;
            case RecipientType::PrivatePerson:
                mIsPrivatePerson->setChecked(true);
                mLabelRecipientName->setText("Empfängername");
                break;
            default:
                break;
        }

        mRecipientType = data->getRecipientType();
        mRecipientNameEdit->setText(data->getRecipientName());
        mStreetEdit->setText(data->getRecipientStreet());
        mStreetNumberEdit->setText(data->getRecipientStreetNumber());
        mAreaCodeEdit->setText(QString::number(data->getRecipientAreaCode()));
        mCityEdit->setText(data->getRecipientCity());
    }
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
            okButtonClicked();
            break;
        case (Gui::Back):
            clearForm();
            break;

        // fall through for any other button
        default:
            break;
    }
    emit menuButtonClicked(btnType);
}

void AddRecipientView::okButtonClicked()
{
    qDebug() << "AddRecipientView::okButtonClicked()";
    // check editable fiels
    if (checkValues ())
    {
        qDebug()<<"checkValues ok";
        if (dataInterface())
        {
            qDebug()<<"datainterface ok";
            dataInterface()->setRecipientAreaCode(mAreaCodeEdit->text().toInt());
            dataInterface()->setRecipientCity(mCityEdit->text());
            dataInterface()->setRecipientName(mRecipientNameEdit->text());
            dataInterface()->setRecipientStreet(mStreetEdit->text());
            dataInterface()->setRecipientStreetNumber(mStreetNumberEdit->text());
            dataInterface()->setRecipientType(mRecipientType);

            if (ioInterface())
            {
                qDebug()<<"ioInterface ok";
                switch (mViewMode)
                {
                //add new db entry
                case ViewMode::NewData:
                    ioInterface()->addNewRecipient(dataInterface()->getDataRecipient());
                    break;
                //edit existing db entry
                case ViewMode::EditData:
                    ioInterface()->updateRecipient(mRecipientId, dataInterface()->getDataRecipient());
                    break;
                //view mode not set
                default:
                    qDebug()<< "AddRecipientView::okButtonClicked: ViewMode not set!";
                    break;
                }
            }
        }
        clearForm();
    }
}

void AddRecipientView::showEvent(QShowEvent *)
{
    //clearForm();
    showFullScreen();
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

    if ("" == mAreaCodeEdit->text() || mAreaCodeEdit->text().length() < 5)
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

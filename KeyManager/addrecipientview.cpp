#include "addrecipientview.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpressionValidator>

AddRecipientView::AddRecipientView(QWidget *parent)
    : QWidget{parent}
{
    mRecipientNameEdit = 0;
    mStreetEdit = 0;
    mStreetNumberEdit = 0;
    mAreaCodeEdit = 0;
    mCityEdit = 0;

    QGridLayout* layout = new QGridLayout (this);

    QLabel *header= new QLabel ("Empfänger anlegen", this);

    QLabel *recipientName = new QLabel ("Name/Firma", this);
    mRecipientNameEdit = new QLineEdit (this);

    QLabel *street = new QLabel ("Straße", this);
    mStreetEdit = new QLineEdit (this);
    mStreetEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\w+"), mStreetEdit));

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
    mCityEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\w+"), mCityEdit));

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding,QSizePolicy::Expanding);

    //prev+retry+next button
    QHBoxLayout *buttonLayout = new QHBoxLayout (this);

    QPushButton* btnPrevious = new QPushButton (this);
    btnPrevious->setIcon(QIcon(":/images/menu_back.png"));
    btnPrevious->setIconSize(QSize(75,75));

    QPushButton* btnOk = new QPushButton (this);
    btnOk->setIcon(QIcon(":/images/btn_Ok.png"));
    btnOk->setIconSize(QSize(75,75));
    //btnOk->setDisabled(true);

    buttonLayout->addWidget(btnPrevious);
    buttonLayout->addWidget(btnOk);

    connect (btnPrevious, SIGNAL (clicked()), this, SLOT (onPreviousBtnClicked()));
    connect (btnOk, SIGNAL (clicked()), this, SLOT (onOkBtnClicked()));

    layout->addWidget(header, 0, 0, 1, 2, Qt::AlignHCenter);
    layout->addWidget(recipientName, 1, 0);
    layout->addWidget(mRecipientNameEdit, 1, 1);
    layout->addWidget(street, 2, 0);
    layout->addWidget(mStreetEdit, 2, 1);
    layout->addWidget(streetNumber, 3, 0);
    layout->addWidget(mStreetNumberEdit, 3, 1);
    layout->addWidget(areaCode, 4, 0);
    layout->addWidget(mAreaCodeEdit, 4, 1);
    layout->addWidget(city, 5, 0);
    layout->addWidget(mCityEdit, 5, 1);
    layout->addItem(spacer, 6, 0);
    layout->addLayout(buttonLayout,7,0,1,2);

    setLayout(layout);

    // line edit manipulators
    // convert all chars to uppercase
    connect(mStreetNumberEdit, SIGNAL(textChanged(QString)), SLOT(toUpper(QString)));
    // check if line edits are empty
    connect(mRecipientNameEdit, SIGNAL(focusOutEvent(QFocusEvent*)), SLOT(checkLineEditContent()));
    connect(mStreetEdit, SIGNAL(selectionChanged()), SLOT(checkLineEditContent()));
    connect(mStreetNumberEdit, SIGNAL(selectionChanged()), SLOT(checkLineEditContent()));
    connect(mAreaCodeEdit, SIGNAL(selectionChanged()), SLOT(checkLineEditContent()));
    connect(mCityEdit, SIGNAL(selectionChanged()), SLOT(checkLineEditContent()));
}

const RecipientData AddRecipientView::getRecipientData ()
{
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

void AddRecipientView::checkLineEditContent()
{
    qDebug () << "checkLineEditContent";

    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(sender());

    if (!lineEdit)
        return;

    if ("" == lineEdit->displayText())
        lineEdit->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
    else
        lineEdit->setStyleSheet("");
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
        fail = true;
    if ("" == mStreetEdit->displayText())
        fail = true;
    if ("" == mStreetNumberEdit->displayText())
        fail = true;
    if ("" == mAreaCodeEdit->displayText())
        fail = true;
    if ("" == mCityEdit->displayText())
        fail = true;

    if (!fail)
        emit okButtonClicked();
}

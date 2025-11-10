#include "keycodeinputdialog.h"
#include <QInputDialog>
#include <QString>

KeycodeInputDialog::KeycodeInputDialog (QWidget *parent)
    : QInputDialog {parent}
{
    mCodeValid = false;

    connect (this, SIGNAL(textValueChanged(const QString&)), this, SLOT(onTextValueChanged(const QString&)));
    connect (this, SIGNAL (accepted()), this, SLOT(onAcceptedClicked()));
}

bool KeycodeInputDialog::isCodeValid()
{
    return mCodeValid;
}

void KeycodeInputDialog::onTextValueChanged(const QString& text)
{
    mCodeValid = false;
    //input format: 0000-0000
    //check current pos
    int pos = text.size();

    qDebug () <<  "pos: " << pos;
    qDebug () <<  "value: " << text;

    if (pos == 5)
        return;

    if (pos == 4)
    {
        QString manipulated = text;
        manipulated.append("-");
        this->setTextValue(manipulated);
        return;
    }

    if (pos > 9)
    {
        QString manipulated = text;
        manipulated.removeLast();
        this->setTextValue(manipulated);
        qDebug () << "mCodeValid oK";
        mCodeValid = true;
        return;
    }

    if (pos > 0 && pos < 5)
    {
        if (!text.back().isDigit())
        {
            QString manipulated = text;
            manipulated.removeLast();
            this->setTextValue(manipulated);
            return;
        }
    }

    if (pos > 5 && pos < 9)
    {
        if (!text.back().isDigit())
        {
            QString manipulated = text;
            manipulated.removeLast();
            this->setTextValue(manipulated);
            return;
        }
    }
}

void KeycodeInputDialog::onAcceptedClicked()
{
    QString value = textValue();

    if (value.size() > 5)
    {
        while (value.size() < 9)
        {
            value.insert(5,"0");
        }
        setTextValue(value);
        qDebug () << "mCodeValid oK";
        mCodeValid = true;
    }
}

#include "keycodeinputdialog.h"
#include <QInputDialog>
#include <QString>

KeycodeInputDialog::KeycodeInputDialog (QWidget *parent)
    : QInputDialog {parent}
{
    connect (this, SIGNAL(textValueChanged(const QString&)), this, SLOT(onTextValueChanged(const QString&)));
    connect (this, SIGNAL (accepted()), this, SLOT(onAcceptedClicked()));
}

void KeycodeInputDialog::onTextValueChanged(const QString& text)
{
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
    QString code = this->textValue();
    while (code.size() < 9)
    {
        code.insert(5,"0");
        this->setTextValue(code);
    }
}

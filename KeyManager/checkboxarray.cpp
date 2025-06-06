#include "checkboxarray.h"
#include "iointerface.h"
#include "datainterface.h"
#include <QCheckBox>
#include <QLayout>
#include <QRadioButton>
#include <QVBoxLayout>

CheckBoxArray::CheckBoxArray(QWidget *parent)
    : QWidget {parent}
{
    mRadioButtonList.clear();
    mLayout = new QVBoxLayout(this);
}

bool CheckBoxArray::init (IOInterface *ioInterface, const QString &tableName, const QString &valueColumn)
{
    //clear();

    bool ok = false;

    if(ioInterface)
    {
        int count = ioInterface->getNumberOfEntries(tableName);

        int dbIndex = 1;
        for (int i = 0; i < count; i++)
        {
            QRadioButton *checkBox = new QRadioButton (this);
            checkBox->setText(ioInterface->getValue(tableName, valueColumn, dbIndex).toString());
            checkBox->setChecked(false);
            registerButton (checkBox,i);
            dbIndex++;
        }
        ok = true;
    }
    return ok;
}

bool CheckBoxArray::init (const QStringList items)
{
    qDebug() << "CheckBoxArray::init";

    //clear();

    qDebug()<<"ok";
    for (int i = 0; i < items.count(); i++)
    {
        qDebug()<<"ok";
        QRadioButton *checkBox = new QRadioButton (this);
        checkBox->setText(items.at(i));
        checkBox->setChecked(false);
        registerButton (checkBox,i);
    }

    return true;
}

QRadioButton* CheckBoxArray::getButtonAt (unsigned int index)
{
    QRadioButton *btn = 0;

    if (index < mRadioButtonList.size())
        btn = mRadioButtonList[index].second;

    return btn;
}

bool CheckBoxArray::setButtonChecked (unsigned int index, bool check)
{
    bool retVal = false;

    //index out of range?
    if (index < mRadioButtonList.count())
    {
        QRadioButton *btn = mRadioButtonList[index].second;
        if (btn)
        {
            btn->setChecked(check);
            retVal = true;
        }
    }

    return retVal;
}

unsigned int CheckBoxArray::count()
{
    return mRadioButtonList.count();
}

int CheckBoxArray::getCheckedButtonIndex ()
{
    int retVal = _UNDEFINED;

    if (mRadioButtonList.empty())
        return retVal;

    for (int i = 0; i < mRadioButtonList.count(); i++)
    {
        QPair <int, QRadioButton*> itemPair;
        itemPair = mRadioButtonList[i];

        QRadioButton *btn = itemPair.second;
        if (0 != btn)
        {
            if (btn->isChecked())
                retVal = itemPair.first;
        }
    }
    return retVal;
}

void CheckBoxArray::registerButton (QRadioButton* btn, int index)
{
    if (0 != btn)
    {
        connect (btn, SIGNAL(clicked()), this, SLOT(onRadioBtnClicked()));

        QPair <int, QRadioButton*> itemPair;
        itemPair.first = index;
        itemPair.second = btn;
        mRadioButtonList.append(itemPair);

        if (mLayout)
        {
            mLayout->addWidget(btn);
        }
    }
}

void CheckBoxArray::onRadioBtnClicked()
{
    int btnIndex = getCheckedButtonIndex();
    emit radioBtnToggled(btnIndex);
}

CheckBoxArray::~CheckBoxArray()
{
    // delete active radio buttons
    /*if (!mRadioButtonList.empty())
    {
        for (int i = 0; i < mRadioButtonList.count (); i++)
        {
            QRadioButton *item = mRadioButtonList[i].second;
            if (nullptr != item)
            {
                item->deleteLater();
            }
        }

        mRadioButtonList.clear();
    }*/
}

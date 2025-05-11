#include "checkboxarray.h"
#include "iointerface.h"
#include "datainterface.h"
#include <QCheckBox>
#include <QLayout>
#include <QRadioButton>

CheckBoxArray::CheckBoxArray(QObject *parent)
    : QObject {parent}
{
    mIoInterface = nullptr;
    mDataInterface = nullptr;
    mRadioButtonList.clear();
}

void CheckBoxArray::setData (IOInterface *ioInterface, DataInterface *dataInterface)
{
    mIoInterface = ioInterface;
    mDataInterface = dataInterface;
}

bool CheckBoxArray::init (const QString &tableName, const QString &valueColumn)
{
    // delete active radio buttons
    if (!mRadioButtonList.empty())
    {
        for (int i = 0; i < mRadioButtonList.count (); i++)
        {
            QRadioButton *item = mRadioButtonList[i].second;
            if (nullptr != item)
                delete item;
        }

        mRadioButtonList.clear();
    }

    if (nullptr == mDataInterface)
    {
        qDebug () << "CheckBoxArray::init - mDataInterface is NULL";
        return false;
    }

    if (nullptr == mIoInterface)
    {
        qDebug () << "CheckBoxArray::init - mIOInterface is NULL";
        return false;
    }

    int count = mIoInterface->getNumberOfEntries(tableName);

    for (int i = 1; i <= count; i++)
    {
        QRadioButton *checkBox = new QRadioButton ();
        checkBox->setText(mIoInterface->getValue(tableName, valueColumn, i).toString());
        checkBox->setChecked(false);

        connect (checkBox, SIGNAL(clicked()), this, SLOT(onRadioBtnClicked()));

        QPair <int, QRadioButton*> itemPair;
        itemPair.first = i;
        itemPair.second = checkBox;
        mRadioButtonList.append(itemPair);
    }

    return true;
}

QRadioButton* CheckBoxArray::getButtonAt (unsigned int index)
{
    return mRadioButtonList[index].second;
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

void CheckBoxArray::onRadioBtnClicked()
{
    emit radioBtnToggled();
}

CheckBoxArray::~CheckBoxArray()
{
    // delete active radio buttons
    if (!mRadioButtonList.empty())
    {
        for (int i = 0; i < mRadioButtonList.count (); i++)
        {
            QRadioButton *item = mRadioButtonList[i].second;
            if (nullptr != item)
                delete item;
        }

        mRadioButtonList.clear();
    }
}

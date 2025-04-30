#include "viewstack.h"
#include <QDebug>

ViewStack::ViewStack ()
{
    mIterator = 0;
}

bool ViewStack::addView(QWidget *w)
{
    bool r = false;
    // call the base class method
    if (w)
    {
        mStackList.append(w);
        r = true;
    }
    return r;
}

QWidget* ViewStack::previous()
{
    QWidget *retVal = nullptr;

    if (0 < mIterator)
    {
        --mIterator;
        retVal = mStackList [mIterator];
    }
    return retVal;
}

QWidget* ViewStack::first ()
{
    mIterator = 0;
    QWidget *retVal = nullptr;

    if (!mStackList.empty())
        retVal = mStackList.first();

    return retVal;
}

QWidget* ViewStack::last ()
{
    mIterator = 0;
    QWidget *retVal = nullptr;

    if (!mStackList.empty())
    {
        retVal = mStackList.last();
        mIterator = mStackList.count()-1;
    }
    else
        mIterator = 0;

    return retVal;
}

QWidget* ViewStack::next ()
{
    QWidget *retVal = nullptr;
    mIterator++;

    if (mIterator < mStackList.size())
        retVal = mStackList [mIterator];

    return retVal;
}

QWidget* ViewStack::current ()
{
    QWidget *retVal = nullptr;

    if (!mStackList.empty ())
    {
        qDebug () << "ViewStack::getNextWidget Iterator: " << mIterator;
        retVal = mStackList [mIterator];
    }
    return retVal;
}

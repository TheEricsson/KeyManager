#include "viewstackmanager.h"
#include "globals.h"

ViewStackManager::ViewStackManager()
{
    mCurrentStackId = _UNDEFINED;
    mCurrentView = nullptr;
}

void ViewStackManager::addStackId (int queueId)
{
    mStackMap.insert(queueId, 0);
}

void ViewStackManager::setCurrentStackId (int queueId)
{
    mCurrentStackId = queueId;
}

const int& ViewStackManager::getCurrentStackId ()
{
    return mCurrentStackId;
}

void ViewStackManager::addNode (int queueId, QWidget *node)
{
    auto i = mStackMap.find(queueId);

    // queue yet unknown -> new viewstack and add first node
    if (i == mStackMap.end())
    {
        ViewStack *stack = new ViewStack ();
        stack->addView(node);
        mStackMap.insert(queueId, stack);
    }
    // queue is known -> add node to queue
    else
    {
        ViewStack *stack = i.value();
        if (stack)
            stack->addView(node);
    }
}

QWidget* ViewStackManager::begin ()
{
    return front(mCurrentStackId);;
}

QWidget* ViewStackManager::end ()
{
    return back(mCurrentStackId);;
}

QWidget* ViewStackManager::current ()
{
    return current(mCurrentStackId);
}

QWidget* ViewStackManager::previous ()
{
    return previous(mCurrentStackId);
}

QWidget* ViewStackManager::next ()
{  
    return next(mCurrentStackId);
}

QWidget* ViewStackManager::front (int queueId)
{
    auto i = mStackMap.find(queueId);

    // queue unknown
    if (i == mStackMap.end())
        return nullptr;
    // queue known -> get first view of the underlying stack
    else
    {
        ViewStack *stack = i.value();
        if (!stack)
            return nullptr;
        else
            return stack->first();
    }
}

QWidget* ViewStackManager::back (int queueId)
{
    auto i = mStackMap.find(queueId);

    // queue unknown
    if (i == mStackMap.end())
        return nullptr;
    // queue known -> get first view of the underlying stack
    else
    {
        ViewStack *stack = i.value();
        if (!stack)
            return nullptr;
        else
            return stack->last();
    }
}

QWidget* ViewStackManager::current (int queueId)
{
    auto i = mStackMap.find(queueId);

    // queue unknown
    if (i == mStackMap.end())
        return nullptr;
    // queue known -> get first view of the underlying stack
    else
    {
        ViewStack *stack = i.value();
        if (!stack)
            return nullptr;
        else
            return stack->current();
    }
}

QWidget* ViewStackManager::previous (int queueId)
{
    auto i = mStackMap.find(queueId);

    // queue unknown
    if (i == mStackMap.end())
        return nullptr;
    // queue known -> get first view of the underlying stack
    else
    {
        ViewStack *stack = i.value();
        if (!stack)
            return nullptr;
        else
            return stack->previous();
    }
}

QWidget* ViewStackManager::next (int queueId)
{
    auto i = mStackMap.find(queueId);

    qDebug () << "i.key: " << i.key();

    // queue unknown
    if (i == mStackMap.end())
        return nullptr;
    // queue known -> get next view of the underlying stack
    else
    {
        ViewStack *stack = i.value();
        if (!stack)
            return nullptr;
        else
        {
            return stack->next();
        }
    }
}

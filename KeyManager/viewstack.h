#ifndef VIEWSTACK_H
#define VIEWSTACK_H

#include <QList>

class QWidget;

class ViewStack
{
    public:
        ViewStack ();
        bool addView (QWidget *w);
        QWidget* previous();
        QWidget* first ();
        QWidget* last ();
        QWidget* current ();
        QWidget* next ();

    private:
        QList <QWidget*> mStackList;
        int mIterator = 0;
};

#endif // VIEWSTACK_H

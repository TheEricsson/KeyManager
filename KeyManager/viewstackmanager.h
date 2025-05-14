#ifndef VIEWSTACKMANAGER_H
#define VIEWSTACKMANAGER_H

#include <QWidget>
#include "viewstack.h"

class ViewStackManager
{
    public:
        ViewStackManager();
        void addStackId (int queueId);
        void setCurrentStackId (int queueId);
        const int& getCurrentStackId ();
        void addNode (int queueId, QWidget *node);
        QWidget* begin ();
        QWidget* end ();
        QWidget* current ();
        QWidget* previous ();
        QWidget* next ();

        enum ViewStackId
        {
            HandoverOut = 0,
            NewCodeScanned = 1,
            Search = 2,
            Settings = 3,
            Scan = 4,
            HandoverIn = 5,
            Tools = 6
        };

    private:
        QWidget *front (int queueId);
        QWidget *back (int queueId);
        QWidget *current (int queueId);
        QWidget *previous (int queueId);
        QWidget *next (int queueId);
        QMap <int, ViewStack*> mStackMap;
        int mCurrentStackId;
        QWidget *mCurrentView;
};

#endif // VIEWSTACKMANAGER_H

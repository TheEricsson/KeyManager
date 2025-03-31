#ifndef VIEWDATAHANDOVER_H
#define VIEWDATAHANDOVER_H

#include <QString>

class ViewDataHandover
{
    public:
        ViewDataHandover();
        void setAnnotation (const QString &data) {mAnnotation = data;};
        const QString& getAnnotation () {return mAnnotation;};
    private:
        QString mAnnotation;
};

#endif // VIEWDATAHANDOVER_H

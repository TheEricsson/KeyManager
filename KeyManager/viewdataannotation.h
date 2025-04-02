#ifndef VIEWDATAANNOTATION_H
#define VIEWDATAANNOTATION_H

#include <QString>

class ViewDataAnnotation
{
public:
    ViewDataAnnotation();
    void setAnnotation (const QString &data) {mAnnotation = data;};
    const QString& getAnnotation () {return mAnnotation;};
private:
    QString mAnnotation;
};

#endif // VIEWDATAANNOTATION_H

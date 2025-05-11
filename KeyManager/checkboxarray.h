#ifndef CHECKBOXARRAY_H
#define CHECKBOXARRAY_H

#include <QWidget>
#include "iointerface.h"
#include "datainterface.h"

class QRadioButton;

class CheckBoxArray : public QObject
{
    Q_OBJECT
    public:
        explicit CheckBoxArray(QObject *parent = nullptr);
        void setData (IOInterface *ioInterface = nullptr, DataInterface *dataInterface = nullptr);
        bool init (const QString &tableName, const QString &filter);
        QRadioButton* getButtonAt (unsigned int index);
        unsigned int count();
        int getCheckedButtonIndex ();
        ~CheckBoxArray();
    signals:
        void radioBtnToggled();

    private:
        IOInterface *mIoInterface;
        DataInterface *mDataInterface;
        QList <QPair <int,QRadioButton*>> mRadioButtonList;
    private slots:
        void onRadioBtnClicked();

signals:
};

#endif // CHECKBOXARRAY_H

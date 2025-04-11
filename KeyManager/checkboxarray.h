#ifndef CHECKBOXARRAY_H
#define CHECKBOXARRAY_H

#include <QWidget>
#include "iointerface.h"
#include "datainterface.h"

class QRadioButton;

class CheckBoxArray : public QWidget
{
    Q_OBJECT
    public:
        explicit CheckBoxArray(QWidget *parent = nullptr);
        void setData (IOInterface *ioInterface = nullptr, DataInterface *dataInterface = nullptr);
        bool init (const QString &tableName, const QString &filter);
        int getCheckedButtonIndex ();
        ~CheckBoxArray();

    private:
        IOInterface *mIoInterface;
        DataInterface *mDataInterface;
        QList <QPair <int,QRadioButton*>> mRadioButtonList;
    private slots:
        void onRadioBtnClicked();

signals:
};

#endif // CHECKBOXARRAY_H

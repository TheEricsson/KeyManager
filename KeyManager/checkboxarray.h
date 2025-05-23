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
        bool init (IOInterface *ioInterface, const QString &tableName, const QString &filter);
        bool init (const QStringList items);
        QRadioButton* getButtonAt (unsigned int index);
        unsigned int count();
        int getCheckedButtonIndex ();
        ~CheckBoxArray();
    signals:
        void radioBtnToggled();

    private:
        void clear();
        void registerButton (QRadioButton* btn, int index);
        QList <QPair <int,QRadioButton*>> mRadioButtonList;
    private slots:
        void onRadioBtnClicked();

signals:
};

#endif // CHECKBOXARRAY_H

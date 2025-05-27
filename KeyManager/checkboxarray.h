#ifndef CHECKBOXARRAY_H
#define CHECKBOXARRAY_H

#include <QWidget>
#include "iointerface.h"
#include "datainterface.h"

class QRadioButton;
class QVBoxLayout;

class CheckBoxArray : public QWidget
{
    Q_OBJECT
    public:
        explicit CheckBoxArray(QWidget *parent = nullptr);
        bool init (IOInterface *ioInterface, const QString &tableName, const QString &filter);
        bool init (const QStringList items);
        QRadioButton* getButtonAt (unsigned int index);
        bool setButtonChecked (unsigned int index, bool check);
        unsigned int count();
        int getCheckedButtonIndex ();
        ~CheckBoxArray();
    signals:
        void radioBtnToggled(int btnIndex);

    private:
        void registerButton (QRadioButton* btn, int index);
        QList <QPair <int, QRadioButton*>> mRadioButtonList;
        QVBoxLayout *mLayout;
    private slots:
        void onRadioBtnClicked();

signals:
};

#endif // CHECKBOXARRAY_H

#ifndef KEYCODEINPUTDIALOG_H
#define KEYCODEINPUTDIALOG_H

#include <QInputDialog>
#include <QObject>
#include <QWidget>

class KeycodeInputDialog : public QInputDialog
{
    Q_OBJECT
    public:
        explicit KeycodeInputDialog(QWidget* parent = nullptr);
        bool isCodeValid();
    private slots:
        void onTextValueChanged(const QString& text);
        void onAcceptedClicked();
    private:
        bool mCodeValid;
};

#endif // KEYCODEINPUTDIALOG_H

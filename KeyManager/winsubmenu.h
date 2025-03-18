#ifndef WINSUBMENU_H
#define WINSUBMENU_H

#include <QWidget>
#include "globals.h"

class QVBoxLayout;
class QLabel;
class QPushButton;

class WinSubmenu : public QWidget
{
    Q_OBJECT
public:
    explicit WinSubmenu (QWidget *parent = nullptr);

protected:
    void setHeader (const QString& label);
    void setMenuButtons (const UiSpecs::eMenuButton& column0 = UiSpecs::eMenuButton::None,
                        const UiSpecs::eMenuButton& column1 = UiSpecs::eMenuButton::None,
                        const UiSpecs::eMenuButton& column2 = UiSpecs::eMenuButton::None);
    void disableButton (int column, bool disable);
    void enableButton (int column, bool enable);

signals:
    void firstButtonClicked ();
    void secondButtonClicked ();
    void thirdButtonClicked ();

private slots:
    void onFirstBtnClicked ();
    void onSecondBtnClicked ();
    void onThirdBtnClicked ();

private:

    QPushButton *mBtnColumn0;
    QPushButton *mBtnColumn1;
    QPushButton *mBtnColumn2;

    QVBoxLayout *mLayout;
    QLabel *mHeaderLabel;
};

#endif // WINSUBMENU_H

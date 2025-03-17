#ifndef WINSUBMENU_H
#define WINSUBMENU_H

#include <QWidget>
#include "globals.h"

class QVBoxLayout;
class QLabel;

class WinSubmenu : public QWidget
{
    Q_OBJECT
public:
    explicit WinSubmenu (QWidget *parent = nullptr);
    void setHeader (const QString& label);
    void setMenuButtons (const UiSpecs::eMenuButton& column1 = UiSpecs::eMenuButton::None,
                        const UiSpecs::eMenuButton& column2 = UiSpecs::eMenuButton::None,
                        const UiSpecs::eMenuButton& column3 = UiSpecs::eMenuButton::None);

signals:
    void firstButtonClicked ();
    void secondButtonClicked ();
    void thirdButtonClicked ();

private slots:
    void onFirstBtnClicked ();
    void onSecondBtnClicked ();
    void onThirdBtnClicked ();

private:
    QVBoxLayout *mLayout;
    QLabel *mHeaderLabel;
};

#endif // WINSUBMENU_H

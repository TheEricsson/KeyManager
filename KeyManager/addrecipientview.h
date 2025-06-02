#ifndef ADDRECIPIENTVIEW_H
#define ADDRECIPIENTVIEW_H

#include <QWidget>
#include <QLineEdit>

#include "winsubmenu.h"
#include "globals.h"

class QLabel;
class QRadioButton;

class AddRecipientView : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit AddRecipientView(QWidget *parent = nullptr);
        void clearForm();

    private slots:
        void toUpper(QString text);
        virtual void onMenuBtnClicked (Gui::MenuButton btnType);
        void onIsCompanyBtnClicked ();
        void onIsPrivatePersonBtnClicked ();
        void onIsEmployeeBtnClicked ();

    private:

        bool checkValues ();

        QLabel* mLabelRecipientName;
        QRadioButton* mIsCompany;

        RecipientType::Value mRecipientType;
        QLineEdit* mRecipientNameEdit;
        QLineEdit* mStreetEdit;
        QLineEdit* mStreetNumberEdit;
        QLineEdit* mAreaCodeEdit;
        QLineEdit* mCityEdit;
};

#endif // ADDRECIPIENTVIEW_H

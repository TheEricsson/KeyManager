#ifndef ADDRECIPIENTVIEW_H
#define ADDRECIPIENTVIEW_H

#include <QWidget>
#include <QLineEdit>
#include "winsubmenu.h"
#include "globals.h"
#include "iointerface.h"

class QLabel;
class QRadioButton;
class ViewDataRecipient;

class AddRecipientView : public WinSubmenu
{
    Q_OBJECT
    public:

        explicit AddRecipientView(QWidget *parent = nullptr);
        void clearForm();
        void setViewMode (ViewMode::Value mode);
        void setData(unsigned int id, ViewDataRecipient *data);

    private slots:
        void toUpper(QString text);
        virtual void onMenuBtnClicked (Gui::MenuButton btnType);
        void onIsCompanyBtnClicked ();
        void onIsPrivatePersonBtnClicked ();
        void onIsEmployeeBtnClicked ();

    private:

        bool checkValues ();
        void okButtonClicked();

        QLabel* mLabelRecipientName;
        QRadioButton* mIsCompany;
        QRadioButton* mIsPrivatePerson;
        QRadioButton* mIsEmployee;

        RecipientType::Value mRecipientType;
        QLineEdit* mRecipientNameEdit;
        QLineEdit* mStreetEdit;
        QLineEdit* mStreetNumberEdit;
        QLineEdit* mAreaCodeEdit;
        QLineEdit* mCityEdit;

        ViewDataRecipient *mRecipientData;
        int mRecipientId;

        ViewMode::Value mViewMode;
};

#endif // ADDRECIPIENTVIEW_H

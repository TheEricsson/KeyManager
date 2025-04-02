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
    const RecipientData getRecipientData ();
    const QString getName ()    {return mRecipientNameEdit->displayText();};
    const QString getStreet ()  {return mStreetEdit->displayText();};
    const QString getNumber ()  {return mStreetNumberEdit->displayText();};
    const QString getAreaCode () {return mAreaCodeEdit->displayText();};
    const QString getCity ()    {return mCityEdit->displayText();};

signals:
    void previousButtonClicked ();
    void OkClicked ();

private slots:
    void toUpper(QString text);
    void onPreviousBtnClicked ();
    void onOkBtnClicked ();
    void onIsCompanyBtnClicked ();
    void onIsPrivatePersonBtnClicked ();
    void onIsEmployeeBtnClicked ();

private:

    QLabel* mLabelRecipientName;
    QRadioButton* mIsCompany;

    RecipientData mRecipientData;

    Database::RecipientType mRecipientType;
    QLineEdit* mRecipientNameEdit;
    QLineEdit* mStreetEdit;
    QLineEdit* mStreetNumberEdit;
    QLineEdit* mAreaCodeEdit;
    QLineEdit* mCityEdit;
};

#endif // ADDRECIPIENTVIEW_H

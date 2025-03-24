#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>

namespace UiSpecs
{
    //button dimensions
    const int buttonHeight = 90;
    const int buttonWidth = 90;
    const int buttonBorder = 3;

    const int buttonImgHeight = buttonHeight - buttonBorder;
    const int buttonImgWidth = buttonWidth - buttonBorder;

    //logo dimensions
    const int logoWidth = 300;
    const int logoHeight = 100;

    //subwindow button layouts
    enum eMenuButton
    {
        BackButton,
        RepeatButton,
        NextButton,
        OkButton,
        AddRecipientButton,
        None,
    };
}

namespace Database
{
    enum eKeychainStatusId
    {
        Available = 1,
        TemporaryOut = 2,
        PermanentOut = 3,
        AdministrationEnded = 4,
        Lost = 5
    };
}

enum RecipientType
{
    Company = 0,
    PrivatePerson = 1,
    Employee = 2
};

struct RecipientData
{
    RecipientType type;
    QString name;
    QString street;
    QString number;
    QString areaCode;
    QString city;
};

#endif // GLOBALS_H

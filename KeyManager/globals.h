#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>

const int _UNDEFINED = -999;

namespace Gui
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

    //menu button types
    enum MenuButton
    {
        Back,
        Repeat,
        Next,
        Ok,
        AddRecipient,
        Pdf,
        MainMenu,
        TakeBack,
        Handout,
        None,
        Scanner,
        Search,
        Settings,
        Exit
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
        Lost = 5,
        Undefined = 999
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

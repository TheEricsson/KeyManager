#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>

const int _UNDEFINED = -999;

namespace Gui
{
    //button dimensions
    const int buttonHeight = 60;
    const int buttonWidth = 60;
    const int buttonBorder = 2;

    const int buttonImgHeight = buttonHeight - buttonBorder;
    const int buttonImgWidth = buttonWidth - buttonBorder;

    //logo dimensions
    const int logoWidth = 600;
    const int logoHeight = 200;

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
        Exit,
        NewCodeScanned,
        AddKey,
        AddCustomer,
        Edit,
        Print
    };
}

namespace Database
{
    enum KeychainStatus
    {
        Available = 1,
        TemporaryOut = 2,
        PermanentOut = 3,
        AdministrationEnded = 4,
        Lost = 5,
        Undefined = 999
    };

    enum RecipientType
    {
        Company = 0,
        PrivatePerson = 1,
        Employee = 2
    };

    const int maxHandoverEntries = 10;
}

#endif // GLOBALS_H

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
        Print,
        TakePicture,
        Save,
        DeleteImage,
        NewImageFromCam
    };
}

namespace KeychainStatus
{
    enum Value
    {
        Available = 1,
        TemporaryOut = 2,
        PermanentOut = 3,
        AdministrationEnded = 4,
        Lost = 5,
        Undefined = 999
    };
}

namespace KeyStatus
{
    enum Value
    {
        Ok = 1,
        NoFit = 2,
        Lost = 3,
        Broken = 4,
        Undefined = 999
    };
}

namespace RecipientType
{
    enum Value
    {
        Company = 1,
        Employee = 2,
        PrivatePerson = 3
    };
}

namespace Database
{
    const int maxHandoverEntries = 50;

    inline const QString normaliseKeycode (const unsigned int code)
    {
        QString barcode = QString::number(code);
        while (barcode.length() < 8)
        {
            barcode.prepend("0");
        }
        barcode.insert(4,"-");
        return barcode;
    }
}

#endif // GLOBALS_H

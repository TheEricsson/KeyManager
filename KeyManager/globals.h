#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>

namespace UiDimensions
{
    //button dimensions
    const int buttonHeight = 90;
    const int buttonWidth = 90;
    const int buttonBorder = 5;

    const int buttonImgHeight = buttonHeight - buttonBorder;
    const int buttonImgWidth = buttonWidth - buttonBorder;

    //logo dimensions
    const int logoWidth = 300;
    const int logoHeight = 100;
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

struct RecipientData
{
    QString name;
    QString street;
    QString number;
    QString areaCode;
    QString city;
};

#endif // GLOBALS_H

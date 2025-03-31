#ifndef VIEWDATASCANNER_H
#define VIEWDATASCANNER_H

#include "globals.h"
#include "datainterface.h"

class ViewDataScanner
{
public:
    ViewDataScanner();
    void setBarcode (int code) {mCode = code;};
    const int& getBarcode () {return mCode;};

private:
    int mCode;
};

#endif // VIEWDATASCANNER_H

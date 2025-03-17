#ifndef HANDOVERVIEW_H
#define HANDOVERVIEW_H

#include <QWidget>
#include "winsubmenu.h"

class HandoverView : public WinSubmenu
{
    Q_OBJECT
public:
    explicit HandoverView (QWidget *parent = nullptr);
};

#endif // HANDOVERVIEW_H

#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QObject>
#include <QWidget>
#include "winsubmenu.h"

class QLabel;

class ImageView : public WinSubmenu
{
    public:
        ImageView(QWidget *parent = nullptr);
        void setImage(QImage &img);
    private:
        void showEvent(QShowEvent *);
        QLabel *mImagePane;
};

#endif // IMAGEVIEW_H

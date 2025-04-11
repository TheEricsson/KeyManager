#ifndef PRINTERINTERFACE_H
#define PRINTERINTERFACE_H

#include <QImage>

class PrinterInterface
{
    public:

        enum BorderStyle
        {
            ThinLine,
            Dotted,
            None
        };

        PrinterInterface();
        virtual ~PrinterInterface();
        virtual void saveAsFile(){};
        virtual void add (const QImage& img, QRect size, QRect margin, bool foldable = false, PrinterInterface::BorderStyle borderstyle = BorderStyle::None){};
        virtual void drawKeycode(QImage &img, int codeDim, int labelWidth = 0, bool foldable = false, Qt::PenStyle style = Qt::NoPen){};
        virtual void begin(){};
        virtual void finish(){};
};

#endif // PRINTERINTERFACE_H

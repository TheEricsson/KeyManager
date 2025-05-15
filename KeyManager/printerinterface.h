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

        enum imgSize
        {
            Small,
            Medium,
            Big
        };

        enum imgAlignment
        {
            Left,
            Centered,
            Right,
        };

        PrinterInterface();
        virtual ~PrinterInterface();
        virtual bool saveAsFile(){};
        virtual const QString getFilePath(){return "";};
        virtual void print(){};
        //virtual void add (const QImage& img, QRect size, QRect margin, bool foldable = false, PrinterInterface::BorderStyle borderstyle = BorderStyle::None){};
        virtual void drawQRCode(QImage &img, int codeDim, int labelWidth = 0, bool foldable = false, Qt::PenStyle style = Qt::NoPen){};
        virtual void insertImage(const QImage &img, PrinterInterface::imgSize size, PrinterInterface::imgAlignment alignment){};
        virtual void insertHLine(){};
        virtual void insertText(QString text){};
        virtual void insertHeaderH1(QString text){};
        virtual void insertHeaderH2(QString text){};

        virtual void begin(){};
        virtual void finish(){};
};

#endif // PRINTERINTERFACE_H

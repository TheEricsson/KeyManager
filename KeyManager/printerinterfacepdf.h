#ifndef PRINTERINTERFACEPDF_H
#define PRINTERINTERFACEPDF_H

#include <QPrinter>
#include <QPainter>
#include "printerinterface.h"

class PrinterInterfacePdf : public PrinterInterface
{
    public:
        PrinterInterfacePdf();
        void saveAsFile();
        //void add (const QImage& img, QRect size, QRect margin, bool foldable, PrinterInterface::BorderStyle borderstyle);
        void drawQRCode(QImage &img, int codeDim, int labelWidth, bool foldable = false, Qt::PenStyle style = Qt::NoPen);
        void insertImage(const QImage &img, PrinterInterface::imgSize, PrinterInterface::imgAlignment);
        void insertHLine();
        void insertText(QString text);
        void insertHeader(QString text);
        void begin();
        void finish();
    private:
        QPrinter mPrinter;
        QPainter mPainter;
        void drawBorder (int posX, int posY, int width, int height, Qt::PenStyle style);

        unsigned int mResolutionPPI;
        unsigned int mPosX;
        unsigned int mPosY;
        unsigned int mCurrentPage;
        unsigned int mPixelPerPageY;
        unsigned int mPixelPerPageX;
};

#endif // PRINTERINTERFACEPDF_H

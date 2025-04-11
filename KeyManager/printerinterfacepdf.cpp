#include "printerinterfacepdf.h"
#include <QFileDialog>
#include <QPainterPath>

PrinterInterfacePdf::PrinterInterfacePdf()
{
    mResolutionPPI = 300;

    mPrinter.setResolution(mResolutionPPI);
    mPrinter.setOutputFormat(QPrinter::PdfFormat);

    mPosX = 0;
    mPosY = 0;
}

void PrinterInterfacePdf::saveAsFile()
{
    //this->finish ();

    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }
    mPrinter.setOutputFileName(fileName);
}

void PrinterInterfacePdf::add (const QImage& img, QRect size, QRect margin, bool foldable, PrinterInterface::BorderStyle borderstyle)
{
    Q_UNUSED(foldable);
    Q_UNUSED(borderstyle);
    //QImage border (55, 55, QImage::Format_ARGB32); //todo

    mPainter.drawImage(QRect(mPosX,mPosY,size.width(), size.height()),img);

    mPosY += margin.height();
    mPosX += margin.width();
}

void PrinterInterfacePdf::drawKeycode(QImage &img, int codeDim, int labelWidth, bool foldable, Qt::PenStyle style)
{
    // input is in mm -> calculate factor depending on print resolution
    float conversionFactor = mResolutionPPI / 2.41 / 10;
    float printSize = conversionFactor * codeDim;
    float printSizeLabelWidth = conversionFactor * labelWidth;

    mPainter.drawImage(QRect(mPosX, mPosY, printSize, printSize), img);

    if (style != Qt::NoPen)
    {
        if (foldable)
            printSizeLabelWidth *= 2;

        drawBorder (mPosX, mPosY, printSizeLabelWidth, printSize, style);
    }

    mPosY += printSize + 5;
}

void PrinterInterfacePdf::drawBorder (int posX, int posY, int width, int height, Qt::PenStyle style)
{
    QPen pen (Qt::black, 3, style, Qt::SquareCap, Qt::RoundJoin);
    mPainter.setPen(pen);
    mPainter.drawRect(posX, posY, width, height);
}

void PrinterInterfacePdf::begin ()
{
    if (!mPainter.isActive())
        mPainter.begin(&mPrinter);
}

void PrinterInterfacePdf::finish ()
{
    if (mPainter.isActive())
        mPainter.end();
}

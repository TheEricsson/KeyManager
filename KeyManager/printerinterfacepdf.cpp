#include "printerinterfacepdf.h"
#include <QFileDialog>
#include <QPainterPath>
#include <QDesktopServices>

PrinterInterfacePdf::PrinterInterfacePdf()
{
    mResolutionPPI = 300;

    mPrinter.setResolution(mResolutionPPI);
    mPrinter.setOutputFormat(QPrinter::PdfFormat);
    mPrinter.setPageSize(QPageSize::A4);

    //ISO 216 standard in inches for A4 format
    float a4PageHeight = 11.7;
    float a4PageWidth = 8.3;

    mPixelPerPageY = a4PageHeight * mResolutionPPI;
    mPixelPerPageX = a4PageWidth * mResolutionPPI;

    mCurrentPage = 1;

    mPosX = 0;
    mPosY = 0;
}

void PrinterInterfacePdf::saveAsFile()
{
    // open file dialog and let user set location + file name
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }
    mPrinter.setOutputFileName(fileName);

    // open the file in file browser
    QDesktopServices::openUrl(QFileInfo(fileName).path());
}

// void PrinterInterfacePdf::add (const QImage& img, QRect size, QRect margin, bool foldable, PrinterInterface::BorderStyle borderstyle)
// {
//     Q_UNUSED(foldable);
//     Q_UNUSED(borderstyle);
//     //QImage border (55, 55, QImage::Format_ARGB32); //todo

//     mPainter.drawImage(QRect(mPosX,mPosY,size.width(), size.height()),img);

//     mPosY += margin.height();
//     mPosX += margin.width();
// }

void PrinterInterfacePdf::drawQRCode (QImage &img, int codeDim, int labelWidth, bool foldable, Qt::PenStyle style)
{
    // input is in mm -> calculate factor depending on print resolution
    float conversionFactor = mResolutionPPI / 2.41 / 10;
    float printSize = conversionFactor * codeDim;
    float printSizeLabelWidth = conversionFactor * labelWidth;

    // set printer position...
    int nextPosY = mPosY + printSize + 5;

    // ... and check, if page break is needed
    if (nextPosY >= mPixelPerPageY)
    {
        mCurrentPage += 1;
        mPosY = 0;
        nextPosY = printSize + 5;
        mPrinter.newPage();
    }

    // draw the image in document
    mPainter.drawImage(QRect(mPosX, mPosY, printSize, printSize), img);

    // draw border
    if (style != Qt::NoPen)
    {
        // foldable code
        if (foldable)
            printSizeLabelWidth *= 2;

        drawBorder (mPosX, mPosY, printSizeLabelWidth, printSize, style);
    }

    // set new printer position
    mPosY = nextPosY;
}

void PrinterInterfacePdf::insertImage(const QImage &img, PrinterInterface::imgSize, PrinterInterface::imgAlignment)
{
    // input is in mm -> calculate factor depending on print resolution
    float conversionFactor = mResolutionPPI / 2.41 / 10;
    float printSize = conversionFactor * 50;

    mPosY += 10;

    // draw the image in document
    mPainter.drawImage(QRect(mPosX, mPosY, img.width()*2, img.height()*2), img);

    // set printer position...
    int nextPosY = mPosY + printSize + 5;
    // set new printer position
    mPosY = nextPosY;
}

void PrinterInterfacePdf::insertHLine()
{
    mPosY += 30;
    int margin = 100;

    mPainter.drawLine(mPosX, mPosY, mPixelPerPageX - margin, mPosY);
}

void PrinterInterfacePdf::insertText(QString text)
{
    mPosY += 60;

    QFont font("Roboto");
    font.setStyleHint(QFont::SansSerif);
    font.setPixelSize(50);
    mPainter.setFont(font);

    mPainter.drawText(mPosX, mPosY, text);
}

void PrinterInterfacePdf::insertHeader(QString text)
{
    mPosY += 110;

    QFont font("Roboto");
    font.setStyleHint(QFont::SansSerif);
    font.setPixelSize(100);
    font.setBold(true);
    mPainter.setFont(font);

    mPainter.drawText(mPosX, mPosY, text);
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

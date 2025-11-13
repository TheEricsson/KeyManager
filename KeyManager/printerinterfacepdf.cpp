#include "printerinterfacepdf.h"
#include <QFileDialog>
#include <QPainterPath>
#include <QDesktopServices>
#include <QRegularExpression>
#include <QPrintDialog>
#include <QStandardPaths>
#include <QTextItem>

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
    mPageMargin = 75;

    mPosX = mPageMargin;
    mPosY = 0;

    mFilePath = "";
}

bool PrinterInterfacePdf::saveAsFile()
{  
    bool retVal = false;

    // open file dialog and let user set location + file name
    mFilePath = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");

    qDebug()<< "mFilePath: " << mFilePath;

    if (mFilePath != "")
    {
        if (QFileInfo(mFilePath).suffix().isEmpty())
        {
            mFilePath.append(".pdf");
        }
        mPrinter.setOutputFileName(mFilePath);
        retVal = true;
    }
    // dialog not accepted
    return retVal;
}

const QString PrinterInterfacePdf::getFilePath()
{
    return mFilePath;
}

void PrinterInterfacePdf::print()
{
    QPrintDialog printDialog;
    if (printDialog.exec() == QDialog::Accepted)
    {
        qDebug () << "print";
    }
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

void PrinterInterfacePdf::drawQRCode (QImage &img, int codeDim, int labelWidth, const QString& codeString, bool foldable, Qt::PenStyle style)
{
    // input is in mm -> calculate factor depending on print resolution
    float conversionFactor = mResolutionPPI / 2.41 / 10;
    float printSize = conversionFactor * codeDim;
    float printSizeLabelWidth = conversionFactor * labelWidth;
    float printSizeQr = printSize * 0.8;
    int posXQr = mPosX + ((printSize - printSizeQr)/2);
    int posYQr = mPosY + ((printSize - printSizeQr)/2);

    // set printer position...
    unsigned int nextPosY = mPosY + printSize + 5;

    // ... and check, if page break is needed
    if (nextPosY >= (mPixelPerPageY - mPageMargin))
    {
        mCurrentPage += 1;
        mPosY = 0;
        nextPosY = printSize + 5;
        mPrinter.newPage();
    }

    // draw the image in document
    //mPainter.drawImage(QRect(mPosX, mPosY, printSize, printSize), img);
    mPainter.drawImage(QRect(posXQr, posYQr, printSizeQr, printSizeQr), img);

    // draw border
    if (style != Qt::NoPen)
    {
        // foldable code
        if (foldable)
            printSizeLabelWidth *= 2;

        drawBorder (mPosX, mPosY, printSizeLabelWidth, printSize, style);
    }

    //draw code as string (only when foldable, on the back side)...
    if (foldable)
    {
        //string set?
        if ("" != codeString)
        {
            QString printString = "ID: ";
            printString.append(codeString);

            int textSize = printSize * 0.15;
            int margin = printSize * 0.05;

            QFont font("Roboto");
            font.setStyleHint(QFont::SansSerif);
            font.setPixelSize(textSize);
            mPainter.setFont(font);

            mPainter.drawText(mPosX + (printSizeLabelWidth/2) + margin, mPosY + margin + textSize, printString);

            QPen pen (Qt::black, 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
            mPainter.setPen(pen);

            //insert 3 lines
            for (int i = 0; i < 3; i++)
            {
                mPosY += printSize * 0.25;
                mPainter.drawLine(mPosX + printSizeLabelWidth/2 + margin, mPosY, printSizeLabelWidth, mPosY);
            }
        }
    }

    // set new printer position
    mPosY = nextPosY;
}

void PrinterInterfacePdf::insertImage(const QImage &img, PrinterInterface::imgSize size, PrinterInterface::imgAlignment alignment)
{
    float printSizeFactor = 1;
    QImage img_scaled;

    switch (size)
    {
        case PrinterInterface::Small:
            img_scaled = img.scaledToWidth(500, Qt::SmoothTransformation);
            break;
        case PrinterInterface::Medium:
            img_scaled = img.scaledToWidth(1000, Qt::SmoothTransformation);
            break;
        case PrinterInterface::Big:
            img_scaled = img.scaledToWidth(1600, Qt::SmoothTransformation);
            break;
        default:
            img_scaled = img;
            break;
    }

    switch (alignment)
    {
        case PrinterInterface::Left:
            mPosX = mPageMargin;
            break;
        case PrinterInterface::Centered:
            mPosX = (mPixelPerPageX / 2) - (img_scaled.width() / 2);
            break;
        case PrinterInterface::Right:
            mPosX = mPixelPerPageX - img_scaled.width() - mPageMargin;
            break;
        default:
            break;
    }

    mPosY += 10;

    // draw the image in document
    mPainter.drawImage(QRect(mPosX, mPosY, img_scaled.width(), img_scaled.height()), img_scaled);

    // set printer position...
    int nextPosY = mPosY + img_scaled.height() + 5;
    // set new printer position
    mPosY = nextPosY;
    mPosX = 0;
}

void PrinterInterfacePdf::insertHLine()
{
    mPosY += 25;
    mPosX = 0;

    mPainter.drawLine(mPosX + mPageMargin, mPosY, mPixelPerPageX - mPageMargin, mPosY);
}

void PrinterInterfacePdf::insertText(QString text)
{
    mPosX = mPageMargin + 20;
    mPosY += 15;

    QFont font("Roboto");
    font.setStyleHint(QFont::SansSerif);
    font.setPixelSize(40);
    mPainter.setFont(font);

    //mPainter.drawText(mPosX, mPosY, text);
    mPainter.drawText(QRect(mPosX, mPosY, mPixelPerPageX - mPageMargin, mPixelPerPageY - mPageMargin), text);

    mPosY += 45;
    //count number of line break characters, to set the correct y position after inserting multiline text
    int textlines = text.count("\n");
    for (int i = 0; i < textlines; i++)
    {
        mPosY += 45;
    }
}

void PrinterInterfacePdf::insertHeaderH1 (QString text)
{
    mPosX = mPageMargin;
    mPosY += 110;

    QFont font("Roboto");
    font.setStyleHint(QFont::SansSerif);
    font.setPixelSize(100);
    font.setBold(true);
    mPainter.setFont(font);

    mPainter.drawText(mPosX, mPosY, text);
}

void PrinterInterfacePdf::insertHeaderH2 (QString text)
{
    mPosX = mPageMargin;
    mPosY += 50;

    QFont font("Roboto");
    font.setStyleHint(QFont::SansSerif);
    font.setPixelSize(40);
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

#include "codegeneratorview.h"
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QGridLayout>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QCheckBox>
#include <QDesktopServices>
#include "iointerface.h"
#include "printerinterface.h"
#include "printerinterfacepdf.h"

CodeGeneratorView::CodeGeneratorView(QWidget *parent) : WinSubmenu {parent}
{
    mNumberOfCodes = 0;
    mPrintHeight = "";
    mPrintWidth = "";
    mBorderCheckState = Qt::Checked;
    mFoldableCheckState = Qt::Checked;
    mShowPdfFile = true;

    QLabel *numberOfCodes = new QLabel ("Anzahl gewünschter Codes:", this);
    QLineEdit *numberOfCodesEdit = new QLineEdit(this);
    numberOfCodesEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d\\d\\d"), numberOfCodesEdit));
    numberOfCodesEdit->setText(mNumberOfCodes);

    QLabel *printHeight = new QLabel ("Sichtfenster Höhe in mm:", this);
    QLineEdit *printHeightEdit = new QLineEdit (this);
    printHeightEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d\\d"), printHeightEdit));

    QLabel *printWidth = new QLabel ("Sichtfenster Breite in mm:", this);
    QLineEdit *printWidthEdit = new QLineEdit (this);
    printWidthEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d\\d"), printWidthEdit));

    QCheckBox *borders = new QCheckBox("Rahmen drucken?", this);
    borders->setCheckState(Qt::Checked);

    QCheckBox *foldable = new QCheckBox("Druck mit Rückseitenbeschriftung?", this);
    foldable->setCheckState(Qt::Checked);

    QCheckBox *openPdfAfterCreation = new QCheckBox("PDF nach Erstellung anzeigen?", this);
    openPdfAfterCreation->setCheckState(Qt::Checked);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    QGridLayout *viewLayout = new QGridLayout();
    viewLayout->addWidget(numberOfCodes, 0, 0);
    viewLayout->addWidget(numberOfCodesEdit, 0, 1);
    viewLayout->addWidget(printHeight, 1, 0);
    viewLayout->addWidget(printHeightEdit, 1, 1);
    viewLayout->addWidget(printWidth, 2, 0);
    viewLayout->addWidget(printWidthEdit, 2, 1);
    viewLayout->addWidget(borders, 3, 0);
    viewLayout->addWidget(foldable, 4, 0);
    viewLayout->addWidget(openPdfAfterCreation, 5, 0);
    viewLayout->addItem(spacer, 6, 0);

    setCentralLayout(viewLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Pdf);
    setMenuButtons(menuButtons);

    connect (numberOfCodesEdit, SIGNAL(textChanged(QString)), this, SLOT(onNumberOfCodesChanged(QString)));
    connect (printHeightEdit, SIGNAL(textChanged(QString)), this, SLOT(onprintHeightChanged(QString)));
    connect (printWidthEdit, SIGNAL(textChanged(QString)), this, SLOT(onprintWidthChanged(QString)));
    connect (borders, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(onBorderCheckStateChanged(Qt::CheckState)));
    connect (foldable, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(onFoldableCheckStateChanged(Qt::CheckState)));
    connect (openPdfAfterCreation, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(onOpenPdfCheckStateChanged(Qt::CheckState)));
}

void CodeGeneratorView::onMenuBtnClicked (Gui::MenuButton btnType)
{
    switch (btnType)
    {

        case (Gui::Pdf):
            // check valid values
            if (checkValues ())
            {
                generatePDF();
                emit menuButtonClicked(btnType);
            }
        break;
        // fall through for any other button
        default:
        emit menuButtonClicked(btnType);
        break;
    }
}

void CodeGeneratorView::onNumberOfCodesChanged(QString text)
{
    mNumberOfCodes = text;
}

void CodeGeneratorView::onprintHeightChanged(QString value)
{
    mPrintHeight = value;
}

void CodeGeneratorView::onprintWidthChanged(QString value)
{
    mPrintWidth = value;
}

void CodeGeneratorView::onBorderCheckStateChanged(Qt::CheckState value)
{
    mBorderCheckState = value;
}

void CodeGeneratorView::onFoldableCheckStateChanged(Qt::CheckState value)
{
    mFoldableCheckState = value;
}

void CodeGeneratorView::onOpenPdfCheckStateChanged(Qt::CheckState value)
{
    mShowPdfFile = value;
}

bool CodeGeneratorView::checkValues()
{
    bool retVal = false;

    if (mNumberOfCodes != "" && mNumberOfCodes != "0" && mPrintHeight != "" && mPrintWidth != "")
        retVal = true;
    else
    {
        retVal = false;
    }

    if (mPrintWidth < mPrintHeight)
    {
        retVal = false;

        QMessageBox msgBox;
        msgBox.setStandardButtons(QMessageBox::Abort);
        msgBox.setText ("Fehler!");
        msgBox.setInformativeText("Die Druckbreite darf nicht kleiner als die Druckhöhe sein.");
        msgBox.exec();
    }
    return retVal;
}

void CodeGeneratorView::generatePDF ()
{
    QImage codeImg;
    unsigned int lockerId = 1; //hardcoded right now - todo: should be a user defined value, stored in db
    QList<QVariant> freeKeyCodes = ioInterface()->getFreeKeycodes(lockerId, mNumberOfCodes.toInt());
    PrinterInterface *pdfPrinter = new PrinterInterfacePdf ();

    if (pdfPrinter->saveAsFile())
    {
        pdfPrinter->begin();

        QString lockerCode = QString::number(lockerId);

        //add leading zeros to locker code
        while (lockerCode.length() < 4)
            lockerCode.prepend("0");

        QString qrCode ("");

        Qt::PenStyle borderPen = Qt::NoPen;
        if (mBorderCheckState)
            borderPen = Qt::DotLine;

        for (int i = 0; i < freeKeyCodes.size(); i++)
        {
            qrCode = QString::number(freeKeyCodes.at(i).toUInt());

            // insert leading 0 so we have 4 digits for the code
            while (qrCode.length() < 4)
                qrCode.prepend("0");

            qrCode.prepend("-");
            qrCode.prepend(lockerCode);

            // generate qr code image
            codeImg = decoder.encodeData (qrCode, QZXing::EncoderFormat_QR_CODE, QSize (300,300), QZXing::EncodeErrorCorrectionLevel_M, true, false);

            // print qr code image to pdf
            pdfPrinter->drawQRCode(codeImg, mPrintHeight.toInt(), mPrintWidth.toInt(), qrCode, mFoldableCheckState, borderPen);
        }

        pdfPrinter->finish();

        // open pdf after creation?
        if (mShowPdfFile)
        {
            //open file with the default app
            QDesktopServices::openUrl(QUrl(pdfPrinter->getFilePath()));
        }
    }

    delete pdfPrinter;
}


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

    setHeader("Codegenerator");

    QLabel *numberOfCodes = new QLabel ("Anzahl gewünschter Codes:", this);
    QLineEdit *numberOfCodesEdit = new QLineEdit(this);
    numberOfCodesEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d\\d"), numberOfCodesEdit));
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

    QGridLayout *viewLayout = new QGridLayout();
    viewLayout->addWidget(numberOfCodes, 0, 0);
    viewLayout->addWidget(numberOfCodesEdit, 0, 1);

    viewLayout->addWidget(printHeight, 1, 0);
    viewLayout->addWidget(printHeightEdit, 1, 1);

    viewLayout->addWidget(printWidth, 2, 0);
    viewLayout->addWidget(printWidthEdit, 2, 1);

    viewLayout->addWidget(borders, 3, 0);
    viewLayout->addWidget(foldable, 4, 0);

    layout()->addItem(viewLayout);

    // QString codeString = "00001-12345";
    // QImage testImg = decoder.encodeData (codeString, QZXing::EncoderFormat_QR_CODE, QSize (240,240), QZXing::EncodeErrorCorrectionLevel_M, true, false);
    // QLabel *code = new QLabel (this);
    // code->setText(codeString);
    // testCode->setPixmap(QPixmap::fromImage(testImg));

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout()->addItem(spacer);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Pdf);
    setMenuButtons(menuButtons);

    connect (numberOfCodesEdit, SIGNAL(textChanged(QString)), this, SLOT(onNumberOfCodesChanged(QString)));
    connect (printHeightEdit, SIGNAL(textChanged(QString)), this, SLOT(onprintHeightChanged(QString)));
    connect (printWidthEdit, SIGNAL(textChanged(QString)), this, SLOT(onprintWidthChanged(QString)));
    connect (borders, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(onBorderCheckStateChanged(Qt::CheckState)));
    connect (foldable, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(onFoldableCheckStateChanged(Qt::CheckState)));
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
    qDebug () << "CodeGeneratorView::onNumberOfCodesChanged";

    update ();

    mNumberOfCodes = text;
}

void CodeGeneratorView::onprintHeightChanged(QString value)
{
    update ();
    qDebug ()<< "onprintHeightChanged";
    mPrintHeight = value;
}

void CodeGeneratorView::onprintWidthChanged(QString value)
{
    update ();
    qDebug ()<< "onprintWidthChanged";
    mPrintWidth = value;
}

void CodeGeneratorView::onBorderCheckStateChanged(Qt::CheckState value)
{
    update ();
    qDebug ()<< "onBorderCheckStateChanged";
    mBorderCheckState = value;
}

void CodeGeneratorView::onFoldableCheckStateChanged(Qt::CheckState value)
{
    update ();
    qDebug ()<< "onFoldableCheckStateChanged";
    mFoldableCheckState = value;
}

bool CodeGeneratorView::checkValues()
{
    if (mNumberOfCodes != "" && mNumberOfCodes != "0" && mPrintHeight != "" && mPrintWidth != "")
    {
        return true;
    }
    else
        QMessageBox::information(this, "Fehler", "Bitte Eingaben prüfen.");
    return false;
}

void CodeGeneratorView::generatePDF ()
{
    QImage codeImg;
    unsigned int lockerId = 1;
    unsigned int freeCode = ioInterface()->getFreeKeycode(lockerId);
    PrinterInterface *pdfPrinter = new PrinterInterfacePdf ();   

    pdfPrinter->saveAsFile();
    pdfPrinter->begin();

    QString lockerCode = QString::number(lockerId);

    //add leading zeros to locker code
    while (lockerCode.length() < 4)
        lockerCode.prepend("0");

    QString qrCode ("");

    Qt::PenStyle borderPen = Qt::NoPen;
    if (mBorderCheckState)
        borderPen = Qt::DotLine;

    for (int i = 0; i < mNumberOfCodes.toInt(); i++)
    {
        qrCode = QString::number(freeCode);

        // insert leading 0 so we have 4 digits for the code
        while (qrCode.length() < 4)
            qrCode.prepend("0");

        qrCode.prepend("-");
        qrCode.prepend(lockerCode);

        qDebug () << "CodeGeneratorView::generatePDF (): code generated: " << qrCode;

        codeImg = decoder.encodeData (qrCode, QZXing::EncoderFormat_QR_CODE, QSize (300,300), QZXing::EncodeErrorCorrectionLevel_M, true, false);

        pdfPrinter->drawKeycode(codeImg, mPrintHeight.toInt(), mPrintWidth.toInt(), mFoldableCheckState, borderPen);

        freeCode++;
    }
    pdfPrinter->finish();

    delete pdfPrinter;
}


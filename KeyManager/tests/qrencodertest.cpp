#include "qrencodertest.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "QZXing.h"
#include <QPixmap>

QREncoderTest::QREncoderTest(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QLabel *encodeStringLabel = new QLabel ("Text to encode: TEST");

    encodedImg = new QLabel();
    encode ("TEST");

    layout->addWidget(encodeStringLabel);
    layout->addWidget(encodedImg);
}

void QREncoderTest::encode(QString string)
{
    QImage barcode = QZXing::encodeData(string);
    encodedImg->setPixmap(QPixmap::fromImage(barcode));
    encodedImg->adjustSize();
}

#ifndef QRENCODERTEST_H
#define QRENCODERTEST_H

#include <QWidget>

class QLabel;
class QString;

class QREncoderTest : public QWidget
{
    Q_OBJECT
public:
    explicit QREncoderTest(QWidget *parent = nullptr);
private:
    void encode (QString string);
    QLabel *encodedImg;

signals:
};

#endif // QRENCODERTEST_H

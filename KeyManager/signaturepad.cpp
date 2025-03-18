#include "signaturepad.h"
#include <QPainter>
#include <QPaintEvent>
#include <QLabel>
#include <QtPrintSupport/qtprintsupportglobal.h>
#include <QMouseEvent>
#include <QPainter>

SignaturePad::SignaturePad(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_StaticContents);
    setFixedSize (300, 150);
}

void SignaturePad::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void SignaturePad::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void SignaturePad::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = false;
    update();
}

void SignaturePad::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->position().toPoint();
        scribbling = true;
    }
}

void SignaturePad::mouseMoveEvent(QMouseEvent *event)
{
    qDebug () << "SignaturePad::mouseMoveEvent(QMouseEvent *event)";

    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->position().toPoint());
}

void SignaturePad::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->position().toPoint());
        scribbling = false;
    }
}

void SignaturePad::paintEvent(QPaintEvent *event)
{
    qDebug () << "SignaturePad::paintEvent(QPaintEvent *event)";

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);

    emit signaturePaint ();
}

void SignaturePad::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void SignaturePad::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
               .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void SignaturePad::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

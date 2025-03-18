#ifndef SIGNATUREPAD_H
#define SIGNATUREPAD_H

#include <QWidget>
#include <QPixmap>
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class SignaturePad : public QWidget
{
    Q_OBJECT
public:

    SignaturePad(QWidget *parent = nullptr);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

public slots:
    void clearImage();

signals:
    void signaturePaint ();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    bool modified = false;
    bool scribbling = false;
    int myPenWidth = 2;
    QColor myPenColor = Qt::blue;
    QImage image;
    QPoint lastPoint;
};

#endif // SIGNATUREPAD_H

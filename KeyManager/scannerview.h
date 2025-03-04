#ifndef SCANNERVIEW_H
#define SCANNERVIEW_H

#include <QWidget>
#include <QMediaDevices>
#include <QScopedPointer>
#include <QImageCapture>
#include <QMediaRecorder>
#include <QMediaCaptureSession>
#include <QCamera>
#include <QCameraDevice>
#include <QImage>
#include <QSize>

class QVideoWidget;

class ScannerView : public QWidget
{
    Q_OBJECT
public:
    explicit ScannerView(QWidget *parent = nullptr);
    QVideoWidget* getViewfinder ();
    QSize getViewfinderSize ();
    ~ScannerView();

private:

    QVideoWidget *m_viewfinder;

signals:
    void abortScanner ();

private slots:
    void onAbortBtnClicked ();
};

#endif // SCANNERVIEW_H

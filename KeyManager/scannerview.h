#ifndef SCANNERVIEW_H
#define SCANNERVIEW_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "winsubmenu.h"
#include "QZXing.h"

class QVideoWidget;
class QLabel;
class QPushButton;
class Camera;
class ViewDataScanner;
class QButtonGroup;
class QFormLayout;
class CheckBoxArray;
class QZXing;

class ScannerView : public WinSubmenu
{
    Q_OBJECT
    public:

        enum ScannerState {
            READY,
            SCANNING,
            SCANSUCCEEDED,
        };

        explicit ScannerView(QWidget *parent = nullptr);
        void reset();
        virtual void showEvent(QShowEvent *);
        virtual void hideEvent(QHideEvent *);
        void setScannerState (ScannerState aStatus);
        QVideoWidget* getViewfinder ();
        QSize getViewfinderSize ();
        void setCustomerLabel (QString aCustomerId);
        void setKeyLabel (QString aKeyId);
        const QString getCustomerLabel ();
        const QString getKeyLabel();
        ~ScannerView();

    signals:
        void keycodeRecognised (int keycode);

    private slots:
        virtual void onMenuBtnClicked (Gui::MenuButton btnType);
        void onCameraChanged();
        void decodeFromVideoFrame ();

    private:
        void startScanner();
        void stopScanner();
        bool codeIsValid(const unsigned int code);
        void playSound();
        void setAvailableCams();
        void keyReleaseEvent(QKeyEvent *event);
        void updateDefaultCamSetting();

        QFormLayout *mCamSettingsLayout;
        CheckBoxArray *mAvailableCameras;
        QList <int> mCameraIds;
        Camera *mCameraInstance;
        QTimer *mGrabTimer;
        ScannerState mScannerState;
        QVideoWidget *m_viewfinder;
        QLabel *mCodeLabel;
        QLabel *mGroupLabel;
        QLabel *mKeyLabel;

        bool mCameraInitDone;

        ViewDataScanner *mScannerData;
        QMediaPlayer mPlayer;
        QAudioOutput mAudioOut;

        QZXing *mDecoder;
};

#endif // SCANNERVIEW_H

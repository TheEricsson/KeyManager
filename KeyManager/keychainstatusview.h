#ifndef KEYCHAINSTATUSVIEW_H
#define KEYCHAINSTATUSVIEW_H

#include <QObject>
#include <QWidget>
#include "winsubmenu.h"
#include "viewdatakeychain.h"

class QLabel;
class QSqlQueryModel;
class QTableView;
class QSqlRelationalTableModel;
class QPushButton;
class KeychainDataHandover;
class QSortFilterProxyModel;
class CameraView;
class ImageView;

class KeychainStatusView : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit KeychainStatusView(QWidget *parent = nullptr);
        ~KeychainStatusView ();
        void setNextBtnText ();
        bool setKeychainModel (QSqlRelationalTableModel* model);
        bool setKeysModel (QSqlRelationalTableModel* model);
        bool setKeychainHistoryModel (QSqlRelationalTableModel* model);
        void setKeychainImage (const QImage& img);
        void setViewMode (ViewMode::Value mode);

    private slots:
        void keyCodeBtnClicked();
        void keyImgBtnClicked();
        void onCameraViewButtonClicked(Gui::MenuButton btn);
        void onImageViewButtonClicked(Gui::MenuButton btn);

    private:
        virtual void showEvent(QShowEvent *);
        void showMessageWindow(const QString& infoText);

        QSqlRelationalTableModel *mKeyModel;
        //QSqlRelationalTableModel *mKeychainModel;
        QSqlRelationalTableModel *mHistoryModel;
        //QSortFilterProxyModel *mFilteredKeychainModel;
        QSortFilterProxyModel *mFilteredKeyModel;
        QSortFilterProxyModel *mFilteredHistoryModel;

        //QSqlQueryModel *mKeyOverview;
        QTableView *mKeys;
        //QTableView *mKeychain;
        QTableView *mHistory;
        QLabel *mKeychainKeycode;
        QLabel *mKeychainStatus;
        QLabel *mKeychainInternalLocation;
        QLabel *mKeychainStreet;
        QLabel *mKeychainStreetNumber;
        QLabel *mKeychainCity;

        QImage *mKeychainImage;
        QPushButton *mKeysImgPreview;
        ViewDataKeychain *mViewData;
        CameraView *mCameraView;
        ImageView *mImageView;

        ViewMode::Value mViewMode;
};

#endif // KEYCHAINSTATUSVIEW_H

#ifndef KEYCHAINSTATUSVIEW_H
#define KEYCHAINSTATUSVIEW_H

#include <QObject>
#include <QWidget>

class QLabel;
class QSqlQueryModel;
class QTableView;
class QSqlRelationalTableModel;
class QPushButton;

class KeychainStatusView : public QWidget
{
    Q_OBJECT
public:
    explicit KeychainStatusView(QWidget *parent = nullptr);
    void setKeychainStatus (const int &statusId);
    bool setKeychainModel (QSqlRelationalTableModel* model);
    bool setKeysModel (QSqlRelationalTableModel* model);
    void setKeychainImagePath (const QString& imgPath);

signals:
    void previousButtonClicked ();
    void nextButtonClicked ();

private slots:
    void onPreviousBtnClicked();
    void onNextBtnClicked();
private:

    QSqlQueryModel *mKeyOverview;
    QTableView *mKeys;
    QTableView *mKeychain;
    QLabel *mIdBarcode;
    QLabel *mStreet;
    QLabel *mStreetNr;
    QLabel *mAreaCode;
    QLabel *mCity;
    QLabel *mQuantityKeys;
    QImage *mKeychainImage;
    QPushButton *mButtonNext;
    QPushButton *mKeysImgPreview;
};

#endif // KEYCHAINSTATUSVIEW_H

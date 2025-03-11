#ifndef KEYCHAINSTATUSVIEW_H
#define KEYCHAINSTATUSVIEW_H

#include <QObject>
#include <QWidget>

class QLabel;
class QSqlQueryModel;
class QTableView;

class KeychainStatusView : public QWidget
{
    Q_OBJECT
public:
    explicit KeychainStatusView(QWidget *parent = nullptr);
    explicit KeychainStatusView(int aLocaleId, int aKeychainId, QWidget *parent = nullptr);
    void setModel (QSqlQueryModel* model);

signals:
    void previousButtonClicked ();
private slots:
    void onPreviousBtnClicked();
private:
    void init ();

    QSqlQueryModel *mKeyOverview;
    QTableView *mKeys;
    QLabel *mIdLocale;
    QLabel *mIdBarcode;
    QLabel *mStreet;
    QLabel *mStreetNr;
    QLabel *mAreaCode;
    QLabel *mCity;
    QLabel *mQuantityKeys;
    QImage *mKeychainImage;
};

#endif // KEYCHAINSTATUSVIEW_H

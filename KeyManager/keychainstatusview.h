#ifndef KEYCHAINSTATUSVIEW_H
#define KEYCHAINSTATUSVIEW_H

#include <QObject>
#include <QWidget>

class QSqlRelationalTableModel;

class KeychainStatusView : public QWidget
{
    Q_OBJECT
public:
    explicit KeychainStatusView(QWidget *parent = nullptr);
    explicit KeychainStatusView(int aLocaleId, int aKeychainId, QWidget *parent = nullptr);

signals:
    void previousButtonClicked ();
private slots:
    void onPreviousBtnClicked();
private:
    QSqlRelationalTableModel *mKeyStatus;
    void init ();
};

#endif // KEYCHAINSTATUSVIEW_H

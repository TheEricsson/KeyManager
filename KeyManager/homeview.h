#ifndef HOMEVIEW_H
#define HOMEVIEW_H

#include <QWidget>

class QPushButton;
class QGridLayout;

class HomeView : public QWidget
{
    Q_OBJECT
public:
    explicit HomeView(QWidget *parent = nullptr);
private:
    QPushButton* mBtnScan;
    QPushButton* mBtnSearch;
    QPushButton* mBtnManage;
    QPushButton* mBtnExit;
    QGridLayout* mLayout;

private slots:
    void onScanButtonReleased();

signals:
    void showScannerView ();
};

#endif // HOMEVIEW_H

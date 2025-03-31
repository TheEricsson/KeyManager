#ifndef HOMEVIEW_H
#define HOMEVIEW_H

#include <QWidget>
#include "winsubmenu.h"

class QPushButton;
class QGridLayout;

class HomeView : public WinSubmenu
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
    void onSearchButtonReleased();

signals:
    void showScannerView ();
    void showTableView ();
};

#endif // HOMEVIEW_H

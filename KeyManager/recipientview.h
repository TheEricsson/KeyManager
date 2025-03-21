#ifndef RECIPIENTVIEW_H
#define RECIPIENTVIEW_H

#include <QObject>
#include <QWidget>
#include <QString>
#include "globals.h"

class QTableView;
class QSqlRelationalTableModel;
class QSortFilterProxyModel;
class QItemSelection;
class QPushButton;
class QLineEdit;

class RecipientView : public QWidget
{
    Q_OBJECT
public:
    explicit RecipientView(QWidget *parent = nullptr);
    bool setModel (QSqlRelationalTableModel* model);
    bool getRecipientData (RecipientData &data);
    void reset();

public slots:
    void setTableFilter(const QString &);

signals:
    void nextButtonClicked();
    void previousButtonClicked();
    void newRecipientButtonClicked();

private slots:

    void onNextBtnClicked ();
    void onPreviousBtnClicked ();
    void onNewRecipientBtnClicked ();
    void onTableSelectionChanged (const QItemSelection &itemNew, const QItemSelection &itemOld);

private:
    QTableView *mRecipients;
    QSortFilterProxyModel *mFilteredModel;
    QPushButton *mBtnNext;
    bool mRowSelected;
};

#endif // RECIPIENTVIEW_H

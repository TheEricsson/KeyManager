#ifndef RECIPIENTVIEW_H
#define RECIPIENTVIEW_H

#include <QObject>
#include <QWidget>
#include <QString>

class QTableView;
class QSqlRelationalTableModel;
class QSortFilterProxyModel;

class RecipientView : public QWidget
{
    Q_OBJECT
public:
    explicit RecipientView(QWidget *parent = nullptr);
    bool setModel (QSqlRelationalTableModel* model);

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

private:
    QTableView *mRecipients;
    QSortFilterProxyModel *mFilteredModel;
};

#endif // RECIPIENTVIEW_H

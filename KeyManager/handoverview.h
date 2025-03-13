#ifndef HANDOVERVIEW_H
#define HANDOVERVIEW_H

#include <QObject>
#include <QWidget>

class HandoverView : public QWidget
{
    Q_OBJECT
public:
    explicit HandoverView(QWidget *parent = nullptr);

signals:
};

#endif // HANDOVERVIEW_H

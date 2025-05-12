#ifndef ANDROIDAPPLICATION_H
#define ANDROIDAPPLICATION_H

#include <QApplication>
#include <QWidget>
#include <QWindow>

/* this code avoids a QT for android bug (description and solution as seen on 'forum.qt.io/topic/162096'):
 * Sometimes, the screen on android is not updated properly.
 * This leads to glitches, especially on QWidgets of type QRadioButton, as well as QTreeView and QTableView
 * The notify event leads to an screen update, as soon as there is an update request caused by a QWidget*/

class AndroidApplication : public QApplication
{
public:
    AndroidApplication(int &argc, char **argv)
        : QApplication(argc, argv)
    {}

    bool notify(QObject *receiver, QEvent *e) override
    {
        // Erstmal normal weiterleiten
        bool ok = QApplication::notify(receiver, e);

        // Bei jedem UpdateRequest-Event erzwingen wir einen Frame-Swap
        if (e->type() == QEvent::UpdateRequest) {
            // Prüfen, ob das Ziel ein QWidget ist
            if (auto w = qobject_cast<QWidget*>(receiver)) {
                if (auto wh = w->window()->windowHandle()) {
                    wh->requestUpdate();  // sofortigen Flush anstoßen
                }
            }
        }
        return ok;
    }
};


#endif // ANDROIDAPPLICATION_H

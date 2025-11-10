#ifndef DBSETTINGSDIALOG_H
#define DBSETTINGSDIALOG_H

#include "winsubmenu.h"

class DbSettingsDialog : public WinSubmenu
{
    Q_OBJECT
    public:
        explicit DbSettingsDialog(QWidget* parent = 0);
    private slots:
        void onBtnWorkingDirectoryPressed();
        void onBtnBackupDirectory();
        void onBtnImportDbPressed();
    /*signals:
        const QString& workingDirectoryChanged();
        const QString& backupDirectoryChanged();
        const QString& importDb();*/
};

#endif // DBSETTINGSDIALOG_H

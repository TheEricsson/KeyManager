#ifndef SETTINGSVIEWDB_H
#define SETTINGSVIEWDB_H
#include "winsubmenu.h"

class SettingsViewDb : public WinSubmenu
{
    Q_OBJECT

    public:
        explicit SettingsViewDb(QWidget *parent = nullptr);

    private:

        enum DbOption
        {
            dbBackup,
            dbRestore
        };

        void showSuccessWindow (SettingsViewDb::DbOption messageType, bool noError);

    private slots:
        void onSaveDbBtnClicked();
        void onReadDbBtnClicked();
};

#endif // SETTINGSVIEWDB_H

#include "dbsettingsdialog.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>

DbSettingsDialog::DbSettingsDialog(QWidget *parent) : WinSubmenu {parent}
{
    setHeader("Datenbankeinstellungen");

    QGridLayout* centralLayout = new QGridLayout();

    QPushButton *btnWorkingDirectory = new QPushButton ("Arbeitsverzeichnis ändern", this);
    QLabel *labelWorkingDirectory = new QLabel("...", this);
    QPushButton *btnBackupDirectory = new QPushButton ("Backupverzeichnis ändern", this);
    QLabel *labelBackupDirectory = new QLabel("...", this);
    QPushButton *btnImportDb = new QPushButton ("Datenbank importieren");
    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    centralLayout->addWidget(btnWorkingDirectory, 0, 0);
    centralLayout->addWidget(labelWorkingDirectory, 0, 1);
    centralLayout->addWidget(btnBackupDirectory, 1, 0);
    centralLayout->addWidget(labelBackupDirectory, 1, 1);
    centralLayout->addWidget(btnImportDb, 2, 0);
    centralLayout->addItem(spacer, 3, 0, 1, 2);

    setCentralLayout(centralLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Ok);
    setMenuButtons(menuButtons);
    disableButton(0, true);

    connect (btnWorkingDirectory, SIGNAL (pressed()), this, SLOT (onBtnWorkingDirectoryPressed()));
    connect (btnBackupDirectory, SIGNAL (pressed()), this, SLOT (onBtnBackupDirectory()));
    connect (btnImportDb, SIGNAL (pressed()), this, SLOT (onBtnImportDbPressed()));
}

void DbSettingsDialog::onBtnWorkingDirectoryPressed()
{
    //todo: check current directory and set if exist

    //otherwise, default to:
    QUrl saveLocation = QFileDialog::getExistingDirectory(nullptr, "Arbeitsverzeichnis setzen...", QString::fromLatin1(qgetenv("OneDrive")));
    qDebug () << "saveLocation: " << saveLocation;
}

void DbSettingsDialog::onBtnBackupDirectory()
{
    //todo: check current directory and set if exist

    //otherwise, default to:
    QUrl saveLocation = QFileDialog::getExistingDirectory(nullptr, "Backupverzeichnis setzen...", QString::fromLatin1(qgetenv("OneDrive")));
    qDebug () << "saveLocation: " << saveLocation;
}

void DbSettingsDialog::onBtnImportDbPressed()
{
    // default import location:
    QUrl saveLocation = QFileDialog::getSaveFileName(nullptr,QString(),QString::fromLatin1(qgetenv("OneDrive")),"Backup (*.sqlite)",nullptr,QFileDialog::Options());
    qDebug () << "saveLocation: " << saveLocation;
}

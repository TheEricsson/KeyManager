#include "settingsviewdb.h"
#include "winsubmenu.h"
#include "iointerface.h"
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

SettingsViewDb::SettingsViewDb(QWidget *parent) : WinSubmenu {parent}
{
    QGridLayout *gridLayout = new QGridLayout();

    setHeader("Datenbank-Einstellungen");

    // not supported right now. Android file access permission failure...:
    // "...requires that you obtain access using ACTION_OPEN_DOCUMENT or related APIs"

    QPushButton *saveDbBtn = new QPushButton("Datenbank-Backup erstellen", this);
    //saveDbBtn->setDisabled(true);
    saveDbBtn->setMinimumSize(QSize(Gui::buttonHeight,Gui::buttonWidth));

    QPushButton *readDbBtn = new QPushButton("Datenbank-Backup\neinlesen", this);
    readDbBtn->setMinimumSize(QSize(Gui::buttonHeight,Gui::buttonWidth));

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    gridLayout->addWidget(saveDbBtn, 0, 0);
    gridLayout->addWidget(readDbBtn, 1, 0);
    gridLayout->addItem(spacer, 2, 0);

    setCentralLayout(gridLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    menuButtons.append(Gui::Next);
    setMenuButtons(menuButtons);

    connect (saveDbBtn, SIGNAL(clicked()), this, SLOT(onSaveDbBtnClicked()));
    connect (readDbBtn, SIGNAL(clicked()), this, SLOT(onReadDbBtnClicked()));
}

void SettingsViewDb::showSuccessWindow(SettingsViewDb::DbOption messageType, bool noError)
{
    QMessageBox msgBox;
    QString headerText("");
    QString infoText("");

    if (noError)
        msgBox.setStandardButtons(QMessageBox::Ok);
    else
        msgBox.setStandardButtons(QMessageBox::Abort);

    switch (messageType)
    {
        case dbBackup:
            headerText = "Datenbank-Backup";
            if (noError)
            {
                headerText = "Datenbank-Backup erfolgreich abgeschlossen.";
            }
            else
            {
                headerText = "Datenbank-Backup fehlgeschlagen!";
            }
            break;
        case dbRestore:
            headerText = "Datenbank-Wiederherstellung";
            if (noError)
            {
                headerText = "Datenbank-Wiedeherstellung erfolgreich abgeschlossen.";
            }
            else
            {
                headerText = "Datenbank-Wiederherstellung fehlgeschlagen!";
            }
            break;
        default:
            break;
    }

    msgBox.setText(headerText);
    msgBox.setInformativeText(infoText);
    msgBox.exec();
}

void SettingsViewDb::onSaveDbBtnClicked()
{
    // open file dialog and let user set location + file name
    QString dbLocation = ioInterface()->getDatabaseLocation();

    // open file dialog and let user set location + file name
    QString saveLocation = QFileDialog::getSaveFileName((QWidget* )0, "Datenbank exportieren", QString(), "*.sqlite");

    bool ok = false;

    //check: file name set in dialog? + save location is not the current db location?
    if (saveLocation != "" && saveLocation != dbLocation)
        ok = true;

    if (ok)
    {
        if (QFileInfo(saveLocation).suffix().isEmpty())
        {
            saveLocation.append(".sqlite");
        }

        if (QFile::exists(saveLocation))
        {
            QFile::remove(saveLocation);
        }
        ok = QFile::copy(dbLocation, saveLocation);
    }

    qDebug() << "SettingsViewDb::onSaveDbBtnClicked() - dbLocation: " << dbLocation;
    qDebug() << "SettingsViewDb::onSaveDbBtnClicked() - saveLocation: " << saveLocation;

    showSuccessWindow (SettingsViewDb::dbBackup, ok);
}

void SettingsViewDb::onReadDbBtnClicked()
{
    QString dbLocation = ioInterface()->getDatabaseLocation();

    QFileDialog dlg;

    // open file dialog and let user set location + file name
    QString dbNew = dlg.getOpenFileName((QWidget* )0, "Datenbank importieren", QString(), "*.sqlite*");

    //QString dbNew = QFileDialog::getOpenFileName((QWidget* )0, "Datenbank importieren", QString(), "*.sqlite*");

    qDebug() << "dbLocation: " << dbLocation;
    qDebug() << "dbNew: " << dbNew;

    // close database, otherwise the file cannot be replaced
    ioInterface()->closeDatabase();

    bool ok = false;

    if (QFile::exists(dbLocation))
    {
        QString dbReplaced = dbLocation + "_replaced.sqlite";
        QFile::copy(dbLocation, dbReplaced);
    }

    QFile::remove(dbLocation);
    ok = QFile::copy(dbNew, dbLocation);

    // open database
    ioInterface()->openDatabase();

    showSuccessWindow (SettingsViewDb::dbRestore, ok);
}

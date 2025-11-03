#include "settingsviewdb.h"
#include "winsubmenu.h"
#include "iointerface.h"
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QSaveFile>
#include <QStandardPaths>
#include <QCheckBox>

SettingsViewDb::SettingsViewDb(QWidget *parent) : WinSubmenu {parent}
{
    QGridLayout *gridLayout = new QGridLayout();

    setHeader("App-Einstellungen");

    //general settings

    //database stuff

    QPushButton *saveDbBtn = new QPushButton("Datenbank-Backup erstellen", this);
    //saveDbBtn->setDisabled(true);
    saveDbBtn->setMinimumSize(QSize(Gui::buttonHeight,Gui::buttonWidth));

    QPushButton *readDbBtn = new QPushButton("Datenbank-Backup\neinlesen", this);
    readDbBtn->setMinimumSize(QSize(Gui::buttonHeight,Gui::buttonWidth));

    QCheckBox *autoBackup = new QCheckBox("Automatisches Backup bei Beenden", this);
    mAutoBackupLocation = new QPushButton("Speicherort...", this);
    mAutoBackupLocation->setMinimumSize(QSize(Gui::buttonHeight,Gui::buttonWidth));
    mAutoBackupLocation->setDisabled(true);

    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    gridLayout->addWidget(saveDbBtn, 0, 0, 1, 2);
    gridLayout->addWidget(readDbBtn, 1, 0, 1, 2);
    gridLayout->addWidget(autoBackup, 2, 0, 1, 1);
    gridLayout->addWidget(mAutoBackupLocation, 2, 1, 1, 1);
    gridLayout->addItem(spacer, 3, 0, 1, 2);

    setCentralLayout(gridLayout);

    QList<Gui::MenuButton> menuButtons;
    menuButtons.append(Gui::Back);
    setMenuButtons(menuButtons);

    connect (saveDbBtn, SIGNAL(clicked()), this, SLOT(onSaveDbBtnClicked()));
    connect (readDbBtn, SIGNAL(clicked()), this, SLOT(onReadDbBtnClicked()));
    connect (autoBackup, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(onAutoBackupCheckStateChanged(Qt::CheckState)));
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
    bool success = false;

    // open file dialog and let user set location + file name
    QString dbLocation = ioInterface()->getDatabaseLocation();

#ifdef Q_OS_WIN64
    QUrl saveLocation = QFileDialog::getSaveFileName(nullptr,QString(),QStandardPaths::writableLocation(QStandardPaths::AppDataLocation),"Backup (*.sqlite)",nullptr,QFileDialog::Options());
#elif Q_OS_ANDROID
    QUrl saveLocation = QFileDialog::getSaveFileUrl(nullptr,QString(),QUrl(),"Backup (*.sqlite)",nullptr,QFileDialog::Options(),QStringList());
#endif

    QString filename = saveLocation.toDisplayString();

    //new file to save backup to
    QFile dbSaveFile(filename);
    //database file to read from
    QFile dbFile (dbLocation);

    if(!dbSaveFile.open(QIODevice::WriteOnly | QFile::Truncate))
    {
        qDebug() << dbSaveFile.errorString();
        success = false;
    }
    else
    {
        if (dbFile.open(QFile::ReadOnly))
        {
            QByteArray fileContents = dbFile.readAll();

            //write db data to new file
            dbSaveFile.write(fileContents);
            success = true;
        }
        else
        {
            qDebug() << dbFile.errorString();
            success = false;
        }
    }

    // close file handles
    if (dbSaveFile.isOpen())
        dbSaveFile.close();

    if (dbFile.isOpen())
        dbFile.close();

    showSuccessWindow (SettingsViewDb::dbBackup, success);
}

void SettingsViewDb::onReadDbBtnClicked()
{
    QString dbLocation = ioInterface()->getDatabaseLocation();

    QFileDialog dlg;

    // open file dialog and let user set location + file name
    QString dbNew = dlg.getOpenFileName((QWidget* )0, "Datenbank importieren", QString(), "*.sqlite*");

    if (!dbNew.isNull()) // check if the dialog was canceled
    {
        qDebug() << "dbLocation: " << dbLocation;
        qDebug() << "dbNew: " << dbNew;

        if ("" != dbNew)
        {
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
    }
}

void SettingsViewDb::onAutoBackupCheckStateChanged(Qt::CheckState aCheckstate)
{
    if (mAutoBackupLocation)
    {
        switch (aCheckstate)
        {
            case Qt::Checked:
                mAutoBackupLocation->setEnabled(true);
                break;
            case Qt::Unchecked:
                mAutoBackupLocation->setDisabled(true);
                break;
            default:
                break;

        }
    }
}

#include "keyscannedview.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

KeyScannedView::KeyScannedView(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *verticalLayout = new QVBoxLayout ();

    QLabel *keyScanned = new QLabel ("Code erkannt!");
    QLabel *mandant = new QLabel ("Mandant:");
    QLabel *keyId = new QLabel ("Schlüssel-ID:");
    QLabel *property = new QLabel ("Liegenschaft");
    QLabel *Typ = new QLabel ("Technik/Wohnung/...");
    QLabel *keyInternalId = new QLabel ("Id Schlüsselschrank");

    QPushButton *btnBack = new QPushButton ("Abbruch");
    QPushButton *btnReturn = new QPushButton ("Schlüssel annehmen");
    QPushButton *btnOut = new QPushButton ("Schlüssel ausgeben");
    QPushButton *btnUnknown = new QPushButton ("Schlüssel anlegen");

    QHBoxLayout *horizontalLayout = new QHBoxLayout ();
    horizontalLayout->addWidget(btnOut);
    horizontalLayout->addWidget(btnReturn);
    horizontalLayout->addWidget(btnUnknown);
    horizontalLayout->addWidget(btnBack);

    verticalLayout->addWidget(keyScanned);
    verticalLayout->addWidget(mandant);
    verticalLayout->addWidget(keyId);
    verticalLayout->addWidget(property);
    verticalLayout->addWidget(Typ);
    verticalLayout->addWidget(keyInternalId);

    verticalLayout->addLayout(horizontalLayout);

    this->setLayout(verticalLayout);
}

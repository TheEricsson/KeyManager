#include "keyscannedview.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

KeyScannedView::KeyScannedView(QWidget *parent)
    : QWidget{parent}
{
    mLayout = new QGridLayout ();
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

    mLayout->addWidget(keyScanned);
    mLayout->addWidget(mandant);
    mLayout->addWidget(keyId);
    mLayout->addWidget(property);
    mLayout->addWidget(Typ);
    mLayout->addWidget(keyInternalId);

    mLayout->addWidget(btnBack);
    mLayout->addWidget(btnReturn);
    mLayout->addWidget(btnOut);
    mLayout->addWidget(btnUnknown);

    this->setLayout(mLayout);
}

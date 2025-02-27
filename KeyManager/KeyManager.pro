QT       += core gui multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../QZXing/QZXing.pri)

SOURCES += \
    camera.cpp \
    homeview.cpp \
    keyscannedview.cpp \
    main.cpp \
    mainwindow.cpp \
    scannerview.cpp \
    tests/qrencodertest.cpp

HEADERS += \
    camera.h \
    homeview.h \
    keyscannedview.h \
    mainwindow.h \
    scannerview.h \
    tests/qrencodertest.h

TRANSLATIONS += \
    KeyManager_de_DE.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# DEFINES += ENCODERTEST

RESOURCES += \
    res/resources.qrc

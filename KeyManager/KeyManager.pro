QT       += core gui multimediawidgets sql core-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../QZXing/QZXing.pri)

SOURCES += \
    camera.cpp \
    databaseimpl.cpp \
    homeview.cpp \
    keychainstatusview.cpp \
    main.cpp \
    mainwindow.cpp \
    scannerview.cpp \
    tableview.cpp \
    tests/qrencodertest.cpp

HEADERS += \
    camera.h \
    databaseimpl.h \
    globals.h \
    homeview.h \
    initdb.h \
    keychainstatusview.h \
    mainwindow.h \
    scannerview.h \
    tableview.h \
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
# DEFINES += NOCAM_ENCODE

RESOURCES += \
    res/resources.qrc

# DISTFILES += \
#     android/AndroidManifest.xml \
#     android/build.gradle \
#     android/res/values/libs.xml \
#     android/res/xml/qtprovider_paths.xml \
#     dbexample/db.sqlite

# contains(ANDROID_TARGET_ARCH,arm64-v8a) {
#     ANDROID_PACKAGE_SOURCE_DIR = \
#         $$PWD/../KeyManager.pro
#}

QT       += core gui multimediawidgets sql core-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../QZXing/QZXing.pri)

SOURCES += \
    addrecipientview.cpp \
    annotationview.cpp \
    calendarview.cpp \
    camera.cpp \
    databaseimpl.cpp \
    dataobject.cpp \
    dataobjecthandover.cpp \
    handoutsummaryview.cpp \
    handoverview.cpp \
    homeview.cpp \
    keychainstatusview.cpp \
    main.cpp \
    mainwindow.cpp \
    menubutton.cpp \
    recipientview.cpp \
    returndateview.cpp \
    scannerview.cpp \
    signaturepad.cpp \
    tableview.cpp \
    tests/qrencodertest.cpp \
    viewdata.cpp \
    viewdatahandover.cpp \
    viewdatakeychainstatus.cpp \
    viewdatarecipient.cpp \
    viewdatareturndate.cpp \
    viewdatascanner.cpp \
    viewstack.cpp \
    viewstackmanager.cpp \
    winsubmenu.cpp

HEADERS += \
    addrecipientview.h \
    annotationview.h \
    calendarview.h \
    camera.h \
    databaseimpl.h \
    dataobject.h \
    dataobjecthandover.h \
    globals.h \
    handoutsummaryview.h \
    handoverview.h \
    homeview.h \
    initdb.h \
    keychainstatusview.h \
    mainwindow.h \
    menubutton.h \
    recipientview.h \
    returndateview.h \
    scannerview.h \
    signaturepad.h \
    tableview.h \
    tests/qrencodertest.h \
    viewdata.h \
    viewdatahandover.h \
    viewdatakeychainstatus.h \
    viewdatarecipient.h \
    viewdatareturndate.h \
    viewdatascanner.h \
    viewstack.h \
    viewstackmanager.h \
    winsubmenu.h

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

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/res/values/libs.xml \
    android/res/xml/qtprovider_paths.xml

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

#ANDROID_VERSION_CODE = "0.1"
#ANDROID_VERSION_NAME = "0.1"

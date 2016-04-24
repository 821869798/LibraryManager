#-------------------------------------------------
#
# Project created by QtCreator 2016-01-27T18:09:52
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LibraryManager
TEMPLATE = app


SOURCES += main.cpp \
    logindialog.cpp \
    skinmessagebox.cpp \
    styletool.cpp \
    tool.cpp \
    rootadmindialog.cpp \
    adminmanageform.cpp \
    bookmanageform.cpp \
    skininputbox.cpp \
    addadmindialog.cpp \
    readermanageform.cpp \
    bookeditdialog.cpp \
    booksearchform.cpp \
    bookinfoform.cpp \
    settingform.cpp \
    formswitch.cpp \
    readereditdialog.cpp \
    adminerdialog.cpp \
    visitordialog.cpp \
    readerdialog.cpp

HEADERS  += \
    logindialog.h \
    skinmessagebox.h \
    styletool.h \
    tool.h \
    rootadmindialog.h \
    adminmanageform.h \
    bookmanageform.h \
    skininputbox.h \
    sqldata.h \
    addadmindialog.h \
    readermanageform.h \
    bookeditdialog.h \
    booksearchform.h \
    bookinfoform.h \
    settingform.h \
    formswitch.h \
    readereditdialog.h \
    adminerdialog.h \
    visitordialog.h \
    readerdialog.h

FORMS    += \
    logindialog.ui \
    skinmessagebox.ui \
    rootadmindialog.ui \
    adminmanageform.ui \
    bookmanageform.ui \
    skininputbox.ui \
    addadmindialog.ui \
    readermanageform.ui \
    bookeditdialog.ui \
    booksearchform.ui \
    bookinfoform.ui \
    settingform.ui \
    readereditdialog.ui \
    adminerdialog.ui \
    visitordialog.ui \
    readerdialog.ui

RESOURCES += \
    rc.qrc

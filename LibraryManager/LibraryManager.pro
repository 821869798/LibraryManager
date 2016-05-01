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
    settingform.cpp \
    formswitch.cpp \
    readereditdialog.cpp \
    adminerdialog.cpp \
    visitordialog.cpp \
    readerdialog.cpp \
    bookinfodialog.cpp \
    borrowhistoryform.cpp \
    borrowdialog.cpp \
    borrowcompletedialog.cpp \
    readerinfoform.cpp

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
    settingform.h \
    formswitch.h \
    readereditdialog.h \
    adminerdialog.h \
    visitordialog.h \
    readerdialog.h \
    bookinfodialog.h \
    borrowhistoryform.h \
    borrowdialog.h \
    borrowcompletedialog.h \
    readerinfoform.h

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
    settingform.ui \
    readereditdialog.ui \
    adminerdialog.ui \
    visitordialog.ui \
    readerdialog.ui \
    bookinfodialog.ui \
    borrowhistoryform.ui \
    borrowdialog.ui \
    borrowcompletedialog.ui \
    readerinfoform.ui

RESOURCES += \
    rc.qrc

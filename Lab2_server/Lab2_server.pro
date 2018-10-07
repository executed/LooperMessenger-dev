
QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab2_server
TEMPLATE = app

CONFIG -= console

SOURCES +=\
    myserver.cpp \
    myclient.cpp \
    dialog.cpp \
    mydao.cpp \
    myentity.cpp \
    main.cpp \
    filehandler.cpp

HEADERS  += \
    myserver.h \
    dialog.h \
    myclient.h \
    mydao.h \
    myentity.h \
    ui_dialog.h \
    messages.h \
    string_constants.h \
    filehandler.h

FORMS    += dialog.ui

DISTFILES +=

RESOURCES += \
    resources.qrc















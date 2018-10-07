#-------------------------------------------------
#
# Project created by QtCreator 2011-10-13T22:35:09
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab2_client
TEMPLATE = app

SOURCES += main.cpp\
    authentication.cpp \
    mainwindow.cpp \
    messagewindow.cpp

HEADERS  += \
    authentication.h \
    mainwindow.h \
    myclient.h \
    messagewindow.h

FORMS    += dialog.ui \
    authentication.ui \
    mainwindow.ui \
    messagewindow.ui

RESOURCES += \
    resources.qrc

DISTFILES +=





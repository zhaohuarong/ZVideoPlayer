QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZVideoPlayer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/include

LIBS += -L$$PWD/lib -lVLCQtCore -lVLCQtWidgets

DESTDIR = $$PWD/bin

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resource.qrc

RC_FILE = app.rc

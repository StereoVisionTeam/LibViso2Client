#-------------------------------------------------
#
# Project created by QtCreator 2014-03-17T13:23:04
#
#-------------------------------------------------

QT       += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LibViso2Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    functioncodes.h \
    errorcodes.h \
    glwidget.h

FORMS    += mainwindow.ui


LIBS += -lglut -lGLU

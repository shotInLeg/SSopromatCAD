#-------------------------------------------------
#
# Project created by QtCreator 2016-12-23T21:04:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SSopromatCAD
TEMPLATE = app


SOURCES += main.cpp\
        ssopromatcad.cpp \
    SSopromatCADKernel/SSopromatCADKernel.cpp \
    startform.cpp

HEADERS  += ssopromatcad.h \
    SSopromatCADKernel/SSopromatCADKernel.h \
    startform.h

FORMS    += ssopromatcad.ui \
    startform.ui

RESOURCES += \
    recourses.qrc

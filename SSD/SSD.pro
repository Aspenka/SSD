#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T12:16:23
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = SSD
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

SOURCES += main.cpp \
    Task.cpp \
    TaskManager.cpp \
    Scheduler.cpp \
    Timer.cpp \
    CronParser.cpp \
    Server.cpp \
    Model.cpp \
    Query.cpp \
    TableSchema.cpp \
    TaskModel.cpp

HEADERS += \
    Task.h \
    TaskManager.h \
    Scheduler.h \
    Timer.h \
    CronParser.h \
    Server.h \
    Model.h \
    Query.h \
    TableSchema.h \
    TaskModel.h

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
    Model.cpp \
    Query.cpp \
    TableSchema.cpp \
    HTTPServer.cpp \
    DCServer.cpp \
    TableModel.cpp \
    Message.cpp \
    Request.cpp \
    var.cpp \
    Action.cpp \
    UpdateAction.cpp

HEADERS += \
    Task.h \
    TaskManager.h \
    Scheduler.h \
    Timer.h \
    CronParser.h \
    Model.h \
    Query.h \
    TableSchema.h \
    HTTPServer.h \
    DCServer.h \
    TableModel.h \
    Message.h \
    Request.h \
    var.h \
    Action.h \
    UpdateAction.h

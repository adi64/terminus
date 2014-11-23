TEMPLATE = app

CONFIG += c++11

QT += qml quick widgets

SOURCES += main.cpp \
    scene.cpp \
    game.cpp \
    abstractgraphicsobject.cpp \
    squircle.cpp \
    camera.cpp \
    train.cpp \
    enginewagon.cpp \
    abstractwagon.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    scene.h \
    game.h \
    abstractgraphicsobject.h \
    squircle.h \
    camera.h \
    train.h \
    train.hpp \
    enginewagon.h \
    abstractwagon.h

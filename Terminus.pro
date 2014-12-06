TEMPLATE = app

CONFIG += c++11

QT += qml quick widgets

SOURCES += \
    source/wagons/abstractwagon.cpp \
    source/wagons/enginewagon.cpp \
    source/wagons/weaponwagon.cpp \
    source/abstractgraphicsobject.cpp \
    source/camera.cpp \
    source/game.cpp \
    source/main.cpp \
    source/scene.cpp \
    source/squircle.cpp \
    source/train.cpp \
    source/geometry.cpp \
    source/resourcemanager.cpp \
    source/indextriple.cpp \
    source/material.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    source/wagons/abstractwagon.h \
    source/wagons/enginewagon.h \
    source/wagons/weaponwagon.h \
    source/abstractgraphicsobject.h \
    source/camera.h \
    source/game.h \
    source/scene.h \
    source/squircle.h \
    source/train.h \
    source/train.hpp \
    source/geometry.h \
    source/resourcemanager.h \
    source/indextriple.h \
    source/material.h

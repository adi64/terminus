TEMPLATE = app

CONFIG += c++11

QT += qml quick widgets multimedia

SOURCES += \
    source/wagons/abstractwagon.cpp \
    source/wagons/enginewagon.cpp \
    source/wagons/weaponwagon.cpp \
    source/abstractgraphicsobject.cpp \
    source/camera.cpp \
    source/game.cpp \
    source/main.cpp \
    source/scene.cpp \
    source/train.cpp \
    source/terrain.cpp \
    source/track.cpp \
    source/resources/material.cpp \
    source/resources/geometry.cpp \
    source/resources/indextriple.cpp \
    source/resources/resourcemanager.cpp \
    source/resources/soundmanager.cpp \
    source/skybox.cpp \
    source/resources/program.cpp \
    source/eventhandler.cpp \
    source/catmullromspline.cpp \
    source/perlinnoise.cpp \
    source/mathutil.cpp \
    source/polyline.cpp \
    source/level.cpp \
    source/deferredactionhandler.cpp

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
    source/train.h \
    source/train.hpp \
    source/terrain.h \
    source/track.h \
    source/resources/geometry.h \
    source/resources/indextriple.h \
    source/resources/resourcemanager.h \
    source/resources/material.h \
    source/resources/soundmanager.h \
    source/resources/program.h \
    source/skybox.h \
    source/eventhandler.h \
    source/catmullromspline.h \
    source/perlinnoise.h \
    source/mathutil.h \
    source/polyline.h \
    source/level.h \
    source/deferredactionhandler.h

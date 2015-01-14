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
    source/projectile.cpp \
    source/abstractphysicsobject.cpp \
    source/dynamicphysicsobject.cpp \
    source/kinematicphysicsobject.cpp

RESOURCES += qml.qrc

INCLUDEPATH += ../bullet/install/include
INCLUDEPATH += ../bullet/install/include/bullet

BULLET_LIB_PATH = $$_PRO_FILE_PWD_/../bullet/install/lib
BULLET_BIN_PATH = $$_PRO_FILE_PWD_/../bullet/install/bin

LIBS += -L$$BULLET_LIB_PATH -L$$BULLET_BIN_PATH -lBulletDynamics -lBulletCollision -lLinearMath

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
    source/projectile.h \
    source/abstractphysicsobject.h \
    source/dynamicphysicsobject.h \
    source/kinematicphysicsobject.h

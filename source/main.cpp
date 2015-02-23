#include <string>
#include <fstream>
#include <iostream>

#include <QDebug>
#include <QApplication>
#include <QQuickView>

#include "game.h"
#include "userinterface.h"

#include <resources/resourcemanager.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<terminus::Game>("Game", 1, 0, "Game");
    qmlRegisterType<terminus::QMLTrain>("Game", 1, 0, "QMLTrain");
    qmlRegisterType<terminus::UserInterface>("Game", 1, 0, "UserInterface");
    qmlRegisterType<terminus::QMLWagon>("Game", 1, 0, "QMLWagon");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/source/qml/main.qml")));
    //view.show();
    view.showFullScreen();

    return app.exec();
}

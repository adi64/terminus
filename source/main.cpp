#include <string>
#include <fstream>
#include <iostream>

#include <QApplication>
#include <QQuickView>

#include "game.h"

#include <resources/resourcemanager.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<terminus::Game>("terminus", 1, 0, "Game");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/source/qml/main.qml")));
    //view.show();
    view.showFullScreen();

    return app.exec();
}

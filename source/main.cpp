#include <string>
#include <fstream>
#include <iostream>

#include <QDebug>
#include <QApplication>
#include <QQuickView>

#include "game.h"

#include <resources/resourcemanager.h>
#include <network/ipprovider.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<terminus::Game>("Game", 1, 0, "Game");
    qmlRegisterType<terminus::IPProvider>("Game", 1, 0, "IPProvider");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/source/qml/main.qml")));
    //view.show();
    view.showFullScreen();

    QObject::connect((QObject*)view.engine(), SIGNAL(quit()), &app, SLOT(quit()));

    return app.exec();
}

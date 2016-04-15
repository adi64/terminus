#include <string>
#include <fstream>
#include <iostream>
#include <time.h>

#include <QDebug>
#include <QApplication>
#include <QQuickView>

#include "game.h"

#include <network/ipprovider.h>

int main(int argc, char *argv[])
{
    // seed PRNG
    srand(time(NULL));

    QApplication app(argc, argv);

    qmlRegisterType<terminus::Game>("Game", 1, 0, "Game");
    qmlRegisterType<terminus::IPProvider>("Game", 1, 0, "IPProvider");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/source/qml/main.qml")));
    view.showFullScreen();

    QObject::connect((QObject*)view.engine(), SIGNAL(quit()), &app, SLOT(quit()), Qt::QueuedConnection);

    return app.exec();
}

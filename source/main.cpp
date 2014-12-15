#include <QApplication>
#include <QQuickView>

#include "game.h"

#include "resources/resourcemanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<terminus::Game>("terminus", 1, 0, "Game");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/terminus/main.qml")));
    view.show();

    return app.exec();
}

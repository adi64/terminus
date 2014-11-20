#include <QApplication>
#include <QQuickView>

#include "game.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<Terminus::Game>("terminus", 1, 0, "Game");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/terminus/main.qml")));
    view.show();

    return app.exec();
}

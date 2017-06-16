#include <string>
#include <fstream>
#include <iostream>

#include <QDebug>
#include <QApplication>
#include <QQuickView>

#include "game.h"
#include "glincl.h"

#include <resources/resourcemanager.h>
#include <network/ipprovider.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // TODO FIXME HACK INCOMING
    // A QOpenGLContext "belongs" to a QSurface (in our case, a QQuickView)
    // that is "living" in a certain thread (the render thread).
    // Our problem is that the Game object is apparently not instantiated from
    // this render thread, but from another thread.
    // This is usually a good thing, since we can process events asynchronously.
    // But in order to initialize e.g. a FrameBufferObject, we need to have
    // initialized the QOpenGLExtraFunctions object, for which we need a
    // "current" QOpenGLContext.
    // Basically, we need to issue OpenGL calls from a thread which is not the
    // render thread.
    // This attribute disables the check that enforces the thread "boundary" and
    // allows us to issue OpenGL calls from any thread.
    // In the future it would be nice if we could refactor our code so that
    // we initialize all graphics-dependent stuff at a later time, so that we
    // don't have to rely on this ugly hack.
    app.setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity);

    qmlRegisterType<terminus::Game>("Game", 1, 0, "Game");
    qmlRegisterType<terminus::IPProvider>("Game", 1, 0, "IPProvider");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/source/qml/main.qml")));
    //view.showFullScreen();
    view.show();

    QObject::connect((QObject*)view.engine(), SIGNAL(quit()), &app, SLOT(quit()), Qt::QueuedConnection);

    return app.exec();
}

#include "game.h"

#include <QQuickView>

#include "scene.h"

namespace Terminus
{

Game::Game()
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    m_scene = new Scene;
}

Game::~Game()
{
    delete m_scene;
}

void Game::sync()
{
    qDebug("sync viewport and stuff");
    m_scene->setViewportSize(window()->size() * window()->devicePixelRatio());
}

void Game::cleanup()
{
    qDebug("cleanup");
}

void Game::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeRendering()), m_scene, SLOT(render()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

}

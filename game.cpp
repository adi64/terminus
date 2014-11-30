#include "game.h"

#include <QQuickView>

#include "scene.h"
#include "resourcemanager.h"
#include "squircle.h"

namespace terminus
{

Game::Game()
: m_scene(new Scene())
, m_resourceManager(new ResourceManager())
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    Squircle* testSquircle = new Squircle;
    m_scene->addNode(testSquircle);
}

Game::~Game()
{
    delete m_scene;
}

void Game::sync()
{
    qDebug("sync viewport and stuff");
    //TODO  // m_scene->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_scene->camera().setViewport(window()->width(), window()->height());
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

#include "game.h"

#include <memory>

#include <QQuickView>

#include "scene.h"
#include "squircle.h"
#include "train.h"

#include "enginewagon.h"
#include "weaponwagon.h"

namespace terminus
{

Game::Game()
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    m_scene = new Scene;

    Squircle* testSquircle = new Squircle;
    m_scene->addNode(testSquircle);

    auto playerTrain = std::unique_ptr<Train>(new Train);;
    playerTrain->addWagon<EngineWagon>();
    playerTrain->addWagon<WeaponWagon>();
    playerTrain->addWagon<WeaponWagon>();
    playerTrain->moveWagon(1, 2);
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

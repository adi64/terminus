#include "game.h"

#include <memory>

#include <QQuickView>

#include "scene.h"
#include "squircle.h"
#include "train.h"

#include "wagons/enginewagon.h"
#include "wagons/weaponwagon.h"

namespace terminus
{

Game::Game()
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    m_scene = new Scene;

    //Squircle* testSquircle = new Squircle;
    //m_scene->addNode(testSquircle);

    m_playerTrain = std::unique_ptr<Train>(new Train(m_scene));
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->moveWagon(1, 2);
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


    // get context opengl-version
    qDebug() << "Widget OpenGl: " << window()->format().majorVersion() << "." << window()->format().minorVersion();
    qDebug() << "Context valid: " << window()->openglContext()->isValid();
    qDebug() << "Really used OpenGl: " << window()->openglContext()->format().majorVersion() << "." << window()->openglContext()->format().minorVersion();
    qDebug() << "OpenGl information: VENDOR: " << (const char*)glGetString(GL_VENDOR);
    qDebug() << " RENDERDER: " << (const char*)glGetString(GL_RENDERER);
    qDebug() << " VERSION: " << (const char*)glGetString(GL_VERSION);
    qDebug() << " GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

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

#include "game.h"

#include <memory>

#include <QQuickView>
#include <QTimer>
#include <QTime>
#include <QVector3D>

#include "scene.h"
#include "resources/resourcemanager.h"
#include "train.h"
#include "terrain.h"

#include "wagons/enginewagon.h"
#include "wagons/weaponwagon.h"

#include "resources/soundmanager.h" //TODO remove
#include <QDebug>

namespace terminus
{

Game::Game()
: m_scene(new Scene())
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    m_scene = new Scene;

    m_timer = new QTimer();
    m_timeStamp = new QTime();
    m_timeStamp->start();

    qDebug() << "before playing";
    SoundManager::getInstance()->playSound("test");
    qDebug() << "after playing";

    m_terrain = std::unique_ptr<Terrain>(new Terrain(m_scene));

    m_playerTrain = std::unique_ptr<Train>(new Train(m_scene, m_terrain->playerTrack()));
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->moveWagon(1, 2);

    m_scene->setInitialTimeStamp(m_timeStamp);

    m_scene->addNode(m_playerTrain.get());
    m_scene->addNode(m_terrain.get());

    m_scene->camera().setEye(QVector3D(0.0, 1.0, 20.0));
    m_scene->camera().setCenter(QVector3D(0.0, 1.0, 0.0));
    m_scene->camera().setUp(QVector3D(0.0, 1.0, 0.0));
}

Game::~Game()
{
    delete m_scene;
}

void Game::sync()
{
    //TODO  // m_scene->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_scene->camera().setViewport(window()->width(), window()->height());


    //Debug Stuff
    // get context opengl-version
    /*
    qDebug() << "Widget OpenGl: " << window()->format().majorVersion() << "." << window()->format().minorVersion();
    qDebug() << "Context valid: " << window()->openglContext()->isValid();
    qDebug() << "Really used OpenGl: " << window()->openglContext()->format().majorVersion() << "." << window()->openglContext()->format().minorVersion();
    qDebug() << "OpenGl information: VENDOR: " << (const char*)glGetString(GL_VENDOR);
    qDebug() << " RENDERDER: " << (const char*)glGetString(GL_RENDERER);
    qDebug() << " VERSION: " << (const char*)glGetString(GL_VERSION);
    qDebug() << " GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    */

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

        // force redraw
        connect(m_timer, &QTimer::timeout, win, &QQuickWindow::update);
        m_timer->start(1000 / 60);
    }
}

void Game::handleKeyboardEvent(Qt::Key key)
{
    qDebug() << "ermergerd keyboard event: " << key;
    if(key == Qt::Key_W)
    {
        auto newEye = m_scene->camera().eye();
        newEye += QVector3D(0.0, 0.0, -1.0);
        m_scene->camera().setEye(newEye);
    }
    if(key == Qt::Key_S)
    {
        auto newEye = m_scene->camera().eye();
        newEye += QVector3D(0.0, 0.0, 1.0);
        m_scene->camera().setEye(newEye);
    }
    if(key == Qt::Key_A)
    {
        auto newEye = m_scene->camera().eye();
        newEye += QVector3D(-1.0, 0.0, 0.0);
        m_scene->camera().setEye(newEye);
    }
    if(key == Qt::Key_D)
    {
        auto newEye = m_scene->camera().eye();
        newEye += QVector3D(1.0, 0.0, 0.0);
        m_scene->camera().setEye(newEye);
    }
    if(key == Qt::Key_Space)
    {
        auto newEye = m_scene->camera().eye();
        newEye += QVector3D(0.0, 1.0, 0.0);
        m_scene->camera().setEye(newEye);
    }
    if(key == Qt::Key_Control)
    {
        auto newEye = m_scene->camera().eye();
        newEye += QVector3D(0.0, -1.0, 0.0);
        m_scene->camera().setEye(newEye);
    }
    qDebug() << "Camera at " << m_scene->camera().eye() << " looking at " << m_scene->camera().center();
}

}

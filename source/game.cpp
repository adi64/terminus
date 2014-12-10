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

    m_terrain = std::unique_ptr<Terrain>(new Terrain(m_scene));

    m_playerTrain = std::unique_ptr<Train>(new Train(m_scene, m_terrain->playerTrack()));
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->moveWagon(1, 2);

    m_enemyTrain = std::unique_ptr<Train>(new Train(m_scene, m_terrain->enemyTrack()));
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();

    m_scene->setInitialTimeStamp(m_timeStamp);

    m_scene->addNode(m_playerTrain.get());
    m_scene->addNode(m_enemyTrain.get());
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

void Game::keyPressEvent(Qt::Key key)
{
    auto movement = m_scene->camera().movement();
    auto rotation = m_scene->camera().rotation();

    switch(key)
    {
    case Qt::Key_W:
        movement.setZ(-1.0);
        m_scene->camera().setMovement(movement);
        break;
    case Qt::Key_S:
        movement.setZ(1.0);
        m_scene->camera().setMovement(movement);
        break;
    case Qt::Key_A:
        movement.setX(-1.0);
        m_scene->camera().setMovement(movement);
        break;
    case Qt::Key_D:
        movement.setX(1.0);
        m_scene->camera().setMovement(movement);
        break;
    case Qt::Key_Left:
        rotation.setX(-5.0);
        m_scene->camera().setRotation(rotation);
        break;
    case Qt::Key_Right:
        rotation.setX(5.0);
        m_scene->camera().setRotation(rotation);
        break;
    case Qt::Key_Up:
        rotation.setY(5.0);
        m_scene->camera().setRotation(rotation);
        break;
    case Qt::Key_Down:
        rotation.setY(-5.0);
        m_scene->camera().setRotation(rotation);
        break;
    case Qt::Key_R:
        movement.setY(1.0);
        m_scene->camera().setMovement(movement);
        break;
    case Qt::Key_F:
        movement.setY(-1.0);
        m_scene->camera().setMovement(movement);
        break;
    default:
        break;
    }
}

void Game::keyReleaseEvent(Qt::Key key)
{
    auto movement = m_scene->camera().movement();
    auto rotation = m_scene->camera().rotation();

    switch(key)
    {
    case Qt::Key_W:
        movement.setZ(0.0);
        m_scene->camera().setMovement(movement);
        break;
    case Qt::Key_S:
        movement.setZ(0.0);
        m_scene->camera().setMovement(movement);
        break;
    case Qt::Key_A:
        movement.setX(0.0);
        m_scene->camera().setMovement(movement);
        break;
    case Qt::Key_D:
        movement.setX(0.0);
        m_scene->camera().setMovement(movement);
        break;
    case Qt::Key_Left:
        rotation.setX(0.0);
        m_scene->camera().setRotation(rotation);
        break;
    case Qt::Key_Right:
        rotation.setX(0.0);
        m_scene->camera().setRotation(rotation);
        break;
    case Qt::Key_Up:
        rotation.setY(0.0);
        m_scene->camera().setRotation(rotation);
        break;
    case Qt::Key_Down:
        rotation.setY(0.0);
        m_scene->camera().setRotation(rotation);
        break;
    case Qt::Key_R:
        movement.setY(0.0);
        m_scene->camera().setMovement(movement);
        break;
    case Qt::Key_F:
        movement.setY(0.0);
        m_scene->camera().setMovement(movement);
        break;
    default:
        break;
    }
}

}

#include "game.h"

#include <memory>

#include <QQuickView>
#include <QTimer>
#include <QTime>
#include <QVector3D>
#include <QApplication>

#include "scene.h"
#include "resources/resourcemanager.h"
#include "resources/soundmanager.h"
#include "train.h"
#include "terrain.h"
#include "skybox.h"

#include "resources/resourcemanager.h"
#include "wagons/enginewagon.h"
#include "wagons/weaponwagon.h"

namespace terminus
{

Game::Game()
: m_scene(new Scene())
, m_timer(new QTimer())
, m_timeStamp(new QTime())
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    ResourceManager::getInstance()->loadResources();

    m_timeStamp->start();


    // Set up bullet
    // Build the broadphase
    m_bullet_broadphase = std::unique_ptr<btBroadphaseInterface>(new btDbvtBroadphase());

    // Set up the collision configuration and dispatcher
    m_bullet_collisionConfiguration = std::unique_ptr<btDefaultCollisionConfiguration>(new btDefaultCollisionConfiguration());
    m_bullet_dispatcher = std::unique_ptr<btCollisionDispatcher>(new btCollisionDispatcher(m_bullet_collisionConfiguration.get()));

    // The actual physics solver
    m_bullet_solver = std::unique_ptr<btSequentialImpulseConstraintSolver>(new btSequentialImpulseConstraintSolver);

    // The world.
    m_bullet_dynamicsWorld = std::unique_ptr<btDiscreteDynamicsWorld>(
                new btDiscreteDynamicsWorld(
                    m_bullet_dispatcher.get(),
                    m_bullet_broadphase.get(),
                    m_bullet_solver.get(),
                    m_bullet_collisionConfiguration.get()
                    )
                );
    m_bullet_dynamicsWorld->setGravity(btVector3(0, -10, 0));




    SoundManager::getInstance()->playBackgroundMusic();

    m_terrain = std::unique_ptr<Terrain>(new Terrain(m_scene));

    m_playerTrain = std::unique_ptr<Train>(new Train(m_scene, m_terrain->playerTrack()));
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->moveWagon(1, 2);

    m_enemyTrain = std::unique_ptr<Train>(new Train(m_scene, m_terrain->enemyTrack()));
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();

    m_skybox = std::unique_ptr<SkyBox>(new SkyBox(m_scene));

    m_scene->setInitialTimeStamp(m_timeStamp);

    m_scene->addNode(m_playerTrain.get());
    m_scene->addNode(m_enemyTrain.get());
    m_scene->addNode(m_terrain.get());
    m_scene->addNode(m_skybox.get());

    m_scene->camera().setEye(QVector3D(-30.0, 10.0, 20.0));
    m_scene->camera().setCenter(QVector3D(0.0, 0.0, 10.0));
    m_scene->camera().setUp(QVector3D(0.0, 1.0, 0.0));
    m_scene->camera().lockToObject(m_playerTrain->wagonAt(0));


    // physics tests
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

    btCollisionShape* fallShape = new btSphereShape(1);

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    m_bullet_dynamicsWorld->addRigidBody(groundRigidBody);

    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
    btScalar mass = 1;
    btVector3 fallInertia(0, 0, 0);
    fallShape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);

    m_fallRigidBody = std::unique_ptr<btRigidBody>(new btRigidBody(fallRigidBodyCI));
    m_bullet_dynamicsWorld->addRigidBody(m_fallRigidBody.get());
}

Game::~Game()
{
    delete m_scene;
}

void Game::sync()
{
    auto elapsedMilliseconds = m_timeStamp->restart();

    // physics
    m_bullet_dynamicsWorld->stepSimulation((float)elapsedMilliseconds / 1000.0f, 10);

    btTransform trans;
    m_fallRigidBody->getMotionState()->getWorldTransform(trans);

    qDebug() << "sphere height: " << trans.getOrigin().getY();


    //TODO  // m_scene->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_scene->camera().setViewport(window()->width(), window()->height());

    m_scene->update(elapsedMilliseconds);


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

void Game::render()
{
    m_scene->render();
}

void Game::cleanup()
{
    qDebug("cleanup");
}

void Game::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeRendering()), this, SLOT(render()), Qt::DirectConnection);
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

    // TODO FIXME: find a proper place to store locked wagon index
    static auto lockedWagonIndex = 0;

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
    case Qt::Key_Q:
        QApplication::quit();
        break;
    case Qt::Key_Space:
        m_scene->camera().toggleLocked();
        break;
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    case Qt::Key_U:
        SoundManager::getInstance()->playSound("angriff");
        break;
    case Qt::Key_I:
        SoundManager::getInstance()->playSound("shot");
        break;
    case Qt::Key_O:
        SoundManager::getInstance()->playSound("alarm");
        break;
    case Qt::Key_M:
        SoundManager::getInstance()->toggleBackgroundMusic();
        break;
    case Qt::Key_Plus:
        if(m_scene->camera().isLocked() && ((lockedWagonIndex + 1) < m_playerTrain->size()))
        {
            lockedWagonIndex++;
            m_scene->camera().lockToObject(m_playerTrain->wagonAt(lockedWagonIndex));
        }
        break;
    case Qt::Key_Minus:
        if(m_scene->camera().isLocked() && lockedWagonIndex > 0)
        {
            lockedWagonIndex--;
            m_scene->camera().lockToObject(m_playerTrain->wagonAt(lockedWagonIndex));
        }
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

void Game::mouseMoveEvent(qreal x, qreal y)
{
    const double sensitivity = 0.5;

    auto oldPosition = QVector2D(window()->width() / 2, window()->height() / 2);
    auto offset = oldPosition - QVector2D(x, y);
    auto rotation = offset * sensitivity;

    // invert X
    rotation *= QVector2D(-1.0, 1.0);

    m_scene->camera().setRotation(rotation);

    QPoint globalPosition = window()->mapToGlobal(QPoint(window()->width() / 2, window()->height() / 2));
    QCursor::setPos(globalPosition);
}

}

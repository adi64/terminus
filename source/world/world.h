#pragma once

#include <list>
#include <memory>
#include <unordered_map>

#include <QObject>

#include <bullet/btBulletDynamicsCommon.h>

#include <util/timer.h>
#include <world/physics/bulletworld.h>
#include <resources/lightmanager.h>
#include <world/camera.h>
#include <deferredactionhandler.h>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QTime;

class btDiscreteDynamicsWorld;
class btCollisionObject;

namespace terminus
{
class Game;
class Train;
class Terrain;
class SkyBox;
class AbstractPlayer;
class AIPlayer;
class LocalPlayer;

class AbstractGraphicsObject;
class AbstractPhysicsObject;

class World : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief The one and only World constructor
     * \param game
     */
    World(Game & game);

    /*!
     * \brief Delete copy constructor
     */
    World(const World & other) = delete;

    /*!
     * \brief Delete assignment operator
     */
    World & operator=(const World & other) = delete;

    ~World();

    void update();
    void render(QOpenGLFunctions & gl) const;

    LocalPlayer & localPlayer();

    Train & playerTrain();
    Train & enemyTrain();
    Terrain & terrain();
    Timer & timer();

    LightManager & lightManager();

    void setInitialTimeStamp(const std::shared_ptr<QTime> &timeStamp);
    void scheduleAction(DeferredAction event);

    void addObject(AbstractGraphicsObject * node);
    void deleteObject(AbstractGraphicsObject * object);

    /*!
     * \brief Returns a shared pointer to the corresponding BulletWorld
     *
     * This getter is mainly used in the constructor of AbstractPhysicsObject.
     * AbstractPhysicsObjects that need to operate on the BulletWorld should not
     * call it via this getter, but use their own stored copy.
     */
    std::shared_ptr<BulletWorld> bulletWorld();

protected:
    Game & m_game;

    LightManager m_lightManager;

    std::shared_ptr<BulletWorld> m_bulletWorld;

    std::unique_ptr<SkyBox> m_skybox;
    std::unique_ptr<Terrain> m_terrain;
    std::shared_ptr<Train> m_rightTrain;
    std::shared_ptr<Train> m_leftTrain;

    std::unique_ptr<LocalPlayer> m_localPlayer;
    std::unique_ptr<AIPlayer> m_aiPlayer;

    std::list<std::unique_ptr<AbstractGraphicsObject>> m_dynamicObjects;
};

}

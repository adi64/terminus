#pragma once

#include <vector>
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
class NetworkManager;

class AbstractGraphicsObject;
class AbstractPhysicsObject;

class World : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a world for a local or multiplayer game
     * \param game Reference to a game instance
     * \param isNetworkGame Whether this game is a network game or a local singleplayer game
     * \param isPlayerOne Whether the local player is "player one" or "player two"
     * \param terrainSeed Seed value for the noise generator to synchronize the terrain with the other network player
     */
    World(Game & game, bool isNetworkGame, bool isPlayerOne, unsigned int terrainSeed);

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
    AbstractPlayer & enemyPlayer();

    Train & playerTrain();
    Train & enemyTrain();
    Terrain & terrain();
    Timer & timer();

    NetworkManager & networkManager();

    LightManager & lightManager();

    void setInitialTimeStamp(const std::shared_ptr<QTime> &timeStamp);
    void scheduleAction(DeferredAction event);

    void addNode(AbstractGraphicsObject * node);
    void deleteNode(AbstractGraphicsObject * node);

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

    std::unique_ptr<Terrain> m_terrain;
    std::unique_ptr<SkyBox> m_skybox;
    std::shared_ptr<Train> m_playerTrain;
    std::shared_ptr<Train> m_enemyTrain;

    std::unique_ptr<LocalPlayer> m_localPlayer;
    std::unique_ptr<AbstractPlayer> m_enemyPlayer;

    std::vector<AbstractGraphicsObject*> m_objects;
};

}

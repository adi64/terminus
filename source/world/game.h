#pragma once

#include <list>
#include <memory>
#include <unordered_map>

#include <QObject>

#include <bullet/btBulletDynamicsCommon.h>

#include <util/actionscheduler.h>
#include <util/timer.h>
#include <player/camera.h>
#include <world/level.h>
#include <world/lightmanager.h>
#include <world/physics/bulletworld.h>

class QTime;

class btCollisionObject;
class btDiscreteDynamicsWorld;

namespace terminus
{
class GameObject;
class AbstractPhysicsObject;
class AbstractPlayer;
class AIPlayer;
class Application;
class LocalPlayer;
class NetworkManager;
class PostprocessingManager;
class SkyBox;
class Terrain;
class Train;

/*!
 * \brief The World class represents a running game instance and contains all
 * graphical (3D) elements of the scene.
 *
 * A running game consists of a Terrain, two Train instances, other graphical elements
 * (Projectile, Skybox, ...) and two AbstractPlayer s controlling the two trains.
 * It also holds the BulletWorld, an encapsulation of all bullet-related classes and methods.
 */
class Game : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a world for a local or multiplayer game
     * \param game Reference to a game instance
     * \param isNetworkGame Whether this game is a network game or a local singleplayer game (with enemy AI)
     * \param isPlayerOne Whether the local player is "player one" or "player two"
     * \param terrainSeed Seed value for the noise generator to synchronize the terrain with the other network player
     */
    Game(Application & game, bool isNetworkGame, bool isPlayerOne, unsigned int terrainSeed);

    /*!
     * \brief Delete copy constructor
     */
    Game(const Game & other) = delete;

    /*!
     * \brief Delete assignment operator
     */
    Game & operator=(const Game & other) = delete;

    ~Game();

    void update();
    void render() const;

    LocalPlayer & localPlayer();
    AbstractPlayer & enemyPlayer();

    Train & localPlayerTrain();
    Train & enemyPlayerTrain();

    Level & level();
    Timer & timer();

    NetworkManager & networkManager();

    LightManager & lightManager();

    QSize viewport() const;

    void scheduleAction(ActionScheduler::Action event);

    void addObject(GameObject * node);
    void deleteObject(GameObject * object);

    /*!
     * \brief Returns a shared pointer to the corresponding BulletWorld
     *
     * This getter is mainly used in the constructor of AbstractPhysicsObject.
     * AbstractPhysicsObjects that need to operate on the BulletWorld should not
     * call it via this getter, but use their own stored copy.
     */
    std::shared_ptr<BulletWorld> bulletWorld();

protected:
    Application & m_game;

    Level m_level;

    LightManager m_lightManager;

    std::unique_ptr<PostprocessingManager> m_postprocessingManager;

    std::shared_ptr<BulletWorld> m_bulletWorld;

    std::unique_ptr<SkyBox> m_skybox;
    std::unique_ptr<Terrain> m_terrain;
    std::shared_ptr<Train> m_rightTrain;
    std::shared_ptr<Train> m_leftTrain;

    std::unique_ptr<LocalPlayer> m_localPlayer;
    std::unique_ptr<AbstractPlayer> m_enemyPlayer;

    std::list<std::unique_ptr<GameObject>> m_dynamicObjects;
};

}

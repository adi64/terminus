#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <QObject>

#include <world/physics/bulletworld.h>

#include <deferredactionhandler.h>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QTime;

namespace terminus
{
class Camera;
class Game;
class Timer;
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
    Timer & timer();

    NetworkManager & networkManager();

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

    std::shared_ptr<BulletWorld> m_bulletWorld;

    std::unique_ptr<Terrain> m_terrain;
    std::unique_ptr<SkyBox> m_skybox;
    std::unique_ptr<Train> m_playerTrain;
    std::unique_ptr<Train> m_enemyTrain;

    std::unique_ptr<LocalPlayer> m_localPlayer;
    std::unique_ptr<AIPlayer> m_aiPlayer;

    std::vector<AbstractGraphicsObject*> m_objects;
};



}

#pragma once

#include <memory>

#include <QObject>
#include <QQuickItem>

#include <bullet/btBulletDynamicsCommon.h>

class QTimer;
class QTime;

namespace terminus
{

class Train;
class Scene;
class EventHandler;
class Terrain;
class ResourceManager;
class SkyBox;

class Game : public QQuickItem
{
    Q_OBJECT

public:
    Game();
    ~Game();

    Scene *scene() const;
    Train *playerTrain() const;
public slots:
    void sync();
    void render();
    void cleanup();
    void handleWindowChanged(QQuickWindow* win);
    void keyPressEvent(Qt::Key key);
    void keyReleaseEvent(Qt::Key key);
    void mouseMoveEvent(qreal x, qreal y);
    void touchMoveEvent(qreal x, qreal y);
    void gyroMoveEvent(qreal x, qreal y);
    void flickEvent(qreal velo);
protected:
    Scene *m_scene;
    std::unique_ptr<EventHandler> m_eventHandler;
    std::unique_ptr<Train> m_playerTrain;
    std::unique_ptr<Train> m_enemyTrain;
    QTimer *m_timer;
    QTime *m_timeStamp;
    std::unique_ptr<Terrain> m_terrain;
    std::unique_ptr<SkyBox> m_skybox;

    // bullet
    std::unique_ptr<btBroadphaseInterface> m_bullet_broadphase;
    std::unique_ptr<btDefaultCollisionConfiguration> m_bullet_collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> m_bullet_dispatcher;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_bullet_solver;
    std::shared_ptr<btDiscreteDynamicsWorld> m_bullet_dynamicsWorld;
};

}

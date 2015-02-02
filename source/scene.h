#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <chrono>

#include <QObject>
#include <QOpenGLFunctions>

#include "camera.h"
#include "deferredactionhandler.h"
#include "timer.h"


class QOpenGLShaderProgram;
class QTime;

class btDiscreteDynamicsWorld;
class btCollisionObject;

namespace terminus
{

class AbstractGraphicsObject;

class Scene : public QObject
{
    Q_OBJECT
public:
    Scene(std::shared_ptr<btDiscreteDynamicsWorld> bulletWorld, std::shared_ptr<DeferredActionHandler> deferredActionHandler);
    ~Scene();

    void update();
    void render();

    Camera & camera();
    Timer & timer();
    btDiscreteDynamicsWorld* bullet_world();

    void scheduleAction(DeferredAction event);

    void addNode(AbstractGraphicsObject* node);
    void deleteNode(AbstractGraphicsObject* node);

    void addCollisionMapping(const btCollisionObject* collisionObject, AbstractGraphicsObject* graphicsObject);
    void removeCollisionMapping(const btCollisionObject* collisionObject);
    AbstractGraphicsObject *getGraphicsObjectForCollisionObject(const btCollisionObject* collisionObject) const;

protected:
    Timer m_timer;

    std::unique_ptr<Camera> m_camera;
    QOpenGLFunctions m_gl;
    std::vector<AbstractGraphicsObject*> m_nodes;

    std::shared_ptr<btDiscreteDynamicsWorld> m_bullet_world;
    std::shared_ptr<DeferredActionHandler> m_deferredActionHandler;
    std::unordered_map<const btCollisionObject*, AbstractGraphicsObject*> m_collisionMap;
};



}

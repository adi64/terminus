#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <QObject>
#include <QOpenGLFunctions>

#include "camera.h"
#include "deferredactionhandler.h"

class QOpenGLShaderProgram;
class QTime;

class btDiscreteDynamicsWorld;
class btCollisionObject;

namespace terminus
{

class AbstractGraphicsObject;
class AbstractPhysicsObject;

class Scene : public QObject
{
    Q_OBJECT
public:
    Scene(std::shared_ptr<btDiscreteDynamicsWorld> bulletWorld, std::shared_ptr<DeferredActionHandler> deferredActionHandler);
    ~Scene();

    void update(int elapsedMilliseconds);
    void render() const;

    Camera & camera();
    btDiscreteDynamicsWorld* bullet_world();
    void scheduleAction(DeferredAction event);

    void addNode(AbstractGraphicsObject* node);
    void deleteNode(AbstractGraphicsObject* node);

    void addCollisionMapping(const btCollisionObject* collisionObject, AbstractPhysicsObject* graphicsObject);
    void removeCollisionMapping(const btCollisionObject* collisionObject);
    terminus::AbstractPhysicsObject *getGraphicsObjectForCollisionObject(const btCollisionObject* collisionObject) const;

    void setInitialTimeStamp(const std::shared_ptr<QTime> &timeStamp);
protected:
private:
    std::unique_ptr<Camera> m_camera;
    mutable QOpenGLFunctions m_gl;
    std::vector<AbstractGraphicsObject*> m_nodes;
    std::shared_ptr<QTime> m_timeStamp;
    std::shared_ptr<btDiscreteDynamicsWorld> m_bullet_world;
    std::shared_ptr<DeferredActionHandler> m_deferredActionHandler;
    std::unordered_map<const btCollisionObject*, AbstractPhysicsObject*> m_collisionMap;
};



}

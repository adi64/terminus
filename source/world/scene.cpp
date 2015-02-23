#include "scene.h"

#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QTime>

#include <bullet/btBulletDynamicsCommon.h>

#include "abstractgraphicsobject.h"
#include "camera.h"
#include <world/physics/abstractphysicsobject.h>
#include <deferredactionhandler.h>

namespace terminus
{

Scene::Scene(std::shared_ptr<btDiscreteDynamicsWorld> bulletWorld, std::shared_ptr<DeferredActionHandler> deferredActionHandler)
: m_activeCamera(nullptr)
, m_gl()
, m_timeStamp(std::shared_ptr<QTime>(new QTime()))
, m_bullet_world(bulletWorld)
, m_deferredActionHandler(deferredActionHandler)
, m_collisionMap()
{
    m_nodes.clear();
}

Scene::~Scene()
{
}

void Scene::addNode(AbstractGraphicsObject *node)
{
    m_nodes.push_back(node);
}

void Scene::deleteNode(AbstractGraphicsObject *node)
{
    for(auto iterator = m_nodes.begin(); iterator != m_nodes.end(); ++iterator)
    {
        if(*iterator == node)
        {
            m_nodes.erase(iterator);
            return;
        }
    }

    qDebug() << "Could not find node " << node;
}

void Scene::addCollisionMapping(const btCollisionObject *collisionObject, AbstractPhysicsObject *graphicsObject)
{
    auto newPair = std::pair<const btCollisionObject*, AbstractPhysicsObject*>(collisionObject, graphicsObject);
    m_collisionMap.insert(newPair);
}

void Scene::removeCollisionMapping(const btCollisionObject *collisionObject)
{
    m_collisionMap.erase(collisionObject);
}

AbstractPhysicsObject *Scene::getGraphicsObjectForCollisionObject(const btCollisionObject *collisionObject) const
{
    try
    {
        return m_collisionMap.at(collisionObject);
    }
    catch(std::exception e)
    {
        qDebug() << "could not find AbstractGraphicsObject for collisionObject " << collisionObject;
        return nullptr;
    }
}

void Scene::setInitialTimeStamp(const std::shared_ptr<QTime>& timeStamp)
{
    m_timeStamp.reset();
    m_timeStamp = timeStamp;
}

void Scene::update(int elapsedMilliseconds)
{
    for(auto node : m_nodes)
    {
        node->update(elapsedMilliseconds);
    }

    // camera updates after all other nodes because it can follow the position of other nodes
    m_activeCamera->update();
}

void Scene::render() const
{

    static bool glInitialized = false;
    if(!glInitialized)
    {
        m_gl.initializeOpenGLFunctions();
        glInitialized = true;
    }

    glViewport(0, 0, m_activeCamera->viewport().x(), m_activeCamera->viewport().y());

    glClearColor(0.5f, 0.55f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW); //TODO check if still necessary after geometry loader

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    for(auto node : m_nodes)
    {
        node->render(m_gl);
    }

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

}

Camera & Scene::camera()
{
    return *m_activeCamera;
}

void Scene::setActiveCamera(std::shared_ptr<Camera> camera)
{
    m_activeCamera = camera;
}

btDiscreteDynamicsWorld *Scene::bullet_world()
{
    return m_bullet_world.get();
}

void Scene::scheduleAction(DeferredAction event)
{
    m_deferredActionHandler->scheduleAction(event);
}

} // namespace terminus

#include "scene.h"

#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QTime>

#include <bullet/btBulletDynamicsCommon.h>

#include "abstractgraphicsobject.h"
#include "camera.h"

namespace terminus
{

Scene::Scene(std::shared_ptr<btDiscreteDynamicsWorld> bulletWorld)
: m_camera(new Camera())
, m_gl()
, m_timeStamp(nullptr)
, m_bullet_world(bulletWorld)
{
    m_nodes.clear();
    qDebug() << m_bullet_world.use_count();
}

Scene::~Scene()
{
    delete m_camera;
    m_camera = nullptr;
}

void Scene::addNode(AbstractGraphicsObject *node)
{
    m_nodes.push_back(node);
}

void Scene::setInitialTimeStamp(QTime *timeStamp)
{
    m_timeStamp = timeStamp;
}

void Scene::update(int elapsedMilliseconds)
{
    for(auto node : m_nodes)
    {
        node->update(elapsedMilliseconds);
    }

    // camera updates after all other nodes because it can follow the position of other nodes
    m_camera->update();
}

void Scene::render()
{

    static bool glInitialized = false;
    if(!glInitialized)
    {
        m_gl.initializeOpenGLFunctions();
        glInitialized = true;
    }

    glViewport(0, 0, m_camera->viewport().x(), m_camera->viewport().y());

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
    return *m_camera;
}

btDiscreteDynamicsWorld *Scene::bullet_world()
{
    return m_bullet_world.get();
}

} // namespace terminus

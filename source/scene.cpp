#include "scene.h"

#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QTime>

#include "abstractgraphicsobject.h"
#include "camera.h"

namespace terminus
{

Scene::Scene()
: m_camera(new Camera())
, m_gl()
, m_timeStamp(nullptr)
{
    m_nodes.clear();
}

Scene::~Scene()
{
    delete m_camera;
}

void Scene::addNode(AbstractGraphicsObject *node)
{
    m_nodes.push_back(node);
}

void Scene::setInitialTimeStamp(QTime *timeStamp)
{
    m_timeStamp = timeStamp;
}

void Scene::render()
{

    static bool glInitialized = false;
    if(!glInitialized)
    {
        m_gl.initializeOpenGLFunctions();
        glInitialized = true;
    }

    qDebug("Start rendering:");

    auto elapsedMilliseconds = m_timeStamp->restart();

    glViewport(0, 0, m_camera->viewport().x(), m_camera->viewport().y());

    glClearColor(0.5f, 0.55f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDisable(GL_CULL_FACE);
    //glFrontFace(GL_CW); //TODO check if still necessary after geometry loader

    for(auto node : m_nodes)
    {
        node->render(m_gl, elapsedMilliseconds);
    }

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

}

Camera & Scene::camera()
{
    return *m_camera;
}

} // namespace terminus
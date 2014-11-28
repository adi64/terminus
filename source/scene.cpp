#include "scene.h"

#include <QDebug>
#include <QOpenGLShaderProgram>

#include "abstractgraphicsobject.h"
#include "camera.h"

namespace terminus
{

Scene::Scene()
: m_camera(new Camera())
{
    m_nodes.clear();
}

Scene::~Scene()
{
    delete m_camera;
}

void Scene::addNode(AbstractGraphicsObject *node)
{
    qDebug() << "m_nodes.size() = " << m_nodes.size();
    m_nodes.push_back(node);
}

void Scene::render()
{
    qDebug("render");

    glViewport(0, 0, m_camera->viewport().x, m_camera->viewport().y);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for(AbstractGraphicsObject* node : m_nodes)
    {
        node->render();
    }

}

Camera & Scene::camera()
{
    return *m_camera;
}

} // namespace terminus

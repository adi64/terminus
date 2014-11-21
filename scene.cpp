#include "scene.h"

#include <QOpenGLShaderProgram>

#include "abstractgraphicsobject.h"
#include "camera.h"

namespace terminus
{

Scene::Scene()
: m_camera(new Camera())
{
}

Scene::~Scene()
{
    delete m_camera;
}

void Scene::addNode(AbstractGraphicsObject *node)
{
    m_nodes.push_back(node);
}

void Scene::render()
{
    qDebug("render");

    glViewport(0, 0, m_camera->viewport().x(), m_camera->viewport().y());

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for(AbstractGraphicsObject* node : m_nodes)
    {
        node->render();
    }

}

Camera & camera()
{
    return *m_camera;
}

} // namespace terminus

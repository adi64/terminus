#include "scene.h"

#include <QOpenGLShaderProgram>

#include "abstractgraphicsobject.h"

namespace Terminus
{

Scene::Scene()
{
}

void Scene::addNode(AbstractGraphicsObject *node)
{
    m_nodes.push_back(node);
}

void Scene::render()
{
    qDebug("render");

    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for(AbstractGraphicsObject* node : m_nodes)
    {
        node->render();
    }

}


}

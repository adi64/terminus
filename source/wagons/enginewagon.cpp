#include "enginewagon.h"

#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QVector3D>

#include "../scene.h"
#include "../resources/resourcemanager.h"
#include "../resources/geometry.h"

namespace terminus
{

EngineWagon::EngineWagon(Scene *scene, Train *train)
    : AbstractWagon(scene, train)
    , m_program(nullptr)
{
}

void EngineWagon::render(QOpenGLFunctions& gl, int elapsedMilliseconds)
{
    if (!m_program)
    {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "data/enginewagon.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "data/enginewagon.frag");

        m_program->link();
    }

    m_program->bind();

    m_position = position();

    QMatrix4x4 model;
    model.setToIdentity();
    model.translate(m_position);

    QMatrix4x4 modelViewProjection;
    modelViewProjection.setToIdentity();
    modelViewProjection = m_scene->camera().viewProjection() * model;

    m_program->setUniformValue("mvp", modelViewProjection);

    std::shared_ptr<std::unique_ptr<Geometry>> ico = ResourceManager::getInstance()->getGeometry("base_Icosahedron");
    (**ico).setAttributes(*m_program);
    (**ico).draw(gl);

    m_program->release();
}

float EngineWagon::length() const
{
    return 6.0f;
}

}

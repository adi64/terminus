#include "weaponwagon.h"

#include <QDebug>
#include <QOpenGLShaderProgram>

#include "../scene.h"
#include "../resources/resourcemanager.h"
#include "../resources/geometry.h"

namespace terminus
{

WeaponWagon::WeaponWagon(Scene *scene, Train *train)
    : AbstractWagon(scene, train)
    , m_program(nullptr)
{
}

void WeaponWagon::render(QOpenGLFunctions& gl, int elapsedMilliseconds)
{
    if (!m_program)
    {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "data/weaponwagon.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "data/weaponwagon.frag");

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

    std::shared_ptr<std::unique_ptr<Geometry>> wagon = ResourceManager::getInstance()->getGeometry("base_Wagon");
    (**wagon).setAttributes(*m_program);
    (**wagon).draw(gl);

    m_program->release();
}

float WeaponWagon::length() const
{
    return 4.0f;
}

} //namespace terminus

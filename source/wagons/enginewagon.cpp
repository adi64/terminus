#include "enginewagon.h"

#include <QDebug>
#include <QMatrix4x4>
#include <QVector3D>

#include "../scene.h"
#include "../resources/resourcemanager.h"
#include "../resources/geometry.h"
#include "../resources/material.h"
#include "../resources/program.h"

namespace terminus
{

EngineWagon::EngineWagon(Scene *scene, Train *train)
: AbstractWagon(scene, train)
{
}

void EngineWagon::render(QOpenGLFunctions& gl, int elapsedMilliseconds)
{
    m_position = position();
    QMatrix4x4 model;
    model.setToIdentity();
    model.translate(m_position);

    Program & program = **(ResourceManager::getInstance()->getProgram("basicShader"));
    //Material & material = **(ResourceManager::getInstance()->getMaterial("base_Orange"));
    //Geometry & geometry = **(ResourceManager::getInstance()->getGeometry("base_Icosahedron"));

    program.bind();

    m_scene->camera().setMatrices(program, model);
    //material.setUniforms(program);
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
    //geometry.setAttributes(program);

    //geometry.draw(gl);

    program.release();
}

float EngineWagon::length() const
{
    return 2.0f;
}

}

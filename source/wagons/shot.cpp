#include "shot.h"

#include <QMatrix4x4>
#include <QDebug>
#include <QVector3D>

#include "../scene.h"
#include "../resources/resourcemanager.h"
#include "../resources/soundmanager.h"
#include "../resources/geometry.h"
#include "../resources/material.h"
#include "../resources/program.h"

namespace terminus
{

Shot::Shot(Scene * scene, QVector3D position)
: AbstractGraphicsObject(scene)
{
    m_position = position + QVector3D(0.0, 0.0, 0.0);
    qDebug() << "shot constructed !!!";
}

void Shot::render(QOpenGLFunctions& gl, int elapsedMilliseconds)
{
    QMatrix4x4 model;
    model.setToIdentity();
    model.translate(m_position);
    m_position = position() + QVector3D(0.0, 0.0, 1.0);
    model.scale(0.3);

    Program & program = **(ResourceManager::getInstance()->getProgram("basicShader"));
    Material & material = **(ResourceManager::getInstance()->getMaterial("base_Red"));
    Geometry & geometry_2 = **(ResourceManager::getInstance()->getGeometry("base_Icosahedron"));

    program.bind();

    m_scene->camera().setMatrices(program, model);
    material.setUniforms(program);
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
    geometry_2.setAttributes(program);

    geometry_2.draw(gl);

    qDebug() << "shot render() !!!";

    program.release();
}

}

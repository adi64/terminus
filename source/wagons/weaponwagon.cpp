#include "weaponwagon.h"

#include <QDebug>
#include <QMatrix4x4>

#include "../scene.h"
#include "../resources/resourcemanager.h"
#include "../resources/geometry.h"
#include "../resources/material.h"
#include "../resources/program.h"
#include "shot.h"

namespace terminus
{

WeaponWagon::WeaponWagon(Scene *scene, Train *train)
: AbstractWagon(scene, train)
, m_shots()
{
}

void WeaponWagon::render(QOpenGLFunctions& gl, int elapsedMilliseconds)
{
    m_position = position();
    QMatrix4x4 model;
    model.setToIdentity();
    model.translate(m_position);

    Program & program = **(ResourceManager::getInstance()->getProgram("basicShader"));
    Material & material = **(ResourceManager::getInstance()->getMaterial("base_Blue"));
    Geometry & geometry_1 = **(ResourceManager::getInstance()->getGeometry("base_Wagon"));

    program.bind();

    m_scene->camera().setMatrices(program, model);
    material.setUniforms(program);
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
    geometry_1.setAttributes(program);

    geometry_1.draw(gl);

    program.release();

    for(auto shot : m_shots)
    {
        shot->render(gl, elapsedMilliseconds);
    }
}

float WeaponWagon::length() const
{
    return 5.0f;
}

void WeaponWagon::shoot()
{
    if(m_shots.size() > 10)
    {
        for(auto shot : m_shots)
        {
            delete shot;
        }
        m_shots.clear();
    }

    m_shots.push_back(new Shot(m_scene, QVector3D(m_position + QVector3D(0.0, 0.0, 1.0))));
}

} //namespace terminus

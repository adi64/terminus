#include "terrain.h"

#include <QDebug>

#include "track.h"
#include "scene.h"
#include "resources/resourcemanager.h"
#include "resources/geometry.h"
#include "resources/material.h"
#include "resources/program.h"

namespace terminus
{

Terrain::Terrain(Scene *scene)
    : AbstractGraphicsObject(scene)
    , m_playerTrack(std::unique_ptr<Track>(new Track(scene, QVector3D(0.0, 1.0, 0.0), QVector3D(500.0, 1.0, 0.0))))
    , m_enemyTrack(std::unique_ptr<Track>(new Track(scene, QVector3D(0.0, 1.0, 20.0), QVector3D(500.0, 1.0, 20.0))))
{   

}

Terrain::~Terrain()
{
}

Track *Terrain::playerTrack() const
{
    return m_playerTrack.get();
}

Track *Terrain::enemyTrack() const
{
    return m_enemyTrack.get();
}

void Terrain::render(QOpenGLFunctions& gl, int elapsedMilliseconds)
{

    // render tracks
    m_playerTrack->render(gl, elapsedMilliseconds);
    m_enemyTrack->render(gl, elapsedMilliseconds);

    // render terrain
    QMatrix4x4 model;
    model.setToIdentity();

    Program & program = **(ResourceManager::getInstance()->getProgram("basicShader"));
    Material & material = **(ResourceManager::getInstance()->getMaterial("base_Green"));
    Geometry & geometry = **(ResourceManager::getInstance()->getGeometry("base_Plane"));

    program.bind();

    m_scene->camera().setMatrices(program, model);
    material.setUniforms(program);
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
    geometry.setAttributes(program);

    geometry.draw(gl);

    program.release();
}

} //namespace terminus

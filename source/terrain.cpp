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
    , m_playerTrack(std::unique_ptr<Track>(new Track(scene, QVector3D(-15.0, 1.0, -10.0), QVector3D(100.0, 1.0, -10.0))))
    , m_enemyTrack(std::unique_ptr<Track>(new Track(scene, QVector3D(-15.0, 1.0, 10.0), QVector3D(100.0, 1.0, 10.0))))
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

void Terrain::update(int elapsedMilliseconds)
{
    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(5.0, 0.0, 0.0);
    m_modelMatrix.scale(2.3);

    // update tracks
    m_playerTrack->update(elapsedMilliseconds);
    m_enemyTrack->update(elapsedMilliseconds);
}

void Terrain::render(QOpenGLFunctions& gl) const
{
    // render terrain
    Program & program = **(ResourceManager::getInstance()->getProgram("basicShader"));
    Material & material = **(ResourceManager::getInstance()->getMaterial("base_Green"));
    Geometry & geometry = **(ResourceManager::getInstance()->getGeometry("terrain_Terrain"));

    program.bind();

    m_scene->camera().setMatrices(program, m_modelMatrix);
    material.setUniforms(program);
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
    geometry.setAttributes(program);

    geometry.draw(gl);

    program.release();

    // render tracks
    m_playerTrack->render(gl);
    m_enemyTrack->render(gl);
}

} //namespace terminus

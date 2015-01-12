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
    m_levelGen.generateLevel();
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
    Program & program = **(ResourceManager::getInstance()->getProgram("terrain"));
    Material & material = **(ResourceManager::getInstance()->getMaterial("base_Green"));
    Geometry & geometry = **(ResourceManager::getInstance()->getGeometry("terrain_patch"));

    for(int iX = 0; iX < m_levelGen.patchCountS(); iX++)
    {
        for(int iZ = 0; iZ < m_levelGen.patchCountT(); iZ++)
        {
            program.bind();

            material.setUniforms(program);
            program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));

            m_scene->camera().setMatrices(program, m_modelMatrix);

            program.setUniform("levelMap", 0);
            QVector4D texInfo(static_cast<float>(iX * (m_levelGen.vertexCountS() - 1)),
                              static_cast<float>(iZ * (m_levelGen.vertexCountT() - 1)),
                              static_cast<float>(m_levelGen.totalVertexCountS()),
                              static_cast<float>(m_levelGen.totalVertexCountT()));
            QVector4D posInfo(m_levelGen.vertexWidth(),
                              m_levelGen.vertexHeight(),
                              iX * m_levelGen.patchWidth(),
                              iZ * m_levelGen.patchHeight());
            program.setUniform("texInfo", texInfo);
            program.setUniform("posInfo", posInfo);

            m_levelGen.bindLevelTexTo(gl, GL_TEXTURE0);
            geometry.setAttributes(program);//it seems that setUniform()-calls fail after this point...
            geometry.draw(gl);
            m_levelGen.releaseLevelTexFrom(gl, GL_TEXTURE0);

            program.release();
        }
    }
    // render tracks
    m_playerTrack->render(gl);
    m_enemyTrack->render(gl);
}

} //namespace terminus

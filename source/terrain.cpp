#include "terrain.h"

#include <QOpenGLShaderProgram>
#include <QDebug>

#include "track.h"
#include "scene.h"
#include "resources/resourcemanager.h"
#include "resources/geometry.h"

namespace terminus
{

Terrain::Terrain(Scene *scene)
    : AbstractGraphicsObject(scene)
    , m_playerTrack(std::unique_ptr<Track>(new Track(scene, QVector3D(0.0, 1.0, 0.0), QVector3D(500.0, 1.0, 0.0))))
    , m_enemyTrack(std::unique_ptr<Track>(new Track(scene, QVector3D(0.0, 1.0, 20.0), QVector3D(500.0, 1.0, 20.0))))
    , m_program(nullptr)
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

    if (!m_program)
    {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "data/terrain.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "data/terrain.frag");

        m_program->link();
    }

    m_program->bind();

    QMatrix4x4 model;
    model.setToIdentity();

    QMatrix4x4 modelViewProjection;
    modelViewProjection.setToIdentity();
    modelViewProjection = m_scene->camera().viewProjection() * model;

    m_program->setUniformValue("mvp", modelViewProjection);

    std::shared_ptr<std::unique_ptr<Geometry>> plane = ResourceManager::getInstance()->getGeometry("base_Plane");
    (**plane).setAttributes(*m_program);
    (**plane).draw(gl);

    m_program->release();
}

} //namespace terminus

#include "terrain.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QDebug>

#include "track.h"
#include "scene.h"

namespace terminus
{

Terrain::Terrain(Scene *scene)
    : AbstractGraphicsObject(scene)
    , m_playerTrack(std::unique_ptr<Track>(new Track(scene, QVector3D(0.0, 1.0, 0.0), QVector3D(500.0, 1.0, 0.0))))
    , m_enemyTrack(std::unique_ptr<Track>(new Track(scene, QVector3D(0.0, 1.0, 20.0), QVector3D(500.0, 1.0, 20.0))))
    , m_program(nullptr)
    , m_initialized(false)
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


const std::vector<QVector3D> Terrain::vertices()
{
    std::vector<QVector3D> data;
    data.push_back(QVector3D(-100.0f, 0.0f, -100.0f)); //0
    data.push_back(QVector3D(-100.0f, 0.0f,  100.0f)); //1
    data.push_back(QVector3D( 100.0f, 0.0f, -100.0f)); //2
    data.push_back(QVector3D( 100.0f, 0.0f,  100.0f)); //3

    return data;
}

const std::vector<unsigned short> Terrain::indices()
{
    std::vector<unsigned short> data;
    data.push_back(0);
    data.push_back(2);
    data.push_back(1);
    data.push_back(3);
    return data;
}

void Terrain::initTerrain(QOpenGLFunctions& gl)
{
    std::vector<QVector3D> v(vertices());
    std::vector<unsigned short> i(indices());

    m_vao = new QOpenGLVertexArrayObject();
    m_vao->create();
    m_vao->bind();

    m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vbo->create();
    m_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);

    m_vbo->bind();
    m_vbo->allocate(v.data(), v.size() * sizeof(QVector3D));

    auto vertexAttribLocation = m_program->attributeLocation("a_vertex");
    gl.glVertexAttribPointer(vertexAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    gl.glEnableVertexAttribArray(vertexAttribLocation);

    m_ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_ibo->create();
    m_ibo->setUsagePattern(QOpenGLBuffer::StaticDraw);

    m_ibo->bind();
    m_ibo->allocate(i.data(), i.size() * sizeof(unsigned short));

    m_vao->release();
    m_vbo->release();
    m_ibo->release();

    m_initialized = true;
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

    if(!m_initialized)
        initTerrain(gl);

    m_vao->bind();

    gl.glDrawElements(GL_TRIANGLE_STRIP, indices().size(), GL_UNSIGNED_SHORT, nullptr);

    m_vao->release();

    m_program->release();
}

}

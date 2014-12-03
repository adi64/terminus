#include "enginewagon.h"

#include <glm/gtc/type_ptr.hpp>

#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "../scene.h"

namespace terminus
{

EngineWagon::EngineWagon(Scene *scene)
    : AbstractWagon(scene)
    , m_program(nullptr)
{
}



void EngineWagon::initCube(QOpenGLFunctions& gl)
{

    static bool bufferObjectsInitialized = false;
    if(bufferObjectsInitialized)
    {
       return;
    }

    std::vector<glm::vec3> v(vertices());
    std::vector<unsigned short> i(indices());

    m_vao = new QOpenGLVertexArrayObject();
    m_vao->create();
    m_vao->bind();

    m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vbo->create();
    m_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);

    m_vbo->bind();
    //m_vbo->allocate(v.data(), v.size() * sizeof(glm::vec3));
    m_vbo->allocate(v.data(), v.size() * sizeof(float) * 3);

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

    bufferObjectsInitialized = true;
}

const std::vector<glm::vec3> EngineWagon::vertices()
{
    std::vector<glm::vec3> data;
    data.push_back(glm::vec3(-3.0f, -1.0f, -1.0f)); //0
    data.push_back(glm::vec3(-3.0f, -1.0f,  1.0f)); //1
    data.push_back(glm::vec3(-3.0f,  1.0f, -1.0f)); //2
    data.push_back(glm::vec3(-3.0f,  1.0f,  1.0f)); //3
    data.push_back(glm::vec3( 3.0f, -1.0f, -1.0f)); //4
    data.push_back(glm::vec3( 3.0f, -1.0f,  1.0f)); //5
    data.push_back(glm::vec3( 3.0f,  1.0f, -1.0f)); //6
    data.push_back(glm::vec3( 3.0f,  1.0f,  1.0f)); //7

    return data;
}

const std::vector<unsigned short> EngineWagon::indices()
{
    std::vector<unsigned short> data;
    data.push_back(2);
    data.push_back(0);
    data.push_back(6);
    data.push_back(4);
    data.push_back(5);
    data.push_back(0);
    data.push_back(1);
    data.push_back(2);
    data.push_back(3);
    data.push_back(6);
    data.push_back(7);
    data.push_back(5);
    data.push_back(3);
    data.push_back(1);

    return data;
}


void EngineWagon::render(QOpenGLFunctions& gl)
{
    if (!m_program)
    {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "data/enginewagon.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "data/enginewagon.frag");

        m_program->link();
    }

    m_program->bind();

    static float timer = 0.0;
    if(timer >= 360.0)
    {
        timer = 0.0;
    }
    timer += 1.0;

    QMatrix4x4 model;
    model.setToIdentity();
    model.translate(QVector3D(-1.8, 0.0, 0.0));
    model.translate(QVector3D(0.9 + abs(180 - timer)/100.0, 0.0, 0.0));
    model.rotate(timer, QVector3D(1.0, 0.0, 0.0));
    model.rotate(timer * 2.0, QVector3D(0.0, 1.0, 0.0));

    QMatrix4x4 modelViewProjection;
    modelViewProjection.setToIdentity();
    modelViewProjection = m_scene->camera().viewProjection() * model;

    m_program->setUniformValue("mvp", modelViewProjection);

    initCube(gl);

    m_vao->bind();

    gl.glDrawElements(GL_TRIANGLE_STRIP, indices().size(), GL_UNSIGNED_SHORT, nullptr);

    m_vao->release();

    m_program->release();
}

float EngineWagon::length()
{
    //TODO change value later
    return 1.5f;
}

}

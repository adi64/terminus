#include "enginewagon.h"

#include <glm/gtc/type_ptr.hpp>

#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

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
    static const GLfloat vertices[24] =
    {
        -1.f, -1.f, -1.f, //0
        -1.f, -1.f,  1.f, //1
        -1.f,  1.f, -1.f, //2
        -1.f,  1.f,  1.f, //3
         1.f, -1.f, -1.f, //4
         1.f, -1.f,  1.f, //5
         1.f,  1.f, -1.f, //6
         1.f,  1.f,  1.f  //7
    };

    static const GLubyte indices[14] =
    {
        2, 0, 6, 4, 5, 0, 1, 2, 3, 6, 7, 5, 3, 1
    };

    static bool bufferObjectsInitialized = false;
    if(bufferObjectsInitialized)
    {
       return;
    }

    m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vbo->create();
    m_vbo->bind();
    m_vbo->allocate(vertices, 24 * sizeof(GLfloat));

    auto vertexAttribLocation = m_program->attributeLocation("a_vertex");
    gl.glVertexAttribPointer(vertexAttribLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    gl.glEnableVertexAttribArray(vertexAttribLocation);

    m_ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_ibo->create();
    m_ibo->bind();
    m_ibo->allocate(indices, 14 * sizeof(GLfloat));

    m_ibo->release();
    m_vbo->release();

    bufferObjectsInitialized = true;
}


void EngineWagon::render(QOpenGLFunctions& gl)
{
    qDebug() << "Trying to render EngineWagon";

    if (!m_program) {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "data/enginewagon.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "data/enginewagon.frag");

        m_program->link();
    }

    m_program->bind();

    auto mvpLocation = m_program->uniformLocation("mvp");
    auto mvp = QMatrix4x4(glm::value_ptr(m_scene->camera().viewProjection()));
    m_program->setUniformValue(mvpLocation, mvp);

    initCube(gl);

    m_vbo->bind();
    m_ibo->bind();

    glClearColor(0.5f, 0.55f, 0.6f, 1.0f);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 34);

    m_program->disableAttributeArray(0);
    m_program->release();
    m_vbo->release();
    m_ibo->release();
}

float EngineWagon::length()
{
    //TODO change value later
    return 1.5f;
}

}

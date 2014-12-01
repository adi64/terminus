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


void EngineWagon::initCube()
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

    m_vbo->allocate(vertices, 24 * sizeof(float));

    int a_vertex = m_program->attributeLocation("a_vertex");

    m_program->enableAttributeArray(a_vertex);
    m_program->setAttributeBuffer(a_vertex, GL_FLOAT, 0, 3);

    m_ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_ibo->create();
    m_ibo->bind();

    m_ibo->allocate(indices, 14 * sizeof(float));

    bufferObjectsInitialized = true;

}


void EngineWagon::render()
{
    qDebug() << "Trying to render EngineWagon";

    if (!m_program) {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "data/enginewagon.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "data/enginewagon.frag");

        m_program->link();
    }

    int mvp = m_program->uniformLocation("mvp");
    glm::mat4 glm_mvp =  m_scene->camera().viewProjection();
    QMatrix4x4 matrix(glm::value_ptr(glm_mvp));
    m_program->setUniformValue(mvp, matrix);

    initCube();

    m_vbo->bind();
    m_ibo->bind();

    m_program->bind();

    glClearColor(0.5f, 0.55f, 0.6f, 1.0f);

    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);

    m_program->disableAttributeArray(0);
    m_program->release();
}

float EngineWagon::length()
{
    //TODO change value later
    return 1.5f;
}

}

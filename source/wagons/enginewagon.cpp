#include "enginewagon.h"

#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

namespace terminus
{

EngineWagon::EngineWagon()
    : m_program(nullptr)
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

//    m_vbo = new GLuint[2];

//    glGenBuffers(2, m_vbo);

//    //VertexBuffer
//    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
//    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

//    //IndexBuffer
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 34 * sizeof(GLushort), indices, GL_STATIC_DRAW);

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


void EngineWagon::render(/*Camera* camera*/)
{

    qDebug() << "render EngineWagon";

    if (!m_program) {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "data/enginewagon.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "data/enginewagon.frag");

        m_program->link();
    }
    m_program->bind();


    initCube();

    //m_program->enableAttributeArray(a_vertex);
    //glVertexAttribPointer(a_vertex, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.5f, 0.55f, 0.6f, 1.0f);

    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);

    m_program->disableAttributeArray(0);
    m_program->release();

    glEnable(GL_DEPTH_TEST);

}

float EngineWagon::length()
{
    //TODO change value later
    return 1.5f;
}

}

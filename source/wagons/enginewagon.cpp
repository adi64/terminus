#include "enginewagon.h"

#include <QDebug>

namespace terminus
{

EngineWagon::EngineWagon()
{
}

/*
void drawCube()
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

    glGenBuffers(2, m_vbo);

    //VertexBuffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    //IndexBuffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 34 * sizeof(GLushort), indices, GL_STATIC_DRAW);

}
*/

void EngineWagon::render(/*Camera* camera*/)
{
/*    if (!m_program) {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "../data/enginewagon.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "../data/enginewagon.frag");

        m_program->link();
    }
    m_program->bind();


    int a_vertex = m_program->attributeLocation("a_vertex");

    m_program->enableAttributeArray(a_vertex);
    glVertexAttribPointer(a_vertex, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.5f, 0.55f, 0.6f, 1.0f);

    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);

    m_program->disableAttributeArray(0);
    m_program->release();

    glEnable(GL_DEPTH_TEST);
    */
}

float EngineWagon::length()
{
    //TODO change value later
    return 1.5f;
}

}

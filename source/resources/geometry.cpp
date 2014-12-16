#include "geometry.h"

#include <string>

#include <QDebug>
#include <QOpenGLShaderProgram>

#include "indextriple.h"

namespace terminus
{

Geometry::Geometry()
: m_isOnGPU(false)
//, m_vao(nullptr)
, m_vbo(nullptr)
, m_ibo(nullptr)
{
}

Geometry::Geometry(const std::vector<unsigned short> & indexBuffer, const std::vector<Vertex> & vertexBuffer)
: m_isOnGPU(false)
, m_vbo(nullptr)
, m_ibo(nullptr)
, m_vertexBuffer(vertexBuffer)
, m_indexBuffer(indexBuffer)
//, m_vao(nullptr)

{
    m_elementCount = m_indexBuffer.size();
}

Geometry::~Geometry()
{
    deallocate();
}

void Geometry::allocate()
{
     if(m_isOnGPU)
         return;

/*     m_vao = new QOpenGLVertexArrayObject();
     m_vao->create();
     m_vao->bind();*/

     m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
     m_vbo->create();
     m_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
     m_vbo->bind();
     m_vbo->allocate(m_vertexBuffer.data(), m_vertexBuffer.size() * sizeof(Vertex));

     m_ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
     m_ibo->create();
     m_ibo->setUsagePattern(QOpenGLBuffer::StaticDraw);
     m_ibo->bind();
     m_ibo->allocate(m_indexBuffer.data(), m_indexBuffer.size() * sizeof(unsigned short));

     m_vbo->release();
     m_ibo->release();

//     m_vao->release();
     m_isOnGPU = true;
}

void Geometry::deallocate()
{
    if(!m_isOnGPU)
        return;

    if(m_vbo)
    {
        m_vbo->destroy();
        delete m_vbo;
        m_vbo = nullptr;
    }
    if(m_ibo)
    {
        m_ibo->destroy();
        delete m_ibo;
        m_ibo = nullptr;
    }
    //deallocate m_vao
    m_isOnGPU = false;
}

void Geometry::setAttributes(Program & program)
{
    program.program().bindAttributeLocation("a_vertex", 0);
    program.program().bindAttributeLocation("a_texCoord", 1);
    program.program().bindAttributeLocation("a_normal", 2);
}

void Geometry::draw(QOpenGLFunctions & gl)
{
    allocate();

    m_vbo->bind();
    m_ibo->bind();

    gl.glEnableVertexAttribArray(0); // positions
    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
    gl.glEnableVertexAttribArray(1); // texture coordinates
    gl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(GLfloat[3])));
    gl.glEnableVertexAttribArray(2); // normals
    gl.glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(GLfloat[3]) * 2));

    gl.glDrawElements(GL_TRIANGLES, m_elementCount, GL_UNSIGNED_SHORT, nullptr);
}

}//namespace terminus

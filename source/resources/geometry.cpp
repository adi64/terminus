#include "geometry.h"

#include <string>
#include <math.h>

#include <QDebug>
#include <QOpenGLShaderProgram>

#include "indextriple.h"

namespace terminus
{

Geometry::Geometry()
: m_isOnGPU(false)
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
{
    m_elementCount = m_indexBuffer.size();
    calculateBBox();
}

Geometry::Geometry(const std::vector<unsigned short> & indexBuffer, const std::vector<Vertex> & vertexBuffer, const QVector3D & minBBox, const QVector3D & maxBBox)
: m_isOnGPU(false)
, m_vbo(nullptr)
, m_ibo(nullptr)
, m_vertexBuffer(vertexBuffer)
, m_indexBuffer(indexBuffer)
, m_bBoxMin(minBBox)
, m_bBoxMax(maxBBox)
{
    m_elementCount = m_indexBuffer.size();
}

Geometry::~Geometry()
{
    deallocate();
}

void Geometry::calculateBBox()
{
    for(auto iVert = m_vertexBuffer.begin(); iVert != m_vertexBuffer.end(); iVert++)
    {
        m_bBoxMax.setX(fmax(iVert->position[0], m_bBoxMax.x()));
        m_bBoxMax.setY(fmax(iVert->position[1], m_bBoxMax.y()));
        m_bBoxMax.setZ(fmax(iVert->position[2], m_bBoxMax.z()));
        m_bBoxMin.setX(fmin(iVert->position[0], m_bBoxMin.x()));
        m_bBoxMin.setY(fmin(iVert->position[1], m_bBoxMin.y()));
        m_bBoxMin.setZ(fmin(iVert->position[2], m_bBoxMin.z()));
    }
}

void Geometry::allocate() const
{
     if(m_isOnGPU)
         return;

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

     m_isOnGPU = true;
}

void Geometry::deallocate() const
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
    m_isOnGPU = false;
}

QVector3D Geometry::bBoxMin() const
{
    return m_bBoxMin;
}

QVector3D Geometry::bBoxMax() const
{
    return m_bBoxMax;
}

void Geometry::setAttributes(Program & program)
{
    program.program().bindAttributeLocation("a_vertex", 0);
    program.program().bindAttributeLocation("a_texCoord", 1);
    program.program().bindAttributeLocation("a_normal", 2);
}

void Geometry::draw(QOpenGLFunctions & gl) const
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

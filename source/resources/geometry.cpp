#include "geometry.h"

#include <string>
#include <math.h>

#include "util/gldebug.h"

#include "indextriple.h"

namespace terminus
{

Geometry::Geometry(const std::vector<unsigned short> & indexBuffer, const std::vector<Vertex> & vertexBuffer)
: m_isOnGPU(false)
, m_vbo(0)
, m_ibo(0)
, m_vertexBuffer(vertexBuffer)
, m_indexBuffer(indexBuffer)
{
    m_elementCount = m_indexBuffer.size();
    calculateBBox();
}

Geometry::Geometry(const std::vector<unsigned short> & indexBuffer, const std::vector<Vertex> & vertexBuffer, const QVector3D & minBBox, const QVector3D & maxBBox)
: m_isOnGPU(false)
, m_vbo(0)
, m_ibo(0)
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

     printGlError(__FILE__, __LINE__);

     //setup VertexBufferObject
     glGenBuffers(1, &m_vbo);
     glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
     glBufferData(GL_ARRAY_BUFFER, m_vertexBuffer.size() * sizeof(Vertex), m_vertexBuffer.data(), GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glGenBuffers(1, &m_ibo);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer.size() * sizeof(unsigned short), m_indexBuffer.data(), GL_STATIC_DRAW);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

     //TODO-LW: Add error checks
     printGlError(__FILE__, __LINE__);

     m_isOnGPU = true;
}

void Geometry::deallocate() const
{
    if(!m_isOnGPU)
        return;

    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);

    m_isOnGPU = false;
}

const QVector3D & Geometry::bBoxMin() const
{
    return m_bBoxMin;
}

const QVector3D & Geometry::bBoxMax() const
{
    return m_bBoxMax;
}

void Geometry::draw() const
{
    allocate();
    printGlError(__FILE__, __LINE__);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

    glEnableVertexAttribArray(0); // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1); // texture coordinates
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(GLfloat[3])));
    glEnableVertexAttribArray(2); // normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(GLfloat[3]) * 2));

    glDrawElements(GL_TRIANGLES, m_elementCount, GL_UNSIGNED_SHORT, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    printGlError(__FILE__, __LINE__);
}

}//namespace terminus

#pragma once

#include <string>
#include <vector>

#include <GLES3/gl3.h>

#include <QVector3D>

#include "indextriple.h"
#include "program.h"

namespace terminus
{

/*!
 * \brief The struct wraps the important vertex data attributes position, texture coordinate and normal vector.
 */
struct Vertex{
    GLfloat position[3];
    GLfloat texCoord[3];
    GLfloat normal[3];
};

/*!
 * \brief The Geometry class wraps the vertices data of one drawable object.
 *
 * Vertices data is stored as an index and a vertex buffer. In order to get rendered, a game object has to reference one geometry.
 */
class Geometry
{

public:
    /*!
     * \brief General geometry constructor
     * \param indexBuffer
     * \param vertexBuffer
     *
     * Supplied geometry data will be stored in the respective member.
     * If no BBox values are given, the BBox will be calculated automatically.
     */
    Geometry(const std::vector<unsigned short> & indexBuffer, const std::vector<Vertex> & vertexBuffer);
    Geometry(const std::vector<unsigned short> & indexBuffer, const std::vector<Vertex> & vertexBuffer, const QVector3D & minBBox, const QVector3D & maxBBox);
    virtual ~Geometry();

    virtual const QVector3D & bBoxMin() const;
    virtual const QVector3D & bBoxMax() const;

    /*!
     * \brief The geometry specific attributes position, texture coordinate and normal vector are set on the used program.
     * \param program
     */
    virtual void setAttributes(Program & program);

    /*!
     * \brief By invoking the draw function, both index and vertex buffer are going to be bound and the geometry will be drawn.
     * \param gl
     */
    virtual void draw() const;

protected:
    virtual void calculateBBox();
    virtual void allocate() const;
    virtual void deallocate() const;

    mutable bool m_isOnGPU;
    mutable int m_vbo;
    mutable int m_ibo;

    unsigned int m_elementCount;

    std::vector<Vertex> m_vertexBuffer;
    std::vector<unsigned short> m_indexBuffer;
    QVector3D m_bBoxMin;
    QVector3D m_bBoxMax;
};

} //terminus

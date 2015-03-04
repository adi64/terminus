#pragma once

#include <string>
#include <vector>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QVector3D>

#include "indextriple.h"
#include "program.h"

namespace terminus
{

struct Vertex{
    GLfloat position[3];
    GLfloat texCoord[3];
    GLfloat normal[3];
};

class Geometry
{

public:
    Geometry(); //construct NULL-Object --- what do we need this for?
    Geometry(const std::vector<unsigned short> & indexBuffer, const std::vector<Vertex> & vertexBuffer);
    Geometry(const std::vector<unsigned short> & indexBuffer, const std::vector<Vertex> & vertexBuffer, const QVector3D & minBBox, const QVector3D & maxBBox);
    virtual ~Geometry();

    virtual const QVector3D & bBoxMin() const;
    virtual const QVector3D & bBoxMax() const;

    virtual void setAttributes(Program & program);
    virtual void draw(QOpenGLFunctions & gl) const;

protected:
    virtual void calculateBBox();
    virtual void allocate() const;
    virtual void deallocate() const;

    mutable bool m_isOnGPU;
    mutable QOpenGLBuffer *m_vbo;
    mutable QOpenGLBuffer *m_ibo;

    unsigned int m_elementCount;

    std::vector<Vertex> m_vertexBuffer;
    std::vector<unsigned short> m_indexBuffer;
    QVector3D m_bBoxMin;
    QVector3D m_bBoxMax;
};

} //terminus

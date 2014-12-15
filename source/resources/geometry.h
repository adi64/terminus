#pragma once

#include <string>
#include <vector>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

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
    virtual ~Geometry();

    virtual void allocate();
    virtual void deallocate();

    virtual void setAttributes(Program & program);
    virtual void draw(QOpenGLFunctions & gl);

protected:
    bool m_isOnGPU;
    //QOpenGLVertexArrayObject * m_vao;
    QOpenGLBuffer * m_vbo;
    QOpenGLBuffer * m_ibo;

    unsigned int m_elementCount;

    std::vector<Vertex> m_vertexBuffer;
    std::vector<unsigned short> m_indexBuffer;
};

} //terminus

#pragma once

#include "abstractgraphicsobject.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class QOpenGLFunctions;

namespace terminus
{

class ScreenAlignedQuad : public AbstractGraphicsObject
{
public:
    ScreenAlignedQuad(Scene *scene);

    void render(QOpenGLFunctions & gl, int elapsedMilliseconds);

protected:
    void initialize(QOpenGLFunctions &gl);
	static void strip(QOpenGLBuffer & vertices);

private:
    bool m_initialized;
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vertices;
};

}

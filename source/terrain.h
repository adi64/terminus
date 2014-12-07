#pragma once

#include <memory>

#include "abstractgraphicsobject.h"

class QOpenGLShaderProgram;
class QOpenGLBuffer;
class QOpenGLVertexArrayObject;

namespace terminus
{

class Track;

class Terrain : public AbstractGraphicsObject
{
public:
    Terrain(Scene *scene);
    ~Terrain();

    void render(QOpenGLFunctions& gl, int elapsedMilliseconds);

    Track *playerTrack() const;
    Track *enemyTrack() const;

protected:
    void initCube(QOpenGLFunctions& gl);
    static const std::vector<QVector3D> vertices();
    static const std::vector<unsigned short> indices();

    std::unique_ptr<Track> m_playerTrack;
    std::unique_ptr<Track> m_enemyTrack;

    QOpenGLShaderProgram *m_program;
    bool m_initialized;
    QOpenGLBuffer *m_vbo;
    QOpenGLBuffer *m_ibo;
    QOpenGLVertexArrayObject *m_vao;
};

}

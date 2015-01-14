#pragma once

#include <memory>

#include <QOpenGLTexture>


#include "abstractgraphicsobject.h"
#include "level.h"

namespace terminus
{

class Track;

class Terrain : public AbstractGraphicsObject
{
public:
    Terrain(Scene *scene);
    ~Terrain();

    void update(int elapsedMilliseconds);
    void render(QOpenGLFunctions& gl) const override;

    Track *playerTrack() const;
    Track *enemyTrack() const;

protected:
    void renderPatch(QOpenGLFunctions& gl, int iX, int iZ) const;

    void allocateTerrainMap(QOpenGLFunctions & gl) const;
    void deallocateTerrainMap(QOpenGLFunctions & gl) const;

protected:
    Level m_level;

    std::unique_ptr<Track> m_playerTrack;
    std::unique_ptr<Track> m_enemyTrack;

    mutable bool m_terrainMapOnGPU;
    mutable GLuint m_terrainMap;
};

}

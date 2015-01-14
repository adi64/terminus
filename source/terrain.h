#pragma once

#include <memory>

#include "abstractgraphicsobject.h"
#include "kinematicphysicsobject.h"
#include "track.h"
#include "level.h"

namespace terminus
{

class Track;

class Terrain : public KinematicPhysicsObject
{
public:
    Terrain(std::shared_ptr<Scene> scene);
    ~Terrain();

    void update(int elapsedMilliseconds) override;
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

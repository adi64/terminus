#pragma once

#include <memory>

#include <world/physics/kinematicphysicsobject.h>
#include "abstractgraphicsobject.h"
#include "track.h"
#include "level.h"

namespace terminus
{

class Track;

class Terrain : public KinematicPhysicsObject
{
public:
    Terrain(std::shared_ptr<Scene> scene);
    virtual ~Terrain();

    virtual void update(int elapsedMilliseconds) override;
    virtual void render(QOpenGLFunctions& gl) const override;
    virtual void preRender(QOpenGLFunctions & gl, Program & program) const override;
    virtual void postRender(QOpenGLFunctions & gl, Program & program) const override;

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

    mutable int m_currentPatchX;
    mutable int m_currentPatchZ;
};

}

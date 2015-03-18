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
    Terrain(World & world);
    virtual ~Terrain();

    virtual void localUpdate() override;
    virtual void localRender(QOpenGLFunctions& gl) const override;
    virtual void localRenderSetup(QOpenGLFunctions & gl, Program & program) const override;
    virtual void localRenderCleanup(QOpenGLFunctions & gl, Program & program) const override;

    void debug();

    Track *rightTrack() const;
    Track *leftTrack() const;

protected:
    virtual void doForAllChildren(std::function<void(AbstractGraphicsObject &)> callback) override;
    void renderPatch(QOpenGLFunctions& gl, int iX, int iZ) const;

    void allocateTerrainMap(QOpenGLFunctions & gl) const;
    void deallocateTerrainMap(QOpenGLFunctions & gl) const;

    virtual short myCollisionType() const override;
    virtual short possibleCollisionTypes() const override;

protected:
    Level m_level;

    std::unique_ptr<Track> m_rightTrack;
    std::unique_ptr<Track> m_leftTrack;

    mutable bool m_terrainMapOnGPU;
    mutable GLuint m_terrainMap;

    mutable int m_currentPatchX;
    mutable int m_currentPatchZ;

    mutable bool m_debugChanged;
};

}

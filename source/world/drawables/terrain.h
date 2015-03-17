#pragma once

#include <memory>

#include <world/drawables/abstractgraphicsobject.h>
#include <world/drawables/track.h>
#include <world/levelconfiguration.h>

namespace terminus
{

class Level;

class Terrain : public AbstractGraphicsObject
{
public:
    Terrain(World & world, const Level & level);

    virtual void localRender(QOpenGLFunctions& gl) const override;
    virtual void localRenderSetup(QOpenGLFunctions & gl, Program & program) const override;
    virtual void localRenderCleanup(QOpenGLFunctions & gl, Program & program) const override;

    void configureWith(const Level & level);

    Track *rightTrack() const;
    Track *leftTrack() const;

protected:
    virtual void doForAllChildren(std::function<void(AbstractGraphicsObject &)> callback) override;
    void renderPatch(QOpenGLFunctions& gl, int iX, int iZ) const;

    void allocateTerrainMap(QOpenGLFunctions & gl) const;
    void deallocateTerrainMap(QOpenGLFunctions & gl) const;

protected:
    LevelConfiguration m_levelConfig;

    std::unique_ptr<Track> m_rightTrack;
    std::unique_ptr<Track> m_leftTrack;

    std::unique_ptr<std::vector<GLfloat>> m_terrainMapData;
    mutable bool m_terrainMapOnGPU;
    mutable bool m_terrainMapValid;
    mutable GLuint m_terrainMap;

    mutable int m_currentPatchX;
    mutable int m_currentPatchZ;
};

}

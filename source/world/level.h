#pragma once

#include <vector>
#include <memory>

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QSize>
#include <QVector3D>

#include <util/polyline.h>
#include <util/catmullromspline.h>
#include <util/perlinnoise.h>
#include <world/levelconfiguration.h>

namespace terminus
{

class Level
{
public:
    Level();
    Level(const LevelConfiguration & config);
    virtual ~Level();

    void generateLevel();
    void resetLevel();

    void configure(const LevelConfiguration & config);
    const LevelConfiguration & config() const;

    std::vector<GLfloat> * copyTerrainMapData() const;
    Polyline * createRightTrackCourse() const;
    Polyline * createLeftTrackCourse() const;

protected:
    float trackHeight() const;

    void generateTracks();

    void generateTerrainMap();
    QVector2D terrainDisplacement(float x, float z);
    void setTrackEnvironment(const CatmullRomSpline &track);
    float terrainHeight(float x, float z, float fTrack);

    int tMapIndex(int s, int t) const;
    void tMapSetXYZ(int i, float dx, float dy, float dz);
    QVector3D tMapGetXYZ(int i) const;
    void tMapSetW(int i, float w);
    float tMapGetW(int i) const;

protected:
    LevelConfiguration m_config;

    PerlinNoise m_noise;

    bool m_tracksGenerated;
    float m_trackHeight; // might become a PerlinNoise instance...
    std::unique_ptr<CatmullRomSpline> m_rightTrack;
    std::unique_ptr<CatmullRomSpline> m_leftTrack;

    bool m_texGenerated;
    std::vector<GLfloat> m_terrainMapData;

};

}

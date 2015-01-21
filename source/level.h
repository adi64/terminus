#pragma once

#include <vector>
#include <memory>

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QSize>
#include <QVector3D>

#include "polyline.h"
#include "catmullromspline.h"
#include "perlinnoise.h"

namespace terminus
{

class Level
{
public:
    Level();
    virtual ~Level();

    virtual void generateLevel();

    virtual int vertexCountS() const;
    virtual int vertexCountT() const;
    virtual int patchCountS() const;
    virtual int patchCountT() const;
    virtual int totalVertexCountS() const;
    virtual int totalVertexCountT() const;
    virtual float vertexWidthUnscaled() const;
    virtual float vertexHeightUnscaled() const;
    virtual float patchWidthUnscaled() const;
    virtual float patchHeightUnscaled() const;
    virtual float vertexWidth() const;
    virtual float vertexHeight() const;
    virtual float patchWidth() const;
    virtual float patchHeight() const;
    virtual int totalWidth() const;
    virtual int totalHeight() const;
    virtual float scale() const;
    virtual QPoint positionToVertexID(float x, float z) const;
    virtual QPoint positionToPatchID(float x, float z) const;
    virtual QVector2D vertexIDToPosition(int s, int t) const;

    virtual const void * terrainMapData() const;
    virtual std::unique_ptr<Polyline> playerTrack() const;
    virtual std::unique_ptr<Polyline> enemyTrack() const;

protected:
    virtual float trackHeight() const;

    virtual void generateTracks();

    virtual void generateTerrainMap();
    virtual QVector2D terrainDisplacement(float x, float z);
    virtual void setTrackEnvironment(const CatmullRomSpline &track);
    virtual float terrainHeight(float x, float z, float fTrack);

    virtual int tMapIndex(int s, int t) const;
    virtual void tMapSetXYZ(int i, float dx, float dy, float dz);
    virtual QVector3D tMapGetXYZ(int i) const;
    virtual void tMapSetW(int i, float w);
    virtual float tMapGetW(int i) const;

protected:
    int m_vertexCountS, m_vertexCountT;
    int m_patchCountS, m_patchCountT;
    float m_vertexWidth, m_vertexHeight;
    float m_scale;

    PerlinNoise m_noise;

    bool m_tracksGenerated;
    float m_trackHeight;// might become a PerlinNoise instance...

    bool m_texGenerated;
    std::vector<GLfloat> m_terrainMapData;

    std::unique_ptr<CatmullRomSpline> m_playerTrack;
    std::unique_ptr<CatmullRomSpline> m_enemyTrack;
};

}

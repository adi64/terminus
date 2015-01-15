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

    virtual int patchCountS() const;
    virtual int patchCountT() const;
    virtual int vertexCountS() const;
    virtual int vertexCountT() const;
    virtual float vertexWidth() const;
    virtual float vertexHeight() const;
    virtual float patchWidth() const;
    virtual float patchHeight() const;
    virtual int totalVertexCountS() const;
    virtual int totalVertexCountT() const;
    virtual int totalWidth() const;
    virtual int totalHeight() const;
    virtual QPoint positionToVertexID(float x, float z) const;
    virtual QPoint positionToPatchID(float x, float z) const;
    virtual QVector2D vertexIDToPosition(int s, int t) const;

    virtual const void * terrainMapData() const;
    virtual std::unique_ptr<Polyline> playerTrack();
    virtual std::unique_ptr<Polyline> enemyTrack();

protected:
    virtual float trackHeight();

    virtual void generateTracks();

    virtual void generateTerrainMap();
    virtual QVector2D terrainDisplacement(float x, float z);
    virtual void setTrackEnvironment(const CatmullRomSpline &track);
    virtual float terrainHeight(float x, float z, float fTrack);

//    virtual void setTerrainMap(int s, int t, int component, float value);
//    virtual float getTerrainMap(int s, int t, int component);
    virtual int tMapIndex(int s, int t);
    virtual float tMapEncode(float value);
    virtual float tMapDecode(float value);

protected:
    int m_patchCountS, m_patchCountT;
    int m_vertexCountS, m_vertexCountT;
    float m_vertexWidth, m_vertexHeight;
    int m_totalVertexCountS, m_totalVertexCountT;

    PerlinNoise m_noiseX;
    PerlinNoise m_noiseY;
    PerlinNoise m_noiseZ;

    bool m_tracksGenerated;
    float m_trackHeight;// might become a PerlinNoise instance...
    std::unique_ptr<CatmullRomSpline> m_playerTrack;
    std::unique_ptr<CatmullRomSpline> m_enemyTrack;

    bool m_texGenerated;
    std::vector<GLfloat> m_terrainMapData;
//    mutable bool m_isTexAllocated;
//    mutable GLuint m_levelTex;
};

}

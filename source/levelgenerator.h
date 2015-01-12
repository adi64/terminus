#pragma once

#include <vector>
#include <memory>

#include <QOpenGLFunctions>
#include <QSize>
#include <QVector3D>

#include "catmullromspline.h"
#include "perlinnoise.h"

namespace terminus
{

class LevelGenerator
{
public:
    LevelGenerator();
    virtual ~LevelGenerator();

    virtual void generateLevel();

    virtual bool allocateLevelTex(QOpenGLFunctions & gl) const;
    virtual void deallocateLevelTex(QOpenGLFunctions & gl) const;
    virtual void bindLevelTexTo(QOpenGLFunctions & gl, GLenum unit) const;
    virtual void releaseLevelTexFrom(QOpenGLFunctions & gl, GLenum unit) const;

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

    virtual QPoint positionToVertexID(const QVector2D & pos) const;
    virtual QVector2D vertexIDToPosition(const QPoint & vid) const;

    //getLevelTrack
protected:
    virtual void generateLevelTracks();
    virtual void generateLevelTexImage();
    virtual float terrainHeight(float s, float t, float fTrack);
    virtual void setTrackEnvironment(const QVector2D & pointOnTrack);
    virtual void setTexComponent(int s, int t, int component, float value);
    virtual float getTexComponent(int s, int t, int component);

protected:
    int m_patchCountS, m_patchCountT;
    int m_vertexCountS, m_vertexCountT;
    float m_vertexWidth, m_vertexHeight;

    bool m_texGenerated;
    std::unique_ptr<PerlinNoise> m_noiseX;
    std::unique_ptr<PerlinNoise> m_noiseY;
    std::unique_ptr<PerlinNoise> m_noiseZ;
    int m_totalVertexCountS, m_totalVertexCountT;
    std::vector<GLfloat> m_levelTexData;

    mutable bool m_isTexAllocated;
    mutable GLenum m_levelTex;

    bool m_tracksGenerated;
    std::unique_ptr<CatmullRomSpline> m_trackSpline;

};

}

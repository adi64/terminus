#pragma once

#include <vector>

#include <QOpenGLFunctions>
#include <QSize>

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

    virtual int patchesX() const;
    virtual int patchesZ() const;
    virtual int patchVerticesX() const;
    virtual int patchVerticesZ() const;
    virtual float patchVertLengthX() const;
    virtual float patchVertLengthZ() const;
    virtual float patchLengthX() const;
    virtual float patchLengthZ() const;
    virtual int levelTexWidth() const;
    virtual int levelTexHeight() const;

    //getLevelTrack
protected:
    virtual void generateLevelTracks();
    virtual void generateLevelTexImage();
protected:
    int m_patchesX, m_patchesZ;
    int m_patchVertX, m_patchVertZ;
    float m_patchLenVertX, m_patchLenVertZ;

    bool m_texGenerated;
    int m_levelTexWidth, m_levelTexHeight;
    std::vector<GLfloat> m_levelTexData;

    mutable bool m_isTexAllocated;
    mutable GLenum m_levelTex;

    bool m_tracksGenerated;

};

}

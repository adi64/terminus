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

namespace terminus
{

/*!
 * \brief The LevelConfiguration class contains all information necessary to
 * configure a Level instance as well as some convenience methods to convert
 * between model and texture space coordinates.
 *
 * \sa Level
 */
class LevelConfiguration
{
public:
    LevelConfiguration();
    explicit LevelConfiguration(long long seed);

    virtual long long seed() const;
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

protected:
    long long m_seed;

    int m_vertexCountS;
    int m_vertexCountT;

    int m_patchCountS;
    int m_patchCountT;

    float m_vertexWidth;
    float m_vertexHeight;

    float m_scale;
};

} //namespace terminus

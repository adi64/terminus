#include "levelconfiguration.h"

#include <chrono>
#include <cmath>

namespace terminus
{

LevelConfiguration::LevelConfiguration()
: LevelConfiguration(std::chrono::system_clock().now().time_since_epoch().count())
{
}

LevelConfiguration::LevelConfiguration(long long seed)
: m_seed(seed)
, m_vertexCountS(64)
, m_vertexCountT(73)
, m_patchCountS(32)
, m_patchCountT(2)
, m_vertexWidth(1.f)
, m_vertexHeight(sqrt(3.f)/2.f)
, m_scale(4.f)
{
}

long long LevelConfiguration::seed() const
{
    return m_seed;
}

int LevelConfiguration::vertexCountS() const
{
    return m_vertexCountS;
}
int LevelConfiguration::vertexCountT() const
{
    return m_vertexCountT;
}
int LevelConfiguration::patchCountS() const
{
    return m_patchCountS;
}
int LevelConfiguration::patchCountT() const
{
    return m_patchCountT;
}
int LevelConfiguration::totalVertexCountS() const
{
    return m_patchCountS * (m_vertexCountS - 1) + 1;
}
int LevelConfiguration::totalVertexCountT() const
{
    return m_patchCountT * (m_vertexCountT - 1) + 1;
}
float LevelConfiguration::vertexWidthUnscaled() const
{
    return m_vertexWidth;
}
float LevelConfiguration::vertexHeightUnscaled() const
{
    return m_vertexHeight;
}
float LevelConfiguration::patchWidthUnscaled() const
{
    return (vertexCountS() - 1) * vertexWidthUnscaled();
}
float LevelConfiguration::patchHeightUnscaled() const
{
    return (vertexCountT() - 1) * vertexHeightUnscaled();
}
float LevelConfiguration::vertexWidth() const
{
    return m_vertexWidth * scale();
}
float LevelConfiguration::vertexHeight() const
{
    return m_vertexHeight * scale();
}
float LevelConfiguration::patchWidth() const
{
    return (vertexCountS() - 1) * vertexWidth();
}
float LevelConfiguration::patchHeight() const
{
    return (vertexCountT() - 1) * vertexHeight();
}
int LevelConfiguration::totalWidth() const
{
    return totalVertexCountS() * vertexWidth();
}
int LevelConfiguration::totalHeight() const
{
    return totalVertexCountT() * vertexHeight();
}
float LevelConfiguration::scale() const
{
    return m_scale;
}
QPoint LevelConfiguration::positionToVertexID(float x, float z) const
{
    int t = round(z / vertexHeight());
    int s = round(x / vertexWidth() - (t % 2 == 0 ? 0.f : 0.5f));
    return QPoint(s, t);
}
QPoint LevelConfiguration::positionToPatchID(float x, float z) const
{
    int t = round(z / patchHeight());
    int s = round(x / patchWidth());
    return QPoint(s, t);
}
QVector2D LevelConfiguration::vertexIDToPosition(int s, int t) const
{
    float x = (static_cast<float>(s) + (t % 2 == 0 ? 0.f : 0.5f)) * vertexWidth();//TODO find out why that works
    float z = static_cast<float>(t) * vertexHeight();
    return QVector2D(x, z);
}

} //namespace terminus

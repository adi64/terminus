#include "polyline.h"

#include <util/mathutil.h>

namespace terminus
{

Polyline::Polyline(const std::vector<QVector3D> & controlPoints)
: m_controlPoints(controlPoints)
{
    calculateKnotSequence();
}

QVector3D Polyline::getPosition(float distance) const
{
    distance = MathUtil::clamp(0.f, length(), distance);
    int i = getIndex(distance);
    float d0 = m_knotSequence[i],
            d1 = m_knotSequence[i+1],
            f = MathUtil::linstep(d0, d1, distance);
    QVector3D v = m_controlPoints[i] * (1.f - f) + m_controlPoints[i + 1] * f;
    return v;
}
QVector3D Polyline::getTangent(float distance) const
{
    distance = MathUtil::clamp(0.f, length(), distance);
    int i = getIndex(distance);
    QVector3D segment = m_controlPoints[i + 1] - m_controlPoints[i];
    return segment.normalized();
}
float Polyline::length() const
{
    return m_tEnd;
}

void Polyline::calculateKnotSequence()
{
    int knotCount = m_controlPoints.size();
    m_knotSequence = std::vector<float>(knotCount);
    m_knotSequence[0] = 0.f;
    for(int i = 1; i < knotCount; i++)
    {
        QVector3D segment = m_controlPoints[i] - m_controlPoints[i - 1];
        m_knotSequence[i] = m_knotSequence[i - 1] + segment.length();
    }
    m_tEnd = m_knotSequence[knotCount - 1];
}

int Polyline::getIndex(float distance) const
{
    for(unsigned int i = 0; i < m_knotSequence.size() - 1; i++)
    {
        if(distance >= m_knotSequence[i] && distance < m_knotSequence[i + 1])
        {
            return i;
        }
    }
    return 0;
}

}//namespace terminus

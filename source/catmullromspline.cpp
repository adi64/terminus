#include "catmullromspline.h"

namespace terminus
{

CatmullRomSpline::CatmullRomSpline(const std::vector<QVector2D> & controlPoints)
: m_controlPoints(controlPoints)
{
    //assert(m_controlPoints.size() >= 4);
    calculateKnotSequence();
}

CatmullRomSpline::~CatmullRomSpline()
{
}

QVector2D CatmullRomSpline::getPosition(float param) const
{
    if(param < 0.f)
        return m_controlPoints[1];
    if(param > length())
        return m_controlPoints[m_controlPoints.size() - 2];
    float t = m_tBegin + param;
    int iBegin = getIndex(t);
    QVector2D p = spline(iBegin, t);
    return p;
}

float CatmullRomSpline::length() const
{
    return m_tEnd - m_tBegin;
}

void CatmullRomSpline::calculateKnotSequence()
{
    int knotCount = m_controlPoints.size();
    m_knotSequence = std::vector<float>(knotCount);
    m_knotSequence[0] = 0.f;
    for(int i = 1; i < knotCount; i++)
    {
        m_knotSequence[i] = m_knotSequence[i-1] + sqrt(ctlPointDistance(i-1));
        //options:   uniform - kS + 1
        //         * centripetal - kS + sqrt(cPD)
        //           chordal - kS + cPD
    }
    m_tBegin = m_knotSequence[1];
    m_tEnd = m_knotSequence[knotCount - 2];
}
float CatmullRomSpline::ctlPointDistance(int i) const
{
    QVector2D pi0 = m_controlPoints[i],
              pi1 = m_controlPoints[i+1];
    float dx = pi1.x() - pi0.x(),
           dy = pi1.y() - pi0.y();
    return sqrt(dx*dx + dy*dy);
}

int CatmullRomSpline::getIndex(float t) const
{
    int searchBegin = 0,
         searchEnd = m_knotSequence.size();
    while(searchBegin < searchEnd)
    {
        int i = searchBegin + (searchEnd - searchBegin) / 2;
        if(t < m_knotSequence[i])
        {
            searchEnd = i;
        }
        else if(t > m_knotSequence[i+1])
        {
            searchBegin = i;
        }
        else
        {
            return i;
        }
    }
    return 1; //ERROR
}

QVector2D CatmullRomSpline::spline(int i1, float t) const
{
    float t0 = m_knotSequence[i1 - 1],
        t1 = m_knotSequence[i1],
        t2 = m_knotSequence[i1 + 1],
        t3 = m_knotSequence[i1 + 2],
        fA1 = 1.f / (t1 - t0),
        tA10 = (t1 - t) * fA1,
        tA11 = (t - t0) * fA1,
        fA2 = 1.f / (t2 - t1),
        tA20 = (t2 - t) * fA2,
        tA21 = (t - t1) * fA2,
        fA3 = 1.f / (t3 - t2),
        tA30 = (t3 - t) * fA3,
        tA31 = (t - t2) * fA3,
        fB1 = 1.f / (t2 - t0),
        tB10 = (t2 - t) * fB1,
        tB11 = (t - t0) * fB1,
        fB2 = 1.f / (t3 - t1),
        tB20 = (t3 - t) * fB2,
        tB21 = (t - t1) * fB2,
        fC = 1.f / (t2 - t1),
        tC0 = (t2 - t) * fC,
        tC1 = (t - t1) * fC;
    QVector2D p0 = m_controlPoints[i1 - 1],
        p1 = m_controlPoints[i1],
        p2 = m_controlPoints[i1 + 1],
        p3 = m_controlPoints[i1 + 2],
        a1 = tA10 * p0 + tA11 * p1,
        a2 = tA20 * p1 + tA21 * p2,
        a3 = tA30 * p2 + tA31 * p3,
        b1 = tB10 * a1 + tB11 * a2,
        b2 = tB20 * a2 + tB21 * a3;
    return tC0 * b1 + tC1 * b2;

}

}//namespace terminus

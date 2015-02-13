#pragma once

#include <vector>

#include <QVector2D>

namespace terminus
{

class CatmullRomSpline
{
public:
    CatmullRomSpline(const std::vector<QVector2D> & controlPoints);
    virtual ~CatmullRomSpline();

    virtual QVector2D getPosition(float param) const;
    virtual float length() const;

protected:
    virtual void calculateKnotSequence();
    virtual float ctlPointDistance(int i) const;

    virtual int getIndex(float t) const;
    virtual QVector2D spline(int i0, float t) const;
protected:
    std::vector<QVector2D> m_controlPoints;
    std::vector<float> m_knotSequence;
    float m_tBegin;
    float m_tEnd;
};

}

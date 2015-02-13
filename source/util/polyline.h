#pragma once

#include <vector>

#include <QVector3D>

namespace terminus
{

class Polyline
{
public:
    Polyline(const std::vector<QVector3D> & controlPoints);
    virtual ~Polyline();

    virtual QVector3D getPosition(float distance) const;
    virtual QVector3D getTangent(float distance) const;
    virtual float length() const;

protected:
    virtual void calculateKnotSequence();
    virtual int getIndex(float distance) const;

protected:
    std::vector<QVector3D> m_controlPoints;

    std::vector<float> m_knotSequence;
    float m_tEnd;
};

}//namespace terminus

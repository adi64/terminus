#pragma once

#include <vector>

#include <QVector3D>

namespace terminus
{

class Polyline
{
public:
    Polyline(const std::vector<QVector3D> & controlPoints = {{0.f, 0.f, 0.f}, {1.f, 0.f, 0.f}});
    virtual ~Polyline();

    QVector3D getPosition(float distance) const;
    QVector3D getTangent(float distance) const;
    float length() const;

protected:
    void calculateKnotSequence();
    int getIndex(float distance) const;

protected:
    std::vector<QVector3D> m_controlPoints;

    std::vector<float> m_knotSequence;
    float m_tEnd;
};

}//namespace terminus

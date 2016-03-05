#pragma once

#include <vector>

#include <QVector3D>

namespace terminus
{

/*!
 * \brief The Polyline class implements a sequence of line segments and is used
 * to approximate a real spline.
 */
class Polyline
{
public:
    explicit Polyline(const std::vector<QVector3D> & controlPoints = {{0.f, 0.f, 0.f}, {1.f, 0.f, 0.f}});

    /*!
     * \param distance
     * \return position on this Polyline at the given distance
     * from the origin position (first control point)
     */
    QVector3D getPosition(float distance) const;

    /*!
     * \param distance
     * \return tangent of this Polyline at the given distance
     * from the origin position (first control point)
     */
    QVector3D getTangent(float distance) const;

    /*!
     * \return maximum distance that this Polyline is defined for
     */
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

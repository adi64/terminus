#pragma once

#include <vector>

#include <QVector2D>

namespace terminus
{

/*!
 * \brief The CatmullRomSpline class implements a spline, that is determined by
 * control points that lie on the spline
 */
class CatmullRomSpline
{
public:
    explicit CatmullRomSpline(const std::vector<QVector2D> & controlPoints);
    virtual ~CatmullRomSpline();

    /*!
     * \brief evaluate the spline function for the given parameter
     * \param param
     * \return point on the spline that corresponds to param
     */
    virtual QVector2D getPosition(float param) const;

    /*!
     * \return the maximum parameter that this spline is defined for
     */
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

#pragma once

#include <QVector3D>
#include <QVector4D>

namespace terminus
{

/*!
 * \brief The Light class represents a light source with a packed,
 * shader compatible memory layout.
 *
 * \sa LightManager
 */
class Light
{
public:
    /*!
     * \brief The type of a light source
     *
     * The type of a light source determines which fields will effectively be used
     */
    enum Type
    {
        AMBIENT     = 0, //! Ambient light, only color is used.
        DIRECTIONAL = 1, //! Directional light, defined by direction and color.
        POINT       = 2, //! Point light, defined by position, attenuation and color.
        SPOT        = 3  //! Spot light, defined by position, direction, cut-off angle, attenuation and color.
    };

    /*!
     * \brief the number of 4d vectors occupied by an instance of this class
     */
    static const int vectorCount = 3;

    /*!
     * \brief a light that has no effect on the scene
     */
    static const Light nullLight;

public:
    static Light createAmbient(const QVector3D & color);
    static Light createDirectional(const QVector3D & color, const QVector3D & direction);
    static Light createPoint(const QVector3D & color, const QVector3D & position, float intensity);
    static Light createSpot(const QVector3D & color, const QVector3D & position, const QVector3D & direction, float intensity, float cutoffDegrees, float cutoffBorder);

public:
    Light(Type type = AMBIENT);

    void setPosition(const QVector3D & position);
    void setDirection(const QVector3D & direction);
    void setColor(const QVector3D & color);
    void setIntensity(float intensity);
    void setCutoff(float cutoffDegrees, float cutoffBorder);

protected:
    QVector4D positionType;
    QVector4D directionIntensity;
    QVector4D colorCutOff;
};

}// namespace terminus

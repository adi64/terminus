#include "light.h"

#include <math.h>

#include <util/mathutil.h>

namespace terminus
{

const Light Light::nullLight;

Light Light::createAmbient(const QVector3D & color)
{
    Light ambient{AMBIENT};
    ambient.setColor(color);
    return ambient;
}

Light Light::createDirectional(const QVector3D & color, const QVector3D & direction)
{
    Light directional{DIRECTIONAL};
    directional.setColor(color);
    directional.setDirection(direction);
    return directional;
}

Light Light::createPoint(const QVector3D & color, const QVector3D & position, float intensity)
{
    Light point{POINT};
    point.setColor(color);
    point.setPosition(position);
    point.setIntensity(intensity);
    return point;
}

Light Light::createSpot(const QVector3D & color, const QVector3D & position, const QVector3D & direction, float intensity, float cutoffDegrees, float cutoffBorder)
{
    Light spot{SPOT};
    spot.setColor(color);
    spot.setPosition(position);
    spot.setDirection(direction);
    spot.setIntensity(intensity);
    spot.setCutoff(cutoffDegrees, cutoffBorder);
    return spot;
}

Light::Light(Type type)
: positionType{0.f, 0.f, 0.f, (float) type}
, directionIntensity{0.f, 0.f, 0.f, 1.f}
, colorCutOff{0.f, 0.f, 0.f, 0.f}
{
}

void Light::setPosition(const QVector3D & position)
{
    positionType.setX(position.x());
    positionType.setY(position.y());
    positionType.setZ(position.z());
}

void Light::setDirection(const QVector3D & direction)
{
    directionIntensity.setX(direction.x());
    directionIntensity.setY(direction.y());
    directionIntensity.setZ(direction.z());
}

void Light::setColor(const QVector3D & color)
{
    colorCutOff.setX(color.x());
    colorCutOff.setY(color.y());
    colorCutOff.setZ(color.z());
}

void Light::setIntensity(float intensity)
{
    directionIntensity.setW(intensity);
}

void Light::setCutoff(float cutoffDegrees, float cutoffBorder)
{
    colorCutOff.setW(floor(MathUtil::clamp(0.f, 90.f, cutoffDegrees)) + MathUtil::clamp(0.f, 1.f, cutoffBorder) * 0.5f);
}

void Light::setActive(bool active)
{
    int type = (int) positionType.w();
    if(active)
    {
        type = type % 4;
    }
    else
    {
        type = 4 + type % 4;
    }
    positionType.setW((int) type);
}

bool Light::isActive()
{
    int type = (int) positionType.w();
    return type == (type % 4);
}

}// namespace terminus

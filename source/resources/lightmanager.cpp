#include "lightmanager.h"

#include <assert.h>
#include <math.h>

#include <QDebug>

#include <util/tostring.h>
#include <util/mathutil.h>

namespace terminus
{

const Light Light::nullLight{{0.0, 0.0, 0.0}, // position
                               {0.0, 0.0, 0.0}, // direction
                               {0.0, 0.0, 0.0}, // color
                               {1.0, 0.0, 0.0}, // attenuation
                               1.0,             //spot cutoff
                               LIGHT_AMBIENT};  //type

const QVector3D Light::defaultAttenuation{1.0, 0.0, 0.001}; //default quadratic attenuation

Light Light::createAmbient(const QVector3D & color)
{
    return {nullLight.position,
            nullLight.direction,
            color,
            nullLight.attenuation,
            nullLight.spotCutOff,
            LIGHT_AMBIENT};
}

Light Light::createDirectional(const QVector3D & color, const QVector3D & direction)
{
    return {nullLight.position,
            direction,
            color,
            nullLight.attenuation,
            nullLight.spotCutOff,
            LIGHT_DIRECTIONAL};
}

Light Light::createPoint(const QVector3D & color, const QVector3D & position)
{
    return {position,
            nullLight.direction,
            color,
            defaultAttenuation,
            nullLight.spotCutOff,
            LIGHT_POINT};
}

Light Light::createSpot(const QVector3D &color, const QVector3D &position, const QVector3D &direction, float cutoffAngle)
{
    return {position,
            direction,
            color,
            defaultAttenuation,
            static_cast<float>(cos(cutoffAngle)),
            LIGHT_SPOT};
}

LightManager::LightManager()
    : m_freeLightID(0)
{
}

LightID LightManager::addAmbientLight(const QVector3D &color)
{
    return addLight(Light::createAmbient(color));
}

LightID LightManager::addDirectionalLight(const QVector3D & direction, const QVector3D & color)
{
    return addLight(Light::createDirectional(color, direction));
}

LightID LightManager::addPointLight(const QVector3D & position, const QVector3D &, const QVector3D & color)
{
    return addLight(Light::createPoint(color, position));
}

LightID LightManager::addSpotLight(const QVector3D & position, const QVector3D & direction, const QVector3D &, const QVector3D & color)
{
    return addLight(Light::createSpot(color, position, direction, 30.f * MathUtil::PI / 180.f));
}

void LightManager::setUniforms(Program & program) const
{
    int i = 0;
    for(const auto & light : m_lights)
    {
        setLightUniforms(program, i, light.second);
        ++i;
    }

    // set all unused lights to invalid
    for(; i<maxLights; ++i)
    {
        setLightUniforms(program, i, Light::nullLight);
    }
}

Light & LightManager::light(LightID lightID)
{
    return m_lights.at(lightID);
}

const std::map<LightID, Light> &LightManager::lights() const
{
    return m_lights;
}

void LightManager::setLightUniforms(Program &program, int index, const Light & light) const
{
    //TODO perhaps the string concatenation is the reason for performance issues
    auto currentLightString = std::string("lights[" + toString(index) + "]");
    program.setUniform(currentLightString + ".position",     light.position);
    program.setUniform(currentLightString + ".direction",    light.direction);
    program.setUniform(currentLightString + ".color",        light.color);
    program.setUniform(currentLightString + ".attenuation",  light.attenuation);
    program.setUniform(currentLightString + ".spotCutOff",   light.spotCutOff);
    program.setUniform(currentLightString + ".type",         light.type);
}

LightID LightManager::addLight(const Light &light)
{
    assert(m_lights.size() <= maxLights);

    auto lightID = getFreeLightID();

    std::unique_lock<std::mutex> lock(m_mutex);
    m_lights.insert(std::pair<LightID, Light>(lightID, light));
    lock.unlock();

    return lightID;
}

LightID LightManager::getFreeLightID()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    LightID reservedID = m_freeLightID;
    m_freeLightID++;

    lock.unlock();

    return reservedID;
}

} // namespace terminus

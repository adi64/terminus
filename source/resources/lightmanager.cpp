#include "lightmanager.h"

#include <cassert>

#include <QDebug>

#include <util/tostring.h>

namespace terminus
{

LightManager::LightManager()
    : m_freeLightID(0)
{

}

LightID LightManager::addAmbientLight(const QVector3D &color)
{
    Light newLight;
    newLight.type = LIGHT_AMBIENT;
    newLight.color = color;

    auto lightID = addLight(newLight);
    return lightID;
}

LightID LightManager::addDirectionalLight(const QVector3D &direction, const QVector3D &color)
{
    Light newLight;
    newLight.type = LIGHT_DIRECTIONAL;
    newLight.direction = direction;
    newLight.color = color;

    auto lightID = addLight(newLight);
    return lightID;
}

void LightManager::setShaderValues(Program &shaderProgram) const
{
    int i = 0;
    for(auto &light : m_lights)
    {
        auto currentLightString = std::string("lights[" + toString(i) + "]");
        shaderProgram.setUniform(currentLightString + ".position",     light.second.position);
        shaderProgram.setUniform(currentLightString + ".direction",    light.second.direction);
        shaderProgram.setUniform(currentLightString + ".color",        light.second.color);
        shaderProgram.setUniform(currentLightString + ".attenuation",  light.second.attenuation);
        shaderProgram.setUniform(currentLightString + ".spotCutOff",   light.second.spotCutOff);
        shaderProgram.setUniform(currentLightString + ".type",         light.second.type);
        ++i;
    }

    // set all unused lights to invalid
    for(; i<maxLights; ++i)
    {
        auto currentLightString = std::string("lights[" + toString(i) + "]");
        shaderProgram.setUniform(currentLightString + ".type",         LIGHT_INVALID);
    }
}

Light &LightManager::light(LightID lightID)
{
    return m_lights.at(lightID);
}

const std::map<LightID, Light> &LightManager::lights() const
{
    return m_lights;
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

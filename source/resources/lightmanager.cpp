#include "lightmanager.h"

#include <cassert>

namespace terminus
{

LightManager::LightManager()
    : m_freeLightID(0)
{

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
        shaderProgram.setUniform(std::string("lights[" + std::to_string(i) + "].position"),     light.second.position);
        shaderProgram.setUniform(std::string("lights[" + std::to_string(i) + "].direction"),    light.second.direction);
        shaderProgram.setUniform(std::string("lights[" + std::to_string(i) + "].color"),        light.second.color);
        shaderProgram.setUniform(std::string("lights[" + std::to_string(i) + "].attenuation"),  light.second.attenuation);
        shaderProgram.setUniform(std::string("lights[" + std::to_string(i) + "].spotCutOff"),   light.second.spotCutOff);
        shaderProgram.setUniform(std::string("lights[" + std::to_string(i) + "].type"),         light.second.type);

        ++i;
    }

    // compatibility: set lightDirection
    shaderProgram.setUniform(std::string("lightDirection"), m_lights.at(0).direction);
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

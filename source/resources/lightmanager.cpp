#include "lightmanager.h"

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
    if(m_lights.size() < 1)
    {
        return;
    }

    // just set first directional light for now
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

#include "lightmanager.h"

namespace terminus
{

LightManager::LightManager()
    : m_freeLightID(0)
{

}

LightID LightManager::addDirectionalLight(const QVector3D &position, const QVector3D &direction)
{
    Light newLight;
    newLight.type = LIGHT_DIRECTIONAL;
    newLight.position = position;
    newLight.direction = direction;

    auto lightID = addLight(newLight);
    return lightID;
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

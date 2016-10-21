#include "lightmanager.h"

#include <cassert>

#include <resources/program.h>

namespace terminus
{

int LightManager::freeSlots() const
{
    return slotCount - m_usedSlots.size();
}

LightManager::ID LightManager::add(const Light & light)
{
    ID slot = getFreeSlot();
    m_usedSlots.insert(slot);
    m_lights[slot] = light;
    return slot;
}

Light & LightManager::get(LightManager::ID id)
{
    return m_lights[id];
}

void LightManager::remove(LightManager::ID slot)
{
    m_lights[slot] = Light::nullLight;
    m_usedSlots.erase(slot);
}

void LightManager::setUniforms(Program & program) const
{
    program.setUniform(std::string("u_light"), reinterpret_cast<const QVector4D*>(&m_lights), slotCount * Light::vectorCount);
}

LightManager::ID LightManager::getFreeSlot()
{
    for(ID i = 0; i < LightManager::slotCount; i++)
    {
        if(m_usedSlots.count(i) == 0)
        {
            return i;
        }
    }
    assert(false);// this is reached if no free slot is left
    return 0;
}

} //namespace terminus

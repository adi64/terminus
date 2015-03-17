#pragma once

#include <set>

#include <QVector3D>

#include <resources/program.h>
#include <world/light.h>

namespace terminus
{

class LightManager
{
public:
    using ID = unsigned int;

    static constexpr int slotCount = 8;

    /*!
     * \brief Adds a constructed light to the internal storage
     * \param light The light to be added
     * \return The ID of the newly created light
     */
    int freeSlots() const;
    ID add(const Light & light);
    Light & get(ID slot);
    void remove(ID slot);

    void setUniforms(Program & program) const;

protected:
    /*!
     * \brief Return a free LightID
     * \return A free LightID
     */
    ID getFreeSlot();

protected:
    Light m_lights[slotCount];

    std::set<ID> m_usedSlots;
};

} // namespace terminus

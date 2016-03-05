#pragma once

#include <set>

#include <QVector3D>

#include <world/light.h>

namespace terminus
{

class Program;

/*!
 * \brief The LightManager class manages slots that can be filled with
 * Light instances and is able to set those lights as a uniform array
 * on a shader program.
 *
 * \sa Light
 */
class LightManager
{
public:
    using ID = unsigned int;

    /*!
     * \brief number of slots managed by this class
     */
    static const int slotCount = 8;

    /*!
     * \return the number of yet unoccupied light slots
     */
    int freeSlots() const;

    /*!
     * \brief Inserts a light in the next free slot
     * \param light
     * \return the number of the slot that the light was inserted in
     *
     * It is an invalid operation to call this method if freeSlots() == 0
     */
    ID add(const Light & light);

    /*!
     * \param slot
     * \return get a reference to the Light instance contained in a particular
     * slot
     */
    Light & get(ID slot);

    /*!
     * \brief mark the given slot as free and store the Light#nullLight in it
     * \param slot
     */
    void remove(ID slot);

    /*!
     * \brief sets the lights managed by this class as a uniform array
     * \param program
     */
    void setUniforms(Program & program) const;

protected:
    ID getFreeSlot();

protected:
    Light m_lights[slotCount];

    std::set<ID> m_usedSlots;
};

} //namespace terminus

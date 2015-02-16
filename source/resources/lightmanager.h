#pragma once

#include <map>
#include <mutex>

#include <QVector3D>

#include <resources/program.h>

namespace terminus
{

using LightID = unsigned int;

enum LightType
{
    LIGHT_AMBIENT = 0,
    LIGHT_DIRECTIONAL = 1,
    LIGHT_POINT = 2,
    LIGHT_SPOT = 3
};

class Light
{
public:
    QVector3D position;
    QVector3D direction;
    LightType type;
};

class LightManager
{
public:
    LightManager();

    LightID addDirectionalLight(const QVector3D &direction);

    void setShaderValues(Program &shaderProgram) const;

    Light &light(LightID lightID);
    const std::map<LightID, Light> &lights() const;
protected:
    /*!
     * \brief Adds a constructed light to the internal storage
     * \param light The light to be added
     * \return The ID of the nwely created light
     *
     * This method is thread-safe.
     */
    LightID addLight(const Light& light);

    /*!
     * \brief Reserve and return the next free LightID
     * \return A reserved, free LightID
     *
     * This method is thread-safe.
     */
    LightID getFreeLightID();

    std::map<LightID, Light> m_lights;
    LightID m_freeLightID;

    std::mutex m_mutex;
private:
};

} // namespace terminus

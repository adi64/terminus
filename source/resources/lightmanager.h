#pragma once

#include <map>
#include <mutex>

#include <QVector3D>

#include <resources/program.h>

namespace terminus
{

using LightID = unsigned int;

/*!
 * \brief The type of a light source
 *
 * The type of a light source determines which fields will effectively be used
 */
enum LightType
{
    LIGHT_AMBIENT       = 0, //! Ambient light, only color is used.
    LIGHT_DIRECTIONAL   = 1, //! Directional light, defined by direction and color.
    LIGHT_POINT         = 2, //! Point light, defined by position, attenuation and color.
    LIGHT_SPOT          = 3  //! Spot light, defined by position, direction, cut-off angle, attenuation and color.
};

/*!
 * \brief A represenation of a light source.
 */
class Light
{
public:
    QVector3D position;
    QVector3D direction;
    QVector3D color;
    QVector3D attenuation;  //! contains the constant, linear and quadratic attenuation factors
    float spotCutOff;       //! cosine of cut-off angle of spotlight
    LightType type;
};

class LightManager
{
public:
    LightManager();

    LightID addAmbientLight(const QVector3D &color);
    LightID addDirectionalLight(const QVector3D &direction, const QVector3D &color);
    LightID addPointLight(const QVector3D &position, const QVector3D &attenuation, const QVector3D &color);
    LightID addSpotLight(const QVector3D &position, const QVector3D &direction, const QVector3D &attenuation, const QVector3D &color);

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

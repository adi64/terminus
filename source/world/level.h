#pragma once

#include <vector>
#include <memory>

#include <QVector2D>
#include <QVector3D>

#include <util/catmullromspline.h>
#include <util/perlinnoise.h>
#include <util/polyline.h>
#include <world/levelconfiguration.h>

namespace terminus
{

/*!
 * \brief The Level class is a generator for level data such as
 * terrain geometry and track courses.
 *
 * It uses a LevelConfiguration instance that specifies the dimensions of the
 * generated terrain and a seed value for the pseudo random number generator.
 *
 * \sa LevelConfiguration
 */
class Level
{
public:
    Level();
    Level(const LevelConfiguration & config);

    /*!
     * \brief generate terrain and tracks if not already generated
     */
    void generateLevel();
    /*!
     * \brief discard terrain and tracks if already generated
     */
    void resetLevel();

    /*!
     * \brief set a new configuration and discard the previously generated
     * terrain and tracks if already generated
     * \param config
     */
    void configure(const LevelConfiguration & config);
    /*!
     * \return the current LevelConfiguration
     */
    const LevelConfiguration & config() const;

    /*!
     * \return a copy of the generated terrain displacement map
     * that can be used to fill an OpenGL texture object
     */
    std::vector<GLfloat> * copyTerrainMapData() const;
    /*!
     * \return a newly created Polyline instance that approximates
     * the course of the right track
     */
    Polyline * createRightTrackCourse() const;
    /*!
     * \return a newly created Polyline instance that approximates
     * the course of the left track
     */
    Polyline * createLeftTrackCourse() const;

protected:
    float trackHeight() const;

    void generateTracks();

    void generateTerrainMap();
    QVector2D terrainDisplacement(float x, float z);
    void setTrackEnvironment(const CatmullRomSpline & track);
    float terrainHeight(float x, float z, float fTrack);

    int tMapIndex(int s, int t) const;
    void tMapSetXYZ(int i, float dx, float dy, float dz);
    QVector3D tMapGetXYZ(int i) const;
    void tMapSetW(int i, float w);
    float tMapGetW(int i) const;

protected:
    LevelConfiguration m_config;

    PerlinNoise m_noise;

    bool m_tracksGenerated;
    float m_trackHeight; // might become a PerlinNoise instance...
    std::unique_ptr<CatmullRomSpline> m_rightTrack;
    std::unique_ptr<CatmullRomSpline> m_leftTrack;

    bool m_texGenerated;
    std::vector<GLfloat> m_terrainMapData;

};

}

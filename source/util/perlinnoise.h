#pragma once

#include <vector>
#include <random>
#include <map>

#include <QVector2D>

namespace terminus
{

/*!
 * \brief The PerlinNoise class implements a two dimensional noise function
 * using multiple selectable gradient vector grids (layers).
 */
class PerlinNoise
{
protected:
    static const float fScale;
public:
    PerlinNoise();
    PerlinNoise(unsigned int seed);

    /*!
     * \brief seeds the underlying random number generator
     * and invalidates the previously generated gradient grids
     * \param seed
     */
    void seed(unsigned int seed);
    /*!
     * \return the seed that was last used to seed the underlying
     * random number generator
     */
    unsigned int seed() const;

    /*!
     * \brief evaluates the noise function at the given coordinates
     * \param layer
     * \param x
     * \param y
     * \return noise value
     *
     * The coordinates can exceed the size of the gradient grid,
     * as they will be wrapped.
     */
    float noise(int layer, float x, float y) const;

    /*!
     * \return a single random value from [-1.0;1.0]
     */
    float symmetricRnd() const;
    /*!
     * \return a single random value from [0.0;1.0]
     */
    float asymmetricRnd() const;

protected:
    float dotGradient(int layer, int iX, int iY, float x, float y) const;

    int gradientIndex(int iX, int iY) const;
    void generateGradients(int layer) const;

    std::vector<QVector2D> & gradients(int layer) const;

protected:
    unsigned int m_seed;
    mutable std::default_random_engine m_rng;
    mutable std::uniform_real_distribution<float> m_distSym;
    mutable std::uniform_real_distribution<float> m_distAsym;

    int m_gradientCount;
    mutable std::map<int,std::vector<QVector2D>> m_gradientGrid;
};

}//namespace terminus

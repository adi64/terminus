#pragma once

#include <vector>
#include <random>
#include <map>

#include <QVector2D>

namespace terminus
{

class PerlinNoise
{
protected:
    static const float fScale;
public:
    PerlinNoise();
    PerlinNoise(unsigned int seed);
    virtual ~PerlinNoise();

    unsigned int seed() const;
    void seed(unsigned int seed);

    float noise(int layer, float x, float y);

    float symmetricRnd();
    float asymmetricRnd();

protected:
    float dotGradient(int layer, int iX, int iY, float x, float y);

    int gradientIndex(int iX, int iY);
    void generateGradients(int layer);

    std::vector<QVector2D> & gradients(int layer);

protected:
    unsigned int m_seed;
    std::default_random_engine m_rng;
    std::uniform_real_distribution<float> m_distSym;
    std::uniform_real_distribution<float> m_distAsym;

    int m_gradientCount;
    std::map<int,std::vector<QVector2D>> m_gradientGrid;
};

}//namespace terminus

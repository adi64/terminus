#pragma once

#include <vector>
#include <random>

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

    virtual unsigned int seed();
    virtual float noise(float x, float y);

protected:
    virtual float symmetricRnd();
    virtual float asymmetricRnd();

    virtual float dotGradient(int iX, int iY, float x, float y);

    virtual int gradientIndex(int iX, int iY);
    virtual void generateGradients();
protected:
    unsigned int m_seed;
    std::default_random_engine m_rng;
    std::uniform_real_distribution<float> m_distSym;
    std::uniform_real_distribution<float> m_distAsym;

    int m_gradientCount;
    std::vector<QVector2D> m_gradients;
};

}//namespace terminus

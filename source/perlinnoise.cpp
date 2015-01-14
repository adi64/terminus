#include "perlinnoise.h"

#include "mathutil.h"

#include <chrono>

namespace terminus
{
const float PerlinNoise::fScale = 1.f / sqrt(0.5f);

PerlinNoise::PerlinNoise()
    : PerlinNoise(std::chrono::system_clock::now().time_since_epoch().count())
{
}
PerlinNoise::PerlinNoise(unsigned int seed)
: m_seed(seed)
, m_rng(seed)
, m_distSym(-1.f, 1.f)
, m_distAsym(0.f, 1.f)
, m_gradientCount(256)
, m_gradients(m_gradientCount * m_gradientCount)
{
    generateGradients();
}
PerlinNoise::~PerlinNoise()
{
}

unsigned int PerlinNoise::seed()
{
    return m_seed;
}

float PerlinNoise::noise(float x, float y)
{
    int x0 = std::floor(x),
         x1 = x0 + 1,
         y0 = std::floor(y),
         y1 = y0 + 1;

    float sx = MathUtil::smoothstep(static_cast<float>(x0), static_cast<float>(x1), x);
    float sy = MathUtil::smoothstep(static_cast<float>(y0), static_cast<float>(y1), y);

    float n0, n1, ix0, ix1, value;
    n0 = dotGradient(x0, y0, x, y);
    n1 = dotGradient(x1, y0, x, y);
    ix0 = MathUtil::mix(n0, n1, sx);
    n0 = dotGradient(x0, y1, x, y);
    n1 = dotGradient(x1, y1, x, y);
    ix1 = MathUtil::mix(n0, n1, sx);
    value = MathUtil::mix(ix0, ix1, sy);
    return MathUtil::clamp(-1.f, 1.f, value * fScale);
}

float PerlinNoise::dotGradient(int iX, int iY, float x, float y) {
    QVector2D gradient = m_gradients[gradientIndex(iX, iY)];
     float dx = x - static_cast<float>(iX),
            dy = y - static_cast<float>(iY);
     return dx * gradient.x() + dy * gradient.y();
 }

float PerlinNoise::symmetricRnd()
{
    return m_distSym(m_rng);
}

float PerlinNoise::asymmetricRnd()
{
    return m_distAsym(m_rng);
}

int PerlinNoise::gradientIndex(int iX, int iY)
{
    iY %= m_gradientCount;
    iX %= m_gradientCount;
    iY = (iY < 0) ? m_gradientCount + iY : iY;
    iX = (iX < 0) ? m_gradientCount + iX : iX;
    return iY * m_gradientCount + iX;
}

void PerlinNoise::generateGradients()
{
    for(int iY = 0; iY < m_gradientCount; iY++)
    {
        for(int iX = 0; iX < m_gradientCount; iX++)
        {
            float alpha = asymmetricRnd() * 2 * MathUtil::PI;
            QVector2D grad(std::sin(alpha), std::cos(alpha));
            m_gradients[gradientIndex(iX, iY)] = grad;
        }
    }
}

}//namespace terminus

#pragma once

#include "abstractgraphicsobject.h"
#include "perlinnoise.h"

class Scene;

namespace terminus
{

class SnowStorm : public AbstractGraphicsObject
{
public:
    SnowStorm(std::shared_ptr<Scene> scene);
    virtual ~SnowStorm();

    void update(int elapsedMilliseconds);
    virtual void preRender(QOpenGLFunctions & gl, Program & program) const override;
    virtual void postRender(QOpenGLFunctions & gl, Program & program) const override;

    int width();
    std::vector<float> & flakes();

protected:
    void initializeFlakes();
    void initializeTextures(QOpenGLFunctions & gl) const;

    float m_width;
    std::vector<float> m_flakes;
    PerlinNoise m_noise;

    mutable bool m_initialized;
    mutable GLuint m_texture;
};

}

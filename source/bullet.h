#pragma once

#include <memory>

#include "abstractgraphicsobject.h"

namespace terminus
{

class Bullet : public AbstractGraphicsObject
{
public:
    Bullet(Scene *scene);

    void update(int elapsedMilliseconds) override;
    void render(QOpenGLFunctions& gl) const override;

protected:
    virtual unsigned int maxAgeInMilliseconds() const;
    unsigned int m_ageInMilliseconds;
};

}

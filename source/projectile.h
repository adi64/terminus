#pragma once

#include <memory>

#include "abstractgraphicsobject.h"

namespace terminus
{

class Projectile : public AbstractGraphicsObject
{
public:
    Projectile(Scene *scene);

    void setPosition(const QVector3D &newPosition) override;

    void update(int elapsedMilliseconds) override;
    void render(QOpenGLFunctions& gl) const override;

protected:
    virtual unsigned int maxAgeInMilliseconds() const;
    unsigned int m_ageInMilliseconds;
};

}

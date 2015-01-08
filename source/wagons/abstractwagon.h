#pragma once

#include "../abstractphysicsobject.h"

namespace terminus
{

class Scene;
class Train;

class AbstractWagon : public AbstractPhysicsObject
{
public:
    AbstractWagon(Scene *scene, Train *train);

    virtual void primaryAction();

    virtual void update(int elapsedMilliseconds) override;

    virtual float length() const;
    virtual float weight() const;
    void setPositionOffset(float accumulatedOffset);
    QVector3D position() const override;

protected:
    float m_positionOffset;
    Train *m_train;
};

}

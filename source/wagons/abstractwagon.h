#pragma once

#include "../kinematicphysicsobject.h"

namespace terminus
{
class Train;

class AbstractWagon : public AbstractGraphicsObject
{
public:
    AbstractWagon(std::shared_ptr<Scene> scene, Train *train);

    virtual void primaryAction();

    virtual void update(int elapsedMilliseconds) override;

    virtual float length() const;
    virtual float weight() const;
    void setPositionOffset(float accumulatedOffset);
    QVector3D position() const override;
    QVector3D tangent() const;
    void update(int elapsedMilliseconds) override;

protected:
    float m_positionOffset;
    float m_weight;
    Train *m_train;
};

}

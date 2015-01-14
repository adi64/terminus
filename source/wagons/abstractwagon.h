#pragma once

#include "../abstractgraphicsobject.h"

namespace terminus
{

class Scene;
class Train;

class AbstractWagon : public AbstractGraphicsObject
{
public:
    AbstractWagon(Scene *scene, Train *train);
    virtual float length() const;
    virtual float weight() const;
    void setPositionOffset(float accumulatedOffset);
    QVector3D position() const;
    QVector3D tangent() const;
    void update(int elapsedMilliseconds) override;

protected:
    float m_positionOffset;
    float m_weight;
    Train *m_train;
};

}

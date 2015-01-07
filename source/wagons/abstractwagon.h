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

    virtual void primaryAction();

    virtual float length() const;
    virtual float weight() const;
    void setPositionOffset(float accumulatedOffset);
    QVector3D position() const override;

protected:
    float m_positionOffset;
    Train *m_train;
};

}

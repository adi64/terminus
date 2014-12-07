#pragma once

#include "../abstractgraphicsobject.h"

namespace terminus
{

class Scene;

class AbstractWagon : public AbstractGraphicsObject
{
public:
    AbstractWagon(Scene *scene);
    virtual float length() const;
    void setPositionOffset(float accumulatedOffset);

protected:
    float m_positionOffset;
};

}

#pragma once

#include "abstractgraphicsobject.h"

namespace terminus
{

class AbstractWagon : public AbstractGraphicsObject
{
public:
    AbstractWagon();
    static float length();
    void setPositionOffset(float accumulatedOffset);

protected:
    float m_positionOffset;

};

}

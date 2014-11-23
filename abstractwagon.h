#pragma once

#include "abstractgraphicsobject.h"

namespace terminus
{

class AbstractWagon : public AbstractGraphicsObject
{
public:
    AbstractWagon();
protected:
    float m_positionOffset;
private:
};

}

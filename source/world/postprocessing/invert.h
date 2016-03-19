#pragma once

#include <QMatrix4x4>

#include <world/postprocessing/abstracteffect.h>

namespace terminus
{

class Invert : public AbstractEffect
{
public:
    Invert(World & world, DisabledBehaviour behaviour = DisabledBehaviour::PASS);
    virtual ~Invert();

protected:
};

}
